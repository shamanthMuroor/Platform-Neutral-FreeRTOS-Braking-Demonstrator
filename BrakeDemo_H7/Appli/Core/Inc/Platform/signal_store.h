#ifndef SIGNAL_STORE_H
#define SIGNAL_STORE_H

#include "platform_signals.h"

typedef struct {
    int value[SIGNAL_COUNT];
} SignalStore;

void SignalStore_Init (SignalStore *signalStore);
int  SignalStore_Read (SignalStore *signalStore, SignalId signalId);
void SignalStore_Write(SignalStore *signalStore, SignalId signalId, int value);

#endif
