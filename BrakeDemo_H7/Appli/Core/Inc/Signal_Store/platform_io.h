#ifndef PLATFORM_IO_H
#define PLATFORM_IO_H

#include "platform_signals.h"

#ifdef __cplusplus
extern "C" {
#endif

void Platform_InitSignals(void);
int Platform_ReadSignal(SignalId signalId);
void Platform_UpdateSignal(SignalId signalId, int value);

#ifdef __cplusplus
}
#endif
#endif
