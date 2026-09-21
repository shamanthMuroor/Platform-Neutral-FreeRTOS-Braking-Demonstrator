/*
 * ssd1306_oled_driver.c
 *
 * Layer 2 implementation. Command values come from the SSD1306 controller
 * datasheet (command table, addressing-mode section, and the 0x00 / 0x40
 * control-byte convention for the I2C interface).
 */

#include "ssd1306_oled_driver.h"
#include "ssd1306_font_6x8.h"
#include "i2c_bus.h"

#include <string.h>

/* ---- Control bytes prefixed to every I2C transfer ---------------------- */
#define SSD1306_CONTROL_BYTE_COMMAND_STREAM   0x00U
#define SSD1306_CONTROL_BYTE_DATA_STREAM      0x40U

/* ---- Command opcodes --------------------------------------------------- */
#define SSD1306_COMMAND_SET_MEMORY_MODE       0x20U
#define SSD1306_COMMAND_SET_COLUMN_ADDRESS    0x21U
#define SSD1306_COMMAND_SET_PAGE_ADDRESS      0x22U
#define SSD1306_COMMAND_SET_START_LINE        0x40U
#define SSD1306_COMMAND_SET_CONTRAST          0x81U
#define SSD1306_COMMAND_SET_CHARGE_PUMP       0x8DU
#define SSD1306_COMMAND_SET_SEGMENT_REMAP     0xA1U
#define SSD1306_COMMAND_DISPLAY_FROM_RAM      0xA4U
#define SSD1306_COMMAND_DISPLAY_NORMAL        0xA6U
#define SSD1306_COMMAND_SET_MULTIPLEX_RATIO   0xA8U
#define SSD1306_COMMAND_DISPLAY_OFF           0xAEU
#define SSD1306_COMMAND_DISPLAY_ON            0xAFU
#define SSD1306_COMMAND_SET_COM_SCAN_REVERSE  0xC8U
#define SSD1306_COMMAND_SET_DISPLAY_OFFSET    0xD3U
#define SSD1306_COMMAND_SET_CLOCK_DIVIDE      0xD5U
#define SSD1306_COMMAND_SET_PRECHARGE_PERIOD  0xD9U
#define SSD1306_COMMAND_SET_COM_PIN_CONFIG    0xDAU
#define SSD1306_COMMAND_SET_VCOM_DESELECT     0xDBU

#define SSD1306_MEMORY_MODE_HORIZONTAL        0x00U

/* One control byte plus one full page of pixel data. */
#define SSD1306_PAGE_TRANSFER_SIZE_BYTES  (1U + SSD1306_DISPLAY_WIDTH_PIXELS)

static uint8_t frame_buffer[SSD1306_PAGE_COUNT][SSD1306_DISPLAY_WIDTH_PIXELS];
static bool    page_has_changed[SSD1306_PAGE_COUNT];
static uint8_t page_transfer_buffer[SSD1306_PAGE_TRANSFER_SIZE_BYTES];

/* ----------------------------------------------------------------------- */
/* Command helpers                                                         */
/* ----------------------------------------------------------------------- */

static bool Ssd1306_SendCommandSequence(const uint8_t *command_bytes, uint16_t command_count)
{
    uint8_t  transfer_buffer[16];
    uint16_t index;

    if (command_count > (uint16_t)(sizeof(transfer_buffer) - 1U))
    {
        return false;
    }

    transfer_buffer[0] = SSD1306_CONTROL_BYTE_COMMAND_STREAM;
    for (index = 0U; index < command_count; index++)
    {
        transfer_buffer[index + 1U] = command_bytes[index];
    }

    return I2cBus_WriteBytes(SSD1306_DEVICE_ADDRESS_8BIT,
                             transfer_buffer,
                             (uint16_t)(command_count + 1U),
                             I2C_BUS_DEFAULT_TIMEOUT_MILLISECONDS);
}

static bool Ssd1306_SendSingleCommand(uint8_t command_byte)
{
    return Ssd1306_SendCommandSequence(&command_byte, 1U);
}

/* ----------------------------------------------------------------------- */
/* Initialisation                                                          */
/* ----------------------------------------------------------------------- */

