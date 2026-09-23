#ifndef DIAGNOSTICS_DIAG_HANDLER_H
#define DIAGNOSTICS_DIAG_HANDLER_H

#include "diag_event.h"

void DiagHandler_HandleEvent(DiagEventId event_id, const DiagEvent *event);

#endif
