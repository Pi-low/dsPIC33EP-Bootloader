#include "../../mcc_generated_files/system.h"
#include "../../mcc_generated_files/pin_manager.h"
#include "../../mcc_generated_files/tmr1.h"
#include "../../mcc_generated_files/memory/flash.h"
#include "../02-FLAH_ROUTINES/flash_routines.h"
#include "../03-TARGET/target.h"
#include "../05-BOOTLOADER/BootloaderTypes.h"
#include "../05-BOOTLOADER/bootloader.h"
#include "Misc.h"

volatile uint32_t BootRequest __attribute__((address(0x1080), persistent));

#ifndef _IS_RELEASE
const char __attribute__((address(ADDR_APPL_DESC), space(prog))) text[FLASH_LOGISTIC_CHAR_SIZE] = __DATE__" "__TIME__" : Bootloader standalone";
const uint16_t SWVersion __attribute__((address(ADDR_APPL_VERSION), space(prog))) = 0xB101;
#endif

static tsGenericMsg tsMainMsg;

void main(void)
{
    teMainStates teCurrentState = eStateTransition;
    
    uint32_t AppliPresent = (uint32_t)FLASH_ReadWord16(ADDR_APPL_FLAG)| 
            ((uint32_t)FLASH_ReadWord16(ADDR_APPL_FLAG + 2) << 16);
    
    if (AppliPresent == APPLIVALID) /* Application is present */
    {
        if (BootRequest != 0xC0DEFEED)
        {
            StartApplication();
        }
    }
    
    SYSTEM_Initialize();
    MCU_FPWM_SetHigh();
    InitBackTask();
    TMR1_Start();
    resetBootState();
    while(1)
    {
        TMR1_Tasks_16BitOperation(); /* SW timer management */
        ManageBackTask(); /* UART frame management */
    
        switch(teCurrentState)
        {
        case eStateTransition:
            
            break;
            
        case eStateIdle:
            teCurrentState = State_BootIdle();
            break;
            
        case eStateFlash:
            teCurrentState = State_Bootloading();
            break;
            
        default:
            break;
        }
    }
}