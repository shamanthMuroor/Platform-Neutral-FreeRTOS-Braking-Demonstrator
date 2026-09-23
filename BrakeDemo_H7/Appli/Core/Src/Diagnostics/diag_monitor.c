#include "platform_io.h"
#include "dtc_codes.h"
#include "diag_monitor.h"
#include "diag_event.h"
#include "diag_publisher.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define REF_SPEED_MAX			800
#define DIAG_FAULT_THRESHOLD    3U

static uint8_t ref_speed_failure_count = 0U;
static DiagFaultState ref_speed_fault_state = DIAG_FAULT_INACTIVE;


void Diag_ReportEvent(DiagEventId event_id, uint16_t dtc_code, DiagEventType event_type, int16_t value, int16_t limit_value, uint16_t failure_count) {
	const DiagEvent event = {
			.dtc_code = dtc_code,
			.event_type = event_type,
			.value = value,
			.limit_value = limit_value,
			.failure_count = failure_count
		};

	DiagPublisher_Publish(event_id, &event);
}

void Diag_CheckReferenceSpeed(void) {
    const int32_t ref_speed = Platform_ReadSignal(SIGNAL_REFERENCE_SPEED);

    if (ref_speed > REF_SPEED_MAX) {
        ref_speed_failure_count++;

        if (ref_speed_fault_state == DIAG_FAULT_INACTIVE) {
        	ref_speed_fault_state = DIAG_FAULT_PENDING;
        }

        if ((ref_speed_fault_state == DIAG_FAULT_PENDING) && (ref_speed_failure_count >= DIAG_FAULT_THRESHOLD)) {
        	ref_speed_fault_state = DIAG_FAULT_ACTIVE;
        	Diag_ReportEvent(EVENT_REFERENCE_SPEED_TOO_HIGH, DTC_REFERENCE_SPEED_TOO_HIGH, FAULT_CONFIRMED, (long)ref_speed, REF_SPEED_MAX, ref_speed_failure_count);
//            printf("[DIAGNOSTIC] FAULT code=0x%04X reference_speed=%ld maximum=%d fial_count:%u\r\n", DTC_REFERENCE_SPEED_TOO_HIGH, (long)ref_speed, REF_SPEED_MAX, ref_speed_failure_count);
        } else if (ref_speed_failure_count < DIAG_FAULT_THRESHOLD) {
        	printf("[DIAGNOSTIC][FAULT][PENDING] eventId=%d code=0x%04X reference_speed=%ld limit_value=%d fail_count:%u\r\n", EVENT_REFERENCE_SPEED_TOO_HIGH, DTC_REFERENCE_SPEED_TOO_HIGH, (long)ref_speed, REF_SPEED_MAX, ref_speed_failure_count);
        }

        // ONLY FOR TESTING, prints every 5th failed count
        if ((ref_speed_failure_count % 5U) == 0U) {
        	printf("[DIAGNOSTIC][FAULT][ACTIVE] eventId=%d code=0x%04X reference_speed=%ld limit_value=%d fail_count:%u\r\n", EVENT_REFERENCE_SPEED_TOO_HIGH, DTC_REFERENCE_SPEED_TOO_HIGH, (long)ref_speed, REF_SPEED_MAX, ref_speed_failure_count);
        }

    } else {
        ref_speed_failure_count = 0U;

        if (ref_speed_fault_state != DIAG_FAULT_INACTIVE) {
            ref_speed_fault_state = DIAG_FAULT_INACTIVE;
            Diag_ReportEvent(EVENT_REFERENCE_SPEED_TOO_HIGH, DTC_REFERENCE_SPEED_TOO_HIGH, FAULT_RECOVERED, (long)ref_speed, REF_SPEED_MAX, ref_speed_failure_count);
//            printf("[DIAGNOSTIC] RECOVERED code=0x%04X reference_speed=%ld\r\n", DTC_REFERENCE_SPEED_TOO_HIGH, (long)ref_speed);
        }
    }
}

//bool Diag_IsReferenceSpeedFaultActive() {
//	return ref_speed_fault_state;
//}
