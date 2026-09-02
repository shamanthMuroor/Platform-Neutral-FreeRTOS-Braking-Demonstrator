#include "signal_store.h"

//#include "FreeRTOS.h"
//#include "task.h"
#include <string.h>

/* ---- storage methods ---- */
void SignalStore_Init(SignalStore *signalStore) {
	if (signalStore == NULL) {
		return;
	}
    memset(signalStore->value, 0, sizeof(signalStore->value));
}

int SignalStore_Read(SignalStore *signalStore, SignalId signalId) {
    int value = 0;
    if (signalStore == NULL) {
    	return 0;
    }
    if ((signalId < 0) || (signalId >= SIGNAL_COUNT)) {
    	return 0;
    }

//    taskENTER_CRITICAL();
    value = signalStore->value[signalId];
//    taskEXIT_CRITICAL();
    return value;
}

void SignalStore_Write(SignalStore *signalStore, SignalId signalId, int value) {
    if (signalStore == NULL) {
    	return;
    }
    if ((signalId < 0) || (signalId >= SIGNAL_COUNT)) {
    	return;
    }
//    taskENTER_CRITICAL();
	signalStore->value[signalId] = value;
//    taskEXIT_CRITICAL();
}
