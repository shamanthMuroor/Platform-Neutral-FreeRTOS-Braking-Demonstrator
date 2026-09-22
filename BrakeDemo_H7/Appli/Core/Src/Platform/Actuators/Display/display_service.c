/*
 *
 *
 * Each text line maps one-to-one onto an SSD1306 page, because the font
 * cell height (8 px) equals the page height (8 px). Line N is page N.
 *
 * A line is only re-rasterised when its content actually changed. That,
 * combined with the driver's dirty-page tracking, means a screen where
 * only the slip value moves costs a single 129-byte I2C transfer.
 *
 * Number formatting is hand-rolled integer-to-ASCII. snprintf is avoided
 * deliberately: it pulls in a large newlib formatter and, depending on
 * link options, float support that has no place in this project.
 */

#include "display_service.h"
#include "ssd1306_oled_driver.h"
#include "i2c_bus.h"

#include <string.h>

#define DISPLAY_LINE_BUFFER_SIZE  (DISPLAY_CHARACTERS_PER_LINE + 1U)

typedef enum
{
    DISPLAY_LINE_KIND_TEXT = 0,
    DISPLAY_LINE_KIND_BAR
} DisplayLineKind;

typedef struct
{
    DisplayLineKind kind;
    char            text[DISPLAY_LINE_BUFFER_SIZE];
    uint8_t         fill_percent;
    bool            needs_redraw;
} DisplayLineState;

static DisplayLineState display_lines[DISPLAY_TEXT_LINE_COUNT];
static bool             display_is_available = false;

/* ----------------------------------------------------------------------- */
/* Small integer formatting helpers                                         */
/* ----------------------------------------------------------------------- */

/* Appends a NUL-terminated string, never overflowing the destination. */
static uint8_t DisplayService_AppendText(char *destination,
                                         uint8_t write_position,
                                         const char *text)
{
    if (text == NULL)
    {
        return write_position;
    }

    while ((*text != '\0') && (write_position < DISPLAY_CHARACTERS_PER_LINE))
    {
        destination[write_position] = *text;
        write_position++;
        text++;
    }

    return write_position;
}

static uint8_t DisplayService_AppendCharacter(char *destination,
                                              uint8_t write_position,
                                              char character)
{
    if (write_position < DISPLAY_CHARACTERS_PER_LINE)
    {
        destination[write_position] = character;
        write_position++;
    }

    return write_position;
}

/*
 * Appends an unsigned value, optionally zero-padded to minimum_digits.
 * Maximum 10 digits covers the full uint32_t range.
 */
static uint8_t DisplayService_AppendUnsignedInteger(char *destination,
                                                    uint8_t write_position,
                                                    uint32_t value,
                                                    uint8_t minimum_digits)
{
    char    digit_stack[10];
    uint8_t digit_count = 0U;

    do
    {
        digit_stack[digit_count] = (char)('0' + (char)(value % 10U));
        digit_count++;
        value /= 10U;
    }
    while ((value != 0U) && (digit_count < (uint8_t)sizeof(digit_stack)));

    while (digit_count < minimum_digits)
    {
        digit_stack[digit_count] = '0';
        digit_count++;
    }

    while (digit_count > 0U)
    {
        digit_count--;
        write_position = DisplayService_AppendCharacter(destination,
                                                        write_position,
                                                        digit_stack[digit_count]);
    }

    return write_position;
}

static uint8_t DisplayService_AppendSignedInteger(char *destination,
                                                  uint8_t write_position,
                                                  int32_t value)
{
    uint32_t magnitude;

    if (value < 0)
    {
        write_position = DisplayService_AppendCharacter(destination, write_position, '-');
        magnitude = (uint32_t)(-(value + 1)) + 1U;   /* safe for INT32_MIN */
    }
    else
    {
        magnitude = (uint32_t)value;
    }

    return DisplayService_AppendUnsignedInteger(destination, write_position, magnitude, 1U);
}

static uint8_t DisplayService_DecimalDigitsForScaling(uint16_t scaling_factor)
{
    uint8_t decimal_digits = 0U;

    while ((scaling_factor > 1U) && (decimal_digits < 4U))
    {
        scaling_factor = (uint16_t)(scaling_factor / 10U);
        decimal_digits++;
    }

    return decimal_digits;
}

/* ----------------------------------------------------------------------- */
/* Line content management                                                 */
/* ----------------------------------------------------------------------- */

static void DisplayService_CommitLineText(uint8_t line_index, const char *composed_text)
{
    DisplayLineState *line;

    if (line_index >= DISPLAY_TEXT_LINE_COUNT)
    {
        return;
    }

    line = &display_lines[line_index];

    if ((line->kind == DISPLAY_LINE_KIND_TEXT) &&
        (strncmp(line->text, composed_text, DISPLAY_CHARACTERS_PER_LINE) == 0))
    {
        return;   /* unchanged, nothing to redraw */
    }

    line->kind = DISPLAY_LINE_KIND_TEXT;
    memset(line->text, 0, DISPLAY_LINE_BUFFER_SIZE);
    strncpy(line->text, composed_text, DISPLAY_CHARACTERS_PER_LINE);
    line->needs_redraw = true;
}

