#include "../../mcc_generated_files/system.h"
#include "../../mcc_generated_files/pin_manager.h"
#include "../../mcc_generated_files/tmr1.h"
#include "../../mcc_generated_files/memory/flash.h"
#include "main.h"
#include "bootloader.h"
#include "../02-FLAH_ROUTINES/flash_routines.h"
#include "../03-TARGET/target.h"
#include "../04-CRC/crc.h"

volatile uint32_t BootRequest __attribute__((address(0x1080), persistent));
const char __attribute__((address(0x4080), space(prog))) text[64] = "Ceci est un test";

void WriteLogisticPage(void);

void main(void)
{
    uint8_t BootState = eBootStandbyState;
    uint32_t AppliPresent = readAppFlag();
    UARTmsg_t Rx_Msg;
    
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
        if (FrameAvailable(&Rx_Msg) == 1) /* On Rx frame */
        {
            switch(Rx_Msg.ID)
            {
            case eService_echo:
                serviceEcho(&Rx_Msg);
                break;
                
            case eService_getInfo:
                serviceGetInfo(&Rx_Msg);
                break;
                
            case eService_eraseFlash:
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

void WriteLogisticPage(void)
{
    uint32_t WriteBuffze32[BOOT_ROW_SIZE_WORD];
    uint16_t i;
    uint8_t result;
    
    FLASH_Unlock(FLASH_UNLOCK_KEY);
    
    for (i= 0; i < BOOT_ROW_SIZE_WORD; i++)
    {
        WriteBuffze32[i] = 0x00AACCBB;
    }
    
    result = FLASH_WriteRow24(ADDR_FLASH_LOGISTIC, WriteBuffze32);
    FLASH_Lock();
}