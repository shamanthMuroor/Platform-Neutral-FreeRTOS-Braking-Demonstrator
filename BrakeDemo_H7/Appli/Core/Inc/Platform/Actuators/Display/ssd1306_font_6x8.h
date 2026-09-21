/*
 * ssd1306_font_6x8.h
 *
 * Layer 3: bitmap font data only. Knows nothing about I2C, the SSD1306
 * controller, or the application.
 */

#ifndef PLATFORM_DISPLAY_SSD1306_FONT_6X8_H
#define PLATFORM_DISPLAY_SSD1306_FONT_6X8_H

#include <stdint.h>

#define SSD1306_FONT_CELL_WIDTH_PIXELS    6U   /* 5 glyph columns + 1 spacing column */
#define SSD1306_FONT_CELL_HEIGHT_PIXELS   8U   /* exactly one SSD1306 page          */
#define SSD1306_FONT_FIRST_CODE_POINT  0x20U
#define SSD1306_FONT_LAST_CODE_POINT   0x7FU
#define SSD1306_FONT_GLYPH_COUNT         96U

/*
 * Column-major glyph bitmaps. Ssd1306Font6x8_Glyphs[g][c] is pixel column c
 * of glyph g, where bit 0 is the top pixel row and bit 7 the bottom row.
 * Index with (code_point - SSD1306_FONT_FIRST_CODE_POINT).
 */
extern const uint8_t Ssd1306Font6x8_Glyphs[SSD1306_FONT_GLYPH_COUNT][SSD1306_FONT_CELL_WIDTH_PIXELS];

#endif /* PLATFORM_DISPLAY_SSD1306_FONT_6X8_H */
