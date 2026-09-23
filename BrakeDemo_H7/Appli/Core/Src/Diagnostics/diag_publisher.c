#include "diag_publisher.h"
#include "diag_handler.h"
#include <stddef.h>

void DiagPublisher_Publish(DiagEventId event_id, const DiagEvent *event) {
	if (event == NULL) return;

	// now we handle here, next we send to COM
	DiagHandler_HandleEvent(event_id, event);
}

