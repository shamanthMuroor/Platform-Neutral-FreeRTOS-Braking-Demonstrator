#include "sigbuf.h"
#include <string.h>

typedef struct {
    volatile int32_t value[2];
    volatile uint8_t active_index;
} SignalBuffer;

static SignalBuffer signal_buffers[SIGNAL_COUNT];

void SignalBuffer_Init(int32_t initial_value) {
	uint32_t signal_index;

	for(signal_index=0U; signal_index < (uint32_t)SIGNAL_COUNT; signal_index++) {
		signal_buffers[signal_index].value[0] 		= initial_value;
		signal_buffers[signal_index].value[1] 		= initial_value;
		signal_buffers[signal_index].active_index 	= 00;
	}
}

void SignalBuffer_Write(SignalId signal_id, int32_t value) {
	uint8_t active_index;
	uint8_t write_index;

	if (signal_id >= SIGNAL_COUNT) {
		return;
	}

//	enter critical section
	active_index = signal_buffers[signal_id].active_index;
	write_index = (uint8_t)(active_index ^ 1u);

	signal_buffers[signal_id].value[write_index] = value;
	signal_buffers[signal_id].active_index = write_index;
//	exit critical section
}


int32_t SignalBuffer_Read(SignalId signal_id) {
	uint8_t active_index;
	int32_t value;

	if (signal_id >= SIGNAL_COUNT) {
		return -1;
	}

//	enter critical section
	active_index = signal_buffers[signal_id].active_index;
	value = signal_buffers[signal_id].value[active_index];
//	exit critical section
	return value;
}
