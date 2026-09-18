#include "sigbuf.h"
#include "platform_io.h"

void Platform_InitSignals(int32_t initial_value) {
	SignalBuffer_Init(initial_value);
}

int32_t Platform_ReadSignal(SignalId signalId) {
    return SignalBuffer_Read(signalId);
}

void Platform_WriteSignal(SignalId signalId, int32_t value) {
    SignalBuffer_Write(signalId, value);
}