bool DisplayService_Initialize(void)
{
    uint8_t line_index;

    for (line_index = 0U; line_index < DISPLAY_TEXT_LINE_COUNT; line_index++)
    {
        display_lines[line_index].kind         = DISPLAY_LINE_KIND_TEXT;
        display_lines[line_index].fill_percent = 0U;
        display_lines[line_index].needs_redraw = true;
        memset(display_lines[line_index].text, 0, DISPLAY_LINE_BUFFER_SIZE);
    }

    I2cBus_Initialize();
    display_is_available = Ssd1306_Initialize();

    return display_is_available;
}

bool DisplayService_IsAvailable(void)
{
    return display_is_available;
}

void DisplayService_ClearAllLines(void)
{
    uint8_t line_index;

    for (line_index = 0U; line_index < DISPLAY_TEXT_LINE_COUNT; line_index++)
    {
        DisplayService_SetLineText(line_index, "");
    }
}

void DisplayService_SetLineText(uint8_t line_index, const char *text)
{
    char    composed_text[DISPLAY_LINE_BUFFER_SIZE];
    uint8_t write_position;

    memset(composed_text, 0, sizeof(composed_text));
    write_position = DisplayService_AppendText(composed_text, 0U, text);
    composed_text[write_position] = '\0';

    DisplayService_CommitLineText(line_index, composed_text);
}

void DisplayService_SetLineFixedPointValue(uint8_t line_index,
                                           const char *label,
                                           int32_t scaled_value,
                                           uint16_t scaling_factor,
                                           const char *unit_text)
{
    char     composed_text[DISPLAY_LINE_BUFFER_SIZE];
    uint8_t  write_position;
    uint8_t  decimal_digits;
    int32_t  whole_part;
    uint32_t fractional_part;

    if (scaling_factor == 0U)
    {
        scaling_factor = 1U;
    }

    decimal_digits = DisplayService_DecimalDigitsForScaling(scaling_factor);

    whole_part = scaled_value / (int32_t)scaling_factor;

    {
        int32_t remainder = scaled_value % (int32_t)scaling_factor;
        fractional_part = (uint32_t)((remainder < 0) ? -remainder : remainder);
    }

    memset(composed_text, 0, sizeof(composed_text));
    write_position = DisplayService_AppendText(composed_text, 0U, label);
    write_position = DisplayService_AppendCharacter(composed_text, write_position, ' ');

    /* A value like -0.123 has whole_part 0, so the sign must be emitted
     * explicitly before the integer part. */
    if ((scaled_value < 0) && (whole_part == 0))
    {
        write_position = DisplayService_AppendCharacter(composed_text, write_position, '-');
    }

    write_position = DisplayService_AppendSignedInteger(composed_text, write_position, whole_part);

    if (decimal_digits > 0U)
    {
        write_position = DisplayService_AppendCharacter(composed_text, write_position, '.');
        write_position = DisplayService_AppendUnsignedInteger(composed_text,
                                                              write_position,
                                                              fractional_part,
                                                              decimal_digits);
    }

    if (unit_text != NULL)
    {
        write_position = DisplayService_AppendCharacter(composed_text, write_position, ' ');
        write_position = DisplayService_AppendText(composed_text, write_position, unit_text);
    }

    composed_text[write_position] = '\0';

    DisplayService_CommitLineText(line_index, composed_text);
}

void DisplayService_SetLineTwoIntegers(uint8_t line_index, const char *label, const char *left_tag, int32_t left_value, const char *right_tag, int32_t right_value) {
    char    composed_text[DISPLAY_LINE_BUFFER_SIZE];
    uint8_t write_position;

    memset(composed_text, 0, sizeof(composed_text));

    write_position = DisplayService_AppendText(composed_text, 0U, label);
    write_position = DisplayService_AppendCharacter(composed_text, write_position, ' ');
    write_position = DisplayService_AppendText(composed_text, write_position, left_tag);
    write_position = DisplayService_AppendSignedInteger(composed_text, write_position, left_value);
    write_position = DisplayService_AppendCharacter(composed_text, write_position, ' ');
    write_position = DisplayService_AppendText(composed_text, write_position, right_tag);
    write_position = DisplayService_AppendSignedInteger(composed_text, write_position, right_value);

    composed_text[write_position] = '\0';

    DisplayService_CommitLineText(line_index, composed_text);
}

void DisplayService_SetLineBar(uint8_t line_index, uint8_t fill_percent) {
    DisplayLineState *line;

    if (line_index >= DISPLAY_TEXT_LINE_COUNT) {
        return;
    }

    if (fill_percent > 100U) {
        fill_percent = 100U;
    }

    line = &display_lines[line_index];

    if ((line->kind == DISPLAY_LINE_KIND_BAR) && (line->fill_percent == fill_percent)) {
        return;
    }

    line->kind         = DISPLAY_LINE_KIND_BAR;
    line->fill_percent = fill_percent;
    line->needs_redraw = true;
}

bool DisplayService_Render(void)
{
    uint8_t line_index;

    if (!display_is_available) {
        return false;
    }

    for (line_index = 0U; line_index < DISPLAY_TEXT_LINE_COUNT; line_index++) {
        DisplayLineState *line = &display_lines[line_index];

        if (!line->needs_redraw) {
            continue;
        }

        if (line->kind == DISPLAY_LINE_KIND_BAR) {
            Ssd1306_DrawHorizontalBar(line_index, line->fill_percent);
        }
        else {
            Ssd1306_ClearPage(line_index);
            Ssd1306_DrawText(0U, line_index, line->text);
        }

        line->needs_redraw = false;
    }

    return Ssd1306_FlushChangedPages();
}
