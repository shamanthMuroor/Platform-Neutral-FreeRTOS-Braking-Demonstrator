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
