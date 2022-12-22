#include "../../mcc_generated_files/system.h"
#include "../../mcc_generated_files/pin_manager.h"
#include "../../mcc_generated_files/tmr1.h"
#include "../../mcc_generated_files/memory/flash.h"
#include "../02-FLAH_ROUTINES/flash_routines.h"
#include "../03-TARGET/target.h"
#include "BootloaderTypes.h"
#include "bootloader.h"

volatile uint32_t BootRequest __attribute__((address(0x1080), persistent));

#ifndef _IS_RELEASE
const uint32_t AppliFlag __attribute__((address(0x4000), space(prog))) = 0xAABBCCDD;
const char __attribute__((address(0x4080), space(prog))) text[64] = "Ceci est un test";
const uint16_t SWVersion __attribute__((address(0x40C0), space(prog))) = 0x0201;
#endif

static tsGenericMsg tsMainMsg;

void main(void)
{
    uint32_t AppliPresent = readAppFlag();
    teOperationRetVal eRetVal;
    
    if (BootRequest != BOOTFLAG && AppliPresent == APPLIVALID) /* Application is present, no bootmode requested */
    {
        StartApplication();
    }
    
    SYSTEM_Initialize();
    MCU_FPWM_SetHigh();
    InitBackTask();
    TMR1_Start();
    
    while(1)
    {
        TMR1_Tasks_16BitOperation(); /* SW timer management */
        ManageBackTask(); /* UART frame management */
        if (FrameAvailable(&tsMainMsg) == eOperationSuccess) /* On Rx frame */
        {
            switch(tsMainMsg.u8ID)
            {
            case eService_echo:
                eRetVal = serviceEcho(&tsMainMsg);
                break;
                
            case eService_getInfo:
                eRetVal = serviceGetInfo(&tsMainMsg);
                break;
                
            case eService_eraseFlash:
                eRetVal = serviceEraseFlash(&tsMainMsg);
                break;
                
            case eService_dataTransfer:
                break;
                
            case eService_checkFlash:
                break;
                
            case eService_writePin:
                break;
                
            case eService_readPin:
                break;
                
            default:
                break;
            }
        }
    }
}