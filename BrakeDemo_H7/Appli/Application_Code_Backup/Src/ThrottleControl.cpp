#include "ThrottleControl.h"

volatile int Input_throttle = 0;
volatile int Output_motorCommand = 0;

#define THROTTLE_MIN 0
#define THROTTLE_MAX 1000

void Throttle_Runnable(void) {
    int requestedThrottle = Input_throttle;

    if (requestedThrottle > THROTTLE_MAX) requestedThrottle = THROTTLE_MAX;
    if (requestedThrottle < THROTTLE_MIN) requestedThrottle = THROTTLE_MIN;

    Output_motorCommand = requestedThrottle;
}
