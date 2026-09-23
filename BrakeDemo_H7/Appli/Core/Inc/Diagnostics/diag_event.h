#ifndef DIAGNOSTICS_DIAG_EVENT_H
#define DIAGNOSTICS_DIAG_EVENT_H

#include <stdint.h>

typedef enum
{
	EVENT_REFERENCE_SPEED_TOO_HIGH = 0,
	EVENT_COUNT
} DiagEventId;

typedef enum
{
    DIAG_FAULT_INACTIVE = 0,
    DIAG_FAULT_PENDING,
    DIAG_FAULT_ACTIVE
} DiagFaultState;

typedef enum
{
	FAULT_CONFIRMED = 0,
    FAULT_RECOVERED
} DiagEventType;

typedef struct
{
    uint16_t dtc_code;
    DiagEventType event_type;
    int16_t value;
    int16_t limit_value;
    uint16_t failure_count;
} DiagEvent;

#endif
