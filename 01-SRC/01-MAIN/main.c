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

#ifndef _IS_RELEASE
const char __attribute__((address(0x4080), space(prog))) text[64] = "Ceci est un test";
const uint32_t AppliFlag __attribute__((address(0x4000), space(prog))) = 0xAABBCCDD;
const uint16_t SWVersion __attribute__((address(0x40C0), space(prog))) = 0x0201;
#endif

uint8_t RxMsgBuffer[MAX_FRM_LEN];

void main(void)
{
    uint32_t AppliPresent = readAppFlag();
    UARTmsg_t Rx_Msg;
    
    Rx_Msg.Data = RxMsgBuffer;
    
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
                serviceEraseFlash(&Rx_Msg);
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