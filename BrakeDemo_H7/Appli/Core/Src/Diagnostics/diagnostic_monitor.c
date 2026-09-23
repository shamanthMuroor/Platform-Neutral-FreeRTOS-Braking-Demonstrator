#include "diag_monitor.h"
#include "platform_io.h"
#include <stdint.h>
#include <stdio.h>

#define REF_SPEED_MAX				 800
#define DIAG_CONFIRM_CHECK_COUNT     3U

#define DTC_REF_SPEED_TOO_HIGH		 0x67U

static uint8_t consecutive_failure_count = 0U;
static uint8_t ref_speed_fault_active = 0U;

void Diag_CheckReferenceSpeed(void) {
    const int32_t ref_speed = Platform_ReadSignal(SIGNAL_REFERENCE_SPEED);

    if (ref_speed > REF_SPEED_MAX) {
        consecutive_failure_count++;

        if ((consecutive_failure_count >= DIAG_CONFIRM_CHECK_COUNT) && (ref_speed_fault_active == 0U)) {
            ref_speed_fault_active = 1U;
            printf("[DIAGNOSTIC] FAULT code=0x%04X reference_speed=%ld maximum=%d fial_count:%d\r\n", DTC_REF_SPEED_TOO_HIGH, (long)ref_speed, REF_SPEED_MAX, consecutive_failure_count);
        }

        // ONLY FOR TESTING, prints every 5th failed count
        if ((consecutive_failure_count%5)==0) {
            printf("[DIAGNOSTIC] FAULT code=0x%04X reference_speed=%ld maximum=%d fail_count:%d\r\n", DTC_REF_SPEED_TOO_HIGH, (long)ref_speed, REF_SPEED_MAX, consecutive_failure_count);
        }

    } else {
        consecutive_failure_count = 0U;

        if (ref_speed_fault_active != 0U) {
            ref_speed_fault_active = 0U;
            printf("[DIAGNOSTIC] RECOVERED code=0x%04X reference_speed=%ld\r\n", DTC_REF_SPEED_TOO_HIGH, (long)ref_speed);
        }
    }
}
