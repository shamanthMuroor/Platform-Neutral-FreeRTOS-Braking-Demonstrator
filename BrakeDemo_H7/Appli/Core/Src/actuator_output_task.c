#include "actuator_output_task.h"
#include "platform_io.h"
#include "main.h"
#include "cmsis_os2.h"
#include <stdbool.h>
#include <stdio.h>

#define LED_GREEN_PORT  GPIOD
#define LED_GREEN_PIN   GPIO_PIN_10
#define LED_BLUE_PORT   GPIOD
#define LED_BLUE_PIN    GPIO_PIN_13
#define LED_RED_PORT    GPIOB
#define LED_RED_PIN     GPIO_PIN_7

void ActuatorOutputTask() {

    for (;;) {
        int32_t left_motor_command  = Platform_ReadSignal(SIGNAL_FRONT_LEFT_MOTOR_COMMAND);
        int32_t right_motor_command  = Platform_ReadSignal(SIGNAL_FRONT_RIGHT_MOTOR_COMMAND);
        int32_t requested_brake     = Platform_ReadSignal(SIGNAL_REQUESTED_BRAKE);
        int32_t fl_slip_active  = Platform_ReadSignal(SIGNAL_FRONT_LEFT_SLIP_ACTIVE);
        int32_t fr_slip_active  = Platform_ReadSignal(SIGNAL_FRONT_RIGHT_SLIP_ACTIVE);
        bool    slip_active = (fl_slip_active != 0) || (fr_slip_active != 0);

        printf("[ACTUATOR] left_motor_command: %lu\r\n", left_motor_command);
        printf("[ACTUATOR] right_motor_command: %lu\r\n", right_motor_command);

        HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, (left_motor_command > 0) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, (requested_brake > 0) ? GPIO_PIN_SET : GPIO_PIN_RESET);

        if (slip_active) {
            printf("[ACTUATOR] slip_active\r\n");
            printf("[ACTUATOR] RED RED RED RED\r\n");
            HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_SET);
        } else {
            HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_RESET);
        }

        osDelay(3000);
    }
}
