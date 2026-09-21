/*
 * ssd1306_oled_driver.h
 *
 * Layer 2: SSD1306 controller driver. Knows the command set, the page
 * memory layout and owns the frame buffer. Knows nothing about I2C
 * register access (delegated to i2c_bus) and nothing about the braking
 * application.
 *
 * Frame buffer layout mirrors the controller: 8 pages of 128 bytes.
 * Page p, column x, bit b  ->  screen pixel (x, p * 8 + b).
 *
 * Only pages whose content actually changed are pushed over the bus,
 * which keeps a typical refresh well under 3 ms instead of the ~23 ms a
 * full 1024-byte frame costs at 400 kHz.
 */

#ifndef PLATFORM_DISPLAY_SSD1306_OLED_DRIVER_H
#define PLATFORM_DISPLAY_SSD1306_OLED_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

#define SSD1306_DEVICE_ADDRESS_8BIT      0x78U   /* 7-bit 0x3C shifted left */
#define SSD1306_DISPLAY_WIDTH_PIXELS      128U
#define SSD1306_DISPLAY_HEIGHT_PIXELS      64U
#define SSD1306_PAGE_COUNT                  8U
#define SSD1306_PAGE_HEIGHT_PIXELS          8U

/* Powers up the panel and clears it. Returns false if the controller does
 * not acknowledge, i.e. wiring or address problem. */
bool Ssd1306_Initialize(void);

/* Frame buffer operations. None of these touch the bus. */
void Ssd1306_ClearFrameBuffer(void);
void Ssd1306_ClearPage(uint8_t page_index);
void Ssd1306_SetPixel(uint8_t x_position, uint8_t y_position, bool pixel_on);
void Ssd1306_DrawCharacter(uint8_t x_position, uint8_t page_index, char character);
void Ssd1306_DrawText(uint8_t x_position, uint8_t page_index, const char *text);

/* Outlined bar occupying one full page, filled from the left. */
void Ssd1306_DrawHorizontalBar(uint8_t page_index, uint8_t fill_percent);

/* Pushes every page marked dirty since the last flush. */
bool Ssd1306_FlushChangedPages(void);

/* Forces the whole frame buffer out on the next flush. */
void Ssd1306_MarkAllPagesChanged(void);

#endif /* PLATFORM_DISPLAY_SSD1306_OLED_DRIVER_H */
