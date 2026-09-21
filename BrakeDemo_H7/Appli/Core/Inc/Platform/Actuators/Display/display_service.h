/*
 * display_service.h
 *
 * Layer 4: device-independent text surface. Presents the panel as 8 text
 * lines of 21 characters, or bar graphs. Knows nothing about I2C, nothing
 * about SSD1306 opcodes, and nothing about which signal goes on which line.
 *
 * All numeric formatting is integer arithmetic. No printf, no floating
 * point anywhere in this module.
 */

#ifndef PLATFORM_DISPLAY_DISPLAY_SERVICE_H
#define PLATFORM_DISPLAY_DISPLAY_SERVICE_H

#include <stdint.h>
#include <stdbool.h>

#define DISPLAY_TEXT_LINE_COUNT        8U
#define DISPLAY_CHARACTERS_PER_LINE   21U   /* 128 px / 6 px per character */

/* Brings up the bus and the panel. Returns false if the panel did not
 * acknowledge, in which case every later call is a harmless no-op. */
bool DisplayService_Initialize(void);

/* True once the panel answered at startup. Lets callers keep using the
 * UART trace when no display is attached. */
bool DisplayService_IsAvailable(void);

void DisplayService_ClearAllLines(void);

/* Plain text. Longer strings are truncated at DISPLAY_CHARACTERS_PER_LINE. */
void DisplayService_SetLineText(uint8_t line_index, const char *text);

/*
 * Scaled-integer value, rendered as "<label> <whole>.<fraction> <unit>".
 *
 *   scaled_value    value multiplied by scaling_factor (e.g. slip x1000)
 *   scaling_factor  1, 10, 100 or 1000 - decides the number of decimals
 *   unit_text       may be NULL
 *
 * Example: label "SLIP L", scaled_value 123, scaling_factor 1000, unit "%"
 *          renders as  "SLIP L 0.123 %"
 */
void DisplayService_SetLineFixedPointValue(uint8_t line_index,
                                           const char *label,
                                           int32_t scaled_value,
                                           uint16_t scaling_factor,
                                           const char *unit_text);

/* Two integers on one line: "<label> <left_label><left> <right_label><right>" */
void DisplayService_SetLineTwoIntegers(uint8_t line_index,
                                       const char *label,
                                       const char *left_tag,
                                       int32_t left_value,
                                       const char *right_tag,
                                       int32_t right_value);

/* Full-width outlined bar graph occupying the whole line. */
void DisplayService_SetLineBar(uint8_t line_index, uint8_t fill_percent);

/* Redraws only the lines that changed, then pushes only the dirty pages. */
bool DisplayService_Render(void);

#endif /* PLATFORM_DISPLAY_DISPLAY_SERVICE_H */
