#ifndef DIAGNOSTICS_DIAG_PUBLISHER_H
#define DIAGNOSTICS_DIAG_PUBLISHER_H

#include "diag_event.h"

void DiagPublisher_Publish(DiagEventId event_id, const DiagEvent *event);

#endif
