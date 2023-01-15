#include <xc.h>
#include <stdint.h>
#include "../../mcc_generated_files/tmr1.h"

void BlockingDelay(uint16_t u16msDelay)
{
    uint16_t u16Timeout = u16msDelay + TMR1_SoftwareCounterGet();
    while (u16Timeout > TMR1_SoftwareCounterGet())
    {
        TMR1_Tasks_16BitOperation();
        ClrWdt();
    };
}

void WatchdogEnable(void)
{
    RCONbits.SWDTEN = 1;
}