bool Ssd1306_Initialize(void)
{
    /* Standard 128x64 power-on sequence. Charge pump must be enabled before
     * the display is switched on, otherwise the panel stays dark. */
    static const uint8_t initialisation_sequence[] =
    {
        SSD1306_COMMAND_DISPLAY_OFF,
        SSD1306_COMMAND_SET_CLOCK_DIVIDE,      0x80U,
        SSD1306_COMMAND_SET_MULTIPLEX_RATIO,   (SSD1306_DISPLAY_HEIGHT_PIXELS - 1U),
        SSD1306_COMMAND_SET_DISPLAY_OFFSET,    0x00U,
        SSD1306_COMMAND_SET_START_LINE,
        SSD1306_COMMAND_SET_CHARGE_PUMP,       0x14U,   /* internal charge pump on */
        SSD1306_COMMAND_SET_MEMORY_MODE,       SSD1306_MEMORY_MODE_HORIZONTAL,
        SSD1306_COMMAND_SET_SEGMENT_REMAP,              /* left-right  orientation */
        SSD1306_COMMAND_SET_COM_SCAN_REVERSE,           /* top-bottom  orientation */
        SSD1306_COMMAND_SET_COM_PIN_CONFIG,    0x12U,   /* alternate, 64-row panel */
        SSD1306_COMMAND_SET_CONTRAST,          0xCFU,
        SSD1306_COMMAND_SET_PRECHARGE_PERIOD,  0xF1U,
        SSD1306_COMMAND_SET_VCOM_DESELECT,     0x40U,
        SSD1306_COMMAND_DISPLAY_FROM_RAM,
        SSD1306_COMMAND_DISPLAY_NORMAL,
        SSD1306_COMMAND_DISPLAY_ON
    };

    uint16_t index;

    if (!I2cBus_IsDevicePresent(SSD1306_DEVICE_ADDRESS_8BIT))
    {
        return false;
    }

    /* Sent one opcode at a time: simple, and initialisation speed is
     * irrelevant compared to the clarity of a per-command failure point. */
    for (index = 0U; index < (uint16_t)sizeof(initialisation_sequence); index++)
    {
        if (!Ssd1306_SendSingleCommand(initialisation_sequence[index]))
        {
            return false;
        }
    }

    Ssd1306_ClearFrameBuffer();
    return Ssd1306_FlushChangedPages();
}

/* ----------------------------------------------------------------------- */
/* Frame buffer drawing                                                    */
/* ----------------------------------------------------------------------- */

void Ssd1306_ClearFrameBuffer(void)
{
    memset(frame_buffer, 0, sizeof(frame_buffer));
    Ssd1306_MarkAllPagesChanged();
}

void Ssd1306_ClearPage(uint8_t page_index)
{
    if (page_index >= SSD1306_PAGE_COUNT)
    {
        return;
    }

    memset(frame_buffer[page_index], 0, SSD1306_DISPLAY_WIDTH_PIXELS);
    page_has_changed[page_index] = true;
}

void Ssd1306_MarkAllPagesChanged(void)
{
    uint8_t page_index;

    for (page_index = 0U; page_index < SSD1306_PAGE_COUNT; page_index++)
    {
        page_has_changed[page_index] = true;
    }
}

void Ssd1306_SetPixel(uint8_t x_position, uint8_t y_position, bool pixel_on)
{
    uint8_t page_index;
    uint8_t bit_mask;

    if ((x_position >= SSD1306_DISPLAY_WIDTH_PIXELS) ||
        (y_position >= SSD1306_DISPLAY_HEIGHT_PIXELS))
    {
        return;
    }

    page_index = (uint8_t)(y_position / SSD1306_PAGE_HEIGHT_PIXELS);
    bit_mask   = (uint8_t)(1U << (y_position % SSD1306_PAGE_HEIGHT_PIXELS));

    if (pixel_on)
    {
        frame_buffer[page_index][x_position] |= bit_mask;
    }
    else
    {
        frame_buffer[page_index][x_position] &= (uint8_t)(~bit_mask);
    }

    page_has_changed[page_index] = true;
}

void Ssd1306_DrawCharacter(uint8_t x_position, uint8_t page_index, char character)
{
    uint8_t glyph_index;
    uint8_t column_index;

    if (page_index >= SSD1306_PAGE_COUNT)
    {
        return;
    }

    /* Anything outside the printable range is drawn as a space. */
    if (((uint8_t)character < SSD1306_FONT_FIRST_CODE_POINT) ||
        ((uint8_t)character > SSD1306_FONT_LAST_CODE_POINT))
    {
        character = ' ';
    }

    glyph_index = (uint8_t)((uint8_t)character - SSD1306_FONT_FIRST_CODE_POINT);

    for (column_index = 0U; column_index < SSD1306_FONT_CELL_WIDTH_PIXELS; column_index++)
    {
        uint8_t target_column = (uint8_t)(x_position + column_index);

        if (target_column >= SSD1306_DISPLAY_WIDTH_PIXELS)
        {
            break;   /* clip at the right edge instead of wrapping */
        }

        frame_buffer[page_index][target_column] =
            Ssd1306Font6x8_Glyphs[glyph_index][column_index];
    }

    page_has_changed[page_index] = true;
}

