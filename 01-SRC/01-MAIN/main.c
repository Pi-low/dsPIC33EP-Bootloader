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
const uint32_t AppliFlag __attribute__((address(ADDR_APPL_FLAG), space(prog))) = 0xAABBCCDD;
const char __attribute__((address(ADDR_APPL_DESC), space(prog))) text[128] = "Ceci est un test";
const uint16_t SWVersion __attribute__((address(ADDR_APPL_VERSION), space(prog))) = 0x0201;
#endif

static tsGenericMsg tsMainMsg;

void main(void)
{
    teOperationRetVal eRetVal;
    uint32_t AppliPresent = ((uint32_t)FLASH_ReadWord16(ADDR_APPL_FLAG) & 0xFFFF)| (uint32_t)FLASH_ReadWord16(ADDR_APPL_FLAG + 2);
    
    if (AppliPresent == APPLIVALID) /* Application is present */
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
                eRetVal = serviceDataTransfer(&tsMainMsg);
                break;
                
            case eService_checkFlash:
                break;
                
            case eService_writePin:
                break;
                
            case eService_readPin:
                break;
                
#ifndef _IS_RELEASE
            case eService_TestCrc:
                eRetVal = serviceCRC(&tsMainMsg);
                break;
#endif
                
            default:
                break;
            }
        }
    }
}