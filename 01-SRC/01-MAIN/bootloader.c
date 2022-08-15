#include "../../mcc_generated_files/system.h"
#include "../../mcc_generated_files/pin_manager.h"
#include "../../mcc_generated_files/tmr1.h"
#include "../../mcc_generated_files/memory/flash.h"
#include "../03-TARGET/target.h"
#include "../02-FLAH_ROUTINES/flash_routines.h"
#include "main.h"
#include "bootloader.h"

static UARTmsg_t uartTxMsg;

uint8_t serviceEcho(UARTmsg_t * uartMsg)
{
    uint8_t CharBuff[100];
    uint16_t i;
    
    for (i = 0; i < uartMsg->Length; i++)
    {
        CharBuff[i] = uartMsg->Data[i];
    }
    constructFrame(eService_echo, CharBuff, uartMsg->Length, &uartTxMsg);
    sendFrame(&uartTxMsg);
    
    return 0;
}

uint8_t serviceGetInfo(UARTmsg_t * uartMsg)
{
    uint8_t LogisticChar[64];
    readLogisticChar(LogisticChar);
    constructFrame(eService_getInfo, LogisticChar, 64, &uartTxMsg);
    sendFrame(&uartTxMsg);
    return 0;
}

uint8_t serviceEraseFlash(UARTmsg_t * uartMsg)
{
    return 0;
}

uint8_t serviceDataTransfer(UARTmsg_t * uartMsg)
{
    return 0;
}

uint8_t serviceCheckFlash(UARTmsg_t * uartMsg)
{
    return 0;
}

uint8_t serviceWritePin(UARTmsg_t * uartMsg)
{
    return 0;
}

uint8_t serviceReadPin(UARTmsg_t * uartMsg)
{
    return 0;
}