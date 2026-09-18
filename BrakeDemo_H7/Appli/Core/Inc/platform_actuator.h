#ifndef PLATFORM_ACTUATOR_H
#define PLATFORM_ACTUATOR_H

#include <stdint.h>
#include "platform_signals.h"

void PlatformActuator_Init(void);
void PlatformActuator_Apply(SignalId signal_id, int32_t value);

#endif /* PLATFORM_ACTUATOR_H */
