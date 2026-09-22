/*
 * i2c_bus.h
 *
 * Layer 1: the only file in the display stack that knows about the STM32 HAL.
 * Everything above it talks in terms of "write these bytes to this device".
 *
 * The OLED is wired to I2C2 (PF0 = I2C2_SDA, PF1 = I2C2_SCL), because I2C1
 * could not be allocated a full pin pair on this board (PB7 is held by the
 * BSP as LD3).
 */

#ifndef PLATFORM_DISPLAY_I2C_BUS_H
#define PLATFORM_DISPLAY_I2C_BUS_H

#include <stdint.h>
#include <stdbool.h>

/* HAL uses 8-bit addressing, so the SSD1306 7-bit address 0x3C becomes 0x78. */
#define I2C_BUS_DEFAULT_TIMEOUT_MILLISECONDS   100U

/* Binds this module to the CubeMX-generated handle. Call once at startup,
 * after MX_I2C2_Init() has run. */
void I2cBus_Initialize(void);

/* Blocking write. Returns false on NACK, timeout or bus error. */
bool I2cBus_WriteBytes(uint8_t device_address_8bit,
                       const uint8_t *data_bytes,
                       uint16_t byte_count,
                       uint32_t timeout_milliseconds);

/* Plausibility check: does anything acknowledge at this address?
 * Useful once at startup to tell "display not wired" from "display broken". */
bool I2cBus_IsDevicePresent(uint8_t device_address_8bit);

#endif /* PLATFORM_DISPLAY_I2C_BUS_H */
