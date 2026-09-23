#include "brake_demo_screen.h"
#include "cmsis_os2.h"

void DisplayOutputTask(void *argument)
{
    (void)argument;
    BrakeDemoScreen_Initialize();
    for (;;)
    {
        BrakeDemoScreen_Update();
        osDelay(2000);
    }
}
