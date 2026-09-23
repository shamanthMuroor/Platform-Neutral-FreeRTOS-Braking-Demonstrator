#include "diag_monitor.h"
#include "cmsis_os2.h"
#include <stdio.h>

void DiagnosticTask(void *argument)
{
    (void)argument;
    for (;;)
    {
    	Diag_CheckReferenceSpeed();

    	// ONLY FOR TESTING
//    	if (Diag_IsReferenceSpeedFaultActive()) {
//    		printf("YEAAAAAAAAA");
//    	}
        osDelay(2000);
    }
}
