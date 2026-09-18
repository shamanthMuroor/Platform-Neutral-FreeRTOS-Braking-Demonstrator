//#include "platform_actuator.h"
//#include "signal_buffer.h"
//#include <stdbool.h>
//
//
///* --- Match these to the user labels in your .ioc --- */
//#define LED_GREEN_PORT  GPIOD
//#define LED_GREEN_PIN   GPIO_PIN_10
//#define LED_BLUE_PORT   GPIOD
//#define LED_BLUE_PIN    GPIO_PIN_13
//#define LED_RED_PORT    GPIOB
//#define LED_RED_PIN     GPIO_PIN_7
///* --------------------------------------------------- */
//
//#define RED_LED_BLINK_DIVIDER  (5U)   /* 10 ms control period -> ~10 Hz blink */
//
//static uint32_t red_led_blink_counter;
//static bool     red_led_state;
//
//void PlatformActuator_Init(void) {
//    red_led_blink_counter = 0U;
//    red_led_state         = false;
//
////    BSP_LED_Off(LED_BLUE);
////    BSP_LED_Off(LED_RED);
////    BSP_LED_Off(LED_GREEN);
//    HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(LED_BLUE_PORT,  LED_BLUE_PIN,  GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(LED_RED_PORT,   LED_RED_PIN,   GPIO_PIN_RESET);
//}
//
//void PlatformActuator_Apply(SignalId signal_id, int32_t value) {
//    switch (signal_id) {
//
//    case SIGNAL_FRONT_LEFT_MOTOR_COMMAND:
//        /* Future: __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, pwm_duty); */
//    	HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, (value > 0) ? GPIO_PIN_SET : GPIO_PIN_RESET);
//
//    	/* Blue indicates the driver is braking while a command is applied. */
//    	HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, (SignalBuffer_Read(SIGNAL_REQUESTED_BRAKE) > 0) ? GPIO_PIN_SET : GPIO_PIN_RESET);
//    	break;
//
//    case SIGNAL_FRONT_RIGHT_MOTOR_COMMAND:
//        /* Future: __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, pwm_duty); */
//        break;
//
//    case SIGNAL_FRONT_LEFT_SLIP_ACTIVE:
//    case SIGNAL_FRONT_RIGHT_SLIP_ACTIVE:
//        if ((SignalBuffer_Read(SIGNAL_FRONT_LEFT_SLIP_ACTIVE)  != 0) ||
//            (SignalBuffer_Read(SIGNAL_FRONT_RIGHT_SLIP_ACTIVE) != 0)) {
//            red_led_blink_counter++;
//            if ((red_led_blink_counter % RED_LED_BLINK_DIVIDER) == 0U) {
//                red_led_state = !red_led_state;
//                HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, red_led_state ? GPIO_PIN_SET : GPIO_PIN_RESET);
//               }
//             } else {
//                red_led_state = false;
//                HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_RESET);
//             }
//        break;
//
//    default:
//        break;   /* slip values are informational: no actuator */
//    }
//}
