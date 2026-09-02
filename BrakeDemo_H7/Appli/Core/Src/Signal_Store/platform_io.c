#include "platform_io.h"
#include "signal_store.h"

// Shared Store
static SignalStore signalStore;

void Platform_InitSignals(void) {
	SignalStore_Init(&signalStore);
}

int Platform_ReadSignal(SignalId signalId) {
    return SignalStore_Read(&signalStore, signalId);
}

void Platform_UpdateSignal(SignalId signalId, int value) {
    SignalStore_Write(&signalStore, signalId, value);
}
