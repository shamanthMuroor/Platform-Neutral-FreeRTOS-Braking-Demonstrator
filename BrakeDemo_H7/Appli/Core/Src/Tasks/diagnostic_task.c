#include "diag_monitor.h"
#include "cmsis_os2.h"

void DiagnosticTask(void *argument)
{
    (void)argument;
    for (;;)
    {
    	Diag_CheckReferenceSpeed();
        osDelay(2000);
    }
}