void Ssd1306_DrawText(uint8_t x_position, uint8_t page_index, const char *text)
{
    uint8_t cursor_x = x_position;

    if (text == NULL)
    {
        return;
    }

    while ((*text != '\0') && (cursor_x < SSD1306_DISPLAY_WIDTH_PIXELS))
    {
        Ssd1306_DrawCharacter(cursor_x, page_index, *text);
        cursor_x = (uint8_t)(cursor_x + SSD1306_FONT_CELL_WIDTH_PIXELS);
        text++;
    }
}

void Ssd1306_DrawHorizontalBar(uint8_t page_index, uint8_t fill_percent)
{
    /* Outline uses rows 0 and 6 of the page, fill uses rows 2..4, leaving
     * row 7 blank as separation from the line below. */
    const uint8_t outline_top_and_bottom_mask = (uint8_t)((1U << 0) | (1U << 6));
    const uint8_t fill_mask                   = (uint8_t)((1U << 2) | (1U << 3) | (1U << 4));
    const uint8_t left_right_edge_mask        = 0x7FU;   /* rows 0..6 */

    uint8_t  column_index;
    uint32_t filled_width_pixels;

    if (page_index >= SSD1306_PAGE_COUNT)
    {
        return;
    }

    if (fill_percent > 100U)
    {
        fill_percent = 100U;
    }

    Ssd1306_ClearPage(page_index);

    /* Integer arithmetic only: interior is 126 px wide (columns 1..126). */
    filled_width_pixels = ((uint32_t)fill_percent * 126U) / 100U;

    for (column_index = 0U; column_index < SSD1306_DISPLAY_WIDTH_PIXELS; column_index++)
    {
        uint8_t column_value;

        if ((column_index == 0U) || (column_index == (SSD1306_DISPLAY_WIDTH_PIXELS - 1U)))
        {
            column_value = left_right_edge_mask;
        }
        else
        {
            column_value = outline_top_and_bottom_mask;

            if ((uint32_t)column_index <= filled_width_pixels)
            {
                column_value |= fill_mask;
            }
        }

        frame_buffer[page_index][column_index] = column_value;
    }

    page_has_changed[page_index] = true;
}

/* ----------------------------------------------------------------------- */
/* Transfer                                                                */
/* ----------------------------------------------------------------------- */

static bool Ssd1306_FlushSinglePage(uint8_t page_index)
{
    const uint8_t address_window_commands[] =
    {
        SSD1306_COMMAND_SET_COLUMN_ADDRESS, 0x00U, (uint8_t)(SSD1306_DISPLAY_WIDTH_PIXELS - 1U),
        SSD1306_COMMAND_SET_PAGE_ADDRESS,   page_index, page_index
    };

    if (!Ssd1306_SendCommandSequence(address_window_commands,
                                     (uint16_t)sizeof(address_window_commands)))
    {
        return false;
    }

    page_transfer_buffer[0] = SSD1306_CONTROL_BYTE_DATA_STREAM;
    memcpy(&page_transfer_buffer[1], frame_buffer[page_index], SSD1306_DISPLAY_WIDTH_PIXELS);

    return I2cBus_WriteBytes(SSD1306_DEVICE_ADDRESS_8BIT,
                             page_transfer_buffer,
                             SSD1306_PAGE_TRANSFER_SIZE_BYTES,
                             I2C_BUS_DEFAULT_TIMEOUT_MILLISECONDS);
}

bool Ssd1306_FlushChangedPages(void)
{
    uint8_t page_index;
    bool    all_transfers_succeeded = true;

    for (page_index = 0U; page_index < SSD1306_PAGE_COUNT; page_index++)
    {
        if (!page_has_changed[page_index])
        {
            continue;
        }

        if (Ssd1306_FlushSinglePage(page_index))
        {
            page_has_changed[page_index] = false;
        }
        else
        {
            /* Leave the page marked dirty so the next cycle retries it. */
            all_transfers_succeeded = false;
        }
    }

    return all_transfers_succeeded;
}
