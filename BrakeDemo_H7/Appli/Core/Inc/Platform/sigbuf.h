#ifndef SIGNAL_BUFFER_H
#define SIGNAL_BUFFER_H

#include "platform_signals.h"
#include <stdint.h>


void SignalBuffer_Init (int32_t initial_value);
int32_t  SignalBuffer_Read (SignalId signalId);
void SignalBuffer_Write(SignalId signalId, int32_t value);

#endif
