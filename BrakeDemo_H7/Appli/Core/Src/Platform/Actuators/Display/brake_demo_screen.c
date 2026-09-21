/*
 * Screen layout (8 lines x 21 characters, line N is SSD1306 page N):
 *
 *   0   REF SPD 320
 *   1   BRK 40 %
 *   2   [==========        ]
 *   3   SPD L280 R315
 *   4   SLIP L 0.125 %
 *   5   SLIP R -0.016 %
 *   6   CMD L250 R240
 *   7   ABS L:ON  R:OFF
 */

#include "brake_demo_screen.h"
#include "display_service.h"

#include "platform_io.h"

#include <stdint.h>
#include <stddef.h>

#define BRAKE_DEMO_SLIP_SCALING_FACTOR   1000U

// Line assignment
#define BRAKE_DEMO_LINE_REFERENCE_SPEED   0U
#define BRAKE_DEMO_LINE_BRAKE_VALUE       1U
#define BRAKE_DEMO_LINE_BRAKE_BAR         2U
#define BRAKE_DEMO_LINE_WHEEL_SPEEDS      3U
#define BRAKE_DEMO_LINE_LEFT_SLIP         4U
#define BRAKE_DEMO_LINE_RIGHT_SLIP        5U
#define BRAKE_DEMO_LINE_MOTOR_COMMANDS    6U
#define BRAKE_DEMO_LINE_SLIP_ACTIVE       7U

static uint8_t BrakeDemoScreen_ClampToPercent(int32_t value) {
    if (value < 0) {
        return 0U;
    }

    if (value > 100) {
        return 100U;
    }

    return (uint8_t)value;
}

static const char *BrakeDemoScreen_SlipActiveText(int32_t left_slip_active, int32_t right_slip_active) {
    if ((left_slip_active != 0) && (right_slip_active != 0)) {
        return "ABS L:ON  R:ON";
    }

    if (left_slip_active != 0) {
        return "ABS L:ON  R:OFF";
    }

    if (right_slip_active != 0) {
        return "ABS L:OFF R:ON";
    }

    return "ABS L:OFF R:OFF";
}

bool BrakeDemoScreen_Initialize(void) {
    bool display_available = DisplayService_Initialize();

    if (display_available) {
        DisplayService_ClearAllLines();
        DisplayService_SetLineText(BRAKE_DEMO_LINE_REFERENCE_SPEED, "BRAKE DEMO STARTING");
        (void)DisplayService_Render();
    }

    return display_available;
}

void BrakeDemoScreen_Update(void)
{
    if (!DisplayService_IsAvailable())
    {
        return;
    }

    /* One consistent snapshot, taken before any formatting happens. */
    const int32_t reference_speed      = Platform_ReadSignal(SIGNAL_REFERENCE_SPEED);
    const int32_t requested_brake      = Platform_ReadSignal(SIGNAL_REQUESTED_BRAKE);
    const int32_t left_wheel_speed     = Platform_ReadSignal(SIGNAL_FRONT_LEFT_WHEEL_SPEED);
    const int32_t right_wheel_speed    = Platform_ReadSignal(SIGNAL_FRONT_RIGHT_WHEEL_SPEED);
    const int32_t left_wheel_slip      = Platform_ReadSignal(SIGNAL_FRONT_LEFT_WHEEL_SLIP);
    const int32_t right_wheel_slip     = Platform_ReadSignal(SIGNAL_FRONT_RIGHT_WHEEL_SLIP);
    const int32_t left_motor_command   = Platform_ReadSignal(SIGNAL_FRONT_LEFT_MOTOR_COMMAND);
    const int32_t right_motor_command  = Platform_ReadSignal(SIGNAL_FRONT_RIGHT_MOTOR_COMMAND);
    const int32_t left_slip_active     = Platform_ReadSignal(SIGNAL_FRONT_LEFT_SLIP_ACTIVE);
    const int32_t right_slip_active    = Platform_ReadSignal(SIGNAL_FRONT_RIGHT_SLIP_ACTIVE);

    /* Line 0: the speed the controller is regulating against. */
    DisplayService_SetLineFixedPointValue(BRAKE_DEMO_LINE_REFERENCE_SPEED, "REF SPEED", reference_speed, 1U, NULL);

    /* Lines 1 and 2: requested brake as a number and as a bar. */
    DisplayService_SetLineFixedPointValue(BRAKE_DEMO_LINE_BRAKE_VALUE, "BRAKE", requested_brake, 1U, "%");

    DisplayService_SetLineBar(BRAKE_DEMO_LINE_BRAKE_BAR, BrakeDemoScreen_ClampToPercent(requested_brake));

    /* Line 3: both wheel speeds side by side, so the select-low behaviour
     * is visible as the two values diverge. */
    DisplayService_SetLineTwoIntegers(BRAKE_DEMO_LINE_WHEEL_SPEEDS, "SPEED",  "L", left_wheel_speed, "R", right_wheel_speed);

    /* Lines 4 and 5: slip, carried as integer x1000 and rendered with three
     * decimals. No floating point involved at any point. */
    DisplayService_SetLineFixedPointValue(BRAKE_DEMO_LINE_LEFT_SLIP, "SLIP L", left_wheel_slip, BRAKE_DEMO_SLIP_SCALING_FACTOR, "%");

    DisplayService_SetLineFixedPointValue(BRAKE_DEMO_LINE_RIGHT_SLIP, "SLIP R", right_wheel_slip, BRAKE_DEMO_SLIP_SCALING_FACTOR, "%");

    /* Line 6: what actually went to the motors after select-low arbitration. */
    DisplayService_SetLineTwoIntegers(BRAKE_DEMO_LINE_MOTOR_COMMANDS, "MOTOR", "L", left_motor_command, "R", right_motor_command);

    // Line 7: slip-active flags
    DisplayService_SetLineText(BRAKE_DEMO_LINE_SLIP_ACTIVE, BrakeDemoScreen_SlipActiveText(left_slip_active, right_slip_active));

    (void)DisplayService_Render();
}
