#include "platform_io.h"
#include "cmsis_os2.h"
#include <stdbool.h>
#include <stdio.h>

#define APPLIED_BRAKE_VALUE              ((int32_t)50)
#define APPLIED_GRIP_LOSS_VALUE          ((int32_t)20)

void SimulatedInputTask() {
    uint32_t elapsed_cycles    = 0U;
    int32_t  reference_speed   = 250;

    Platform_WriteSignal(SIGNAL_REQUESTED_BRAKE,        0);
    Platform_WriteSignal(SIGNAL_FRONT_LEFT_GRIP_LOSS,   0);
    Platform_WriteSignal(SIGNAL_FRONT_RIGHT_GRIP_LOSS,  0);
    Platform_WriteSignal(SIGNAL_FRONT_LEFT_WHEEL_SPEED, 0);
    Platform_WriteSignal(SIGNAL_FRONT_RIGHT_WHEEL_SPEED, 0);

    printf("[H7-Platform] reference_speed: %ld\r\n", reference_speed);
    printf("[H7-Platform] brake: %lu\r\n", APPLIED_BRAKE_VALUE);
    for (;;) {
        printf("elapsed_cycles: %lu\r\n", elapsed_cycles);
        Platform_WriteSignal(SIGNAL_REFERENCE_SPEED, reference_speed);

        if (elapsed_cycles >= 3U) {
        	Platform_WriteSignal(SIGNAL_REQUESTED_BRAKE, APPLIED_BRAKE_VALUE);
        }

        if (elapsed_cycles >= 6U) {
        	Platform_WriteSignal(SIGNAL_FRONT_RIGHT_GRIP_LOSS, APPLIED_GRIP_LOSS_VALUE);
        }

        elapsed_cycles++;
        osDelay(2000);
    }
}
