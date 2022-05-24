#include "../../mcc_generated_files/system.h"
#include "../01-MAIN/main.h"
#include "../01-MAIN/bootloader.h"
#include "target.h"

static uint8_t UARTRxBuffer[262];

void MAnageIncomingBytes(void)
{
    if (UART1_IsRxReady())
    {
        
    }
}