#include "diag_handler.h"
#include <stddef.h>
#include <stdio.h>

void DiagHandler_HandleEvent(DiagEventId event_id, const DiagEvent *event) {
	if (event == NULL) return;

	switch (event->event_type) {
		case FAULT_CONFIRMED:
			printf("[DIAGNOSTIC][FAULT][ACTIVE] eventId=%d, code=0x%04X value=%d limit_value=%d fial_count:%u\r\n",
					event_id,
					event->dtc_code,
					event->value,
					event->limit_value,
					event->failure_count);
			break;
		case FAULT_RECOVERED:
            printf("[DIAGNOSTIC][FAULT][RECOVERED] eventId=%d, code=0x%04X limit_value=%d\r\n", event_id, event->dtc_code, event->value);
            break;
		default:
			break;
	}
}
