/*
 * brake_demo_screen.h
 *
 * Layer 5: screen composer. Decides which braking-demo signal appears on
 * which line. Knows nothing about I2C, SSD1306 opcodes or pixels.
 *
 * This is the only file in the display stack that touches the braking
 * signals, and it reaches them exclusively through Platform_ReadSignal()
 * from platform_io.h. It never touches the Simulink model object directly.
 */

#ifndef PLATFORM_DISPLAY_BRAKE_DEMO_SCREEN_H
#define PLATFORM_DISPLAY_BRAKE_DEMO_SCREEN_H

#include <stdbool.h>

/* Brings up the display stack and paints the startup line.
 * Returns false if no panel acknowledged on the bus. */
bool BrakeDemoScreen_Initialize(void);

/* Takes one consistent snapshot of the signals, formats the eight lines
 * and pushes whatever changed. Call at a fixed period from the display
 * task, not from the control task. */
void BrakeDemoScreen_Update(void);

#endif /* PLATFORM_DISPLAY_BRAKE_DEMO_SCREEN_H */
