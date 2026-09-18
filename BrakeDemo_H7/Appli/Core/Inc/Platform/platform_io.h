#ifndef PLATFORM_IO_H
#define PLATFORM_IO_H

#include "platform_signals.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void Platform_InitSignals(int32_t initial_value);
int32_t Platform_ReadSignal(SignalId signalId);
void Platform_WriteSignal(SignalId signalId, int32_t value);
void Application_RunControlCycle(void);

#ifdef __cplusplus
}
#endif
#endif
