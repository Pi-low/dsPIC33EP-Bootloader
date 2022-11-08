#include "../../mcc_generated_files/system.h"
#include "../../mcc_generated_files/pin_manager.h"
#include "../../mcc_generated_files/tmr1.h"
#include "../../mcc_generated_files/memory/flash.h"
#include "../03-TARGET/target.h"
#include "../02-FLAH_ROUTINES/flash_routines.h"
#include "BootloaderTypes.h"
#include "bootloader.h"

static UARTmsg_t uartTxMsg;
static uint8_t BlankFlashFlag;
uint32_t u32Buffer[64];

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
    uint8_t Logistics[64];
    uint8_t DataLen, SID;
    uint16_t SwVersion;
    uint32_t ApplicationPresentFlag;
    
    SID = eService_getInfo;
    
    if (uartMsg->Length == 1)
    {
        switch (uartMsg->Data[0])
        {
        case 0: /* Get application present flag */
            ApplicationPresentFlag = readAppFlag();
            Logistics[0] = ApplicationPresentFlag >> 24;
            Logistics[1] = ApplicationPresentFlag >> 16;
            Logistics[2] = ApplicationPresentFlag >> 8;
            Logistics[3] = ApplicationPresentFlag;
            DataLen = 4;
            break;

        case 1: /* Get application version */
            SwVersion = readSWVersion();
            Logistics[0] = SwVersion >> 8;
            Logistics[1] = SwVersion;
            DataLen = 2;
            break;

        case 2: /* Get application logistic ascii string */
            readLogisticChar(Logistics);
            DataLen = 64;
            break;

        default:
            SID |= eMaskError_UnknownParameter;
            Logistics[0] = 0xFF;
            DataLen = 1;
            break;
        }
    }
    else
    {
        SID |= eMaskError_IncorrectFrameLength;
        Logistics[0] = 0xFF;
        DataLen = 1;
    }
    constructFrame(SID, Logistics, DataLen, &uartTxMsg);
    sendFrame(&uartTxMsg);
    return 0;
}

uint8_t serviceEraseFlash(UARTmsg_t * uartMsg)
{
    uint32_t i = 0;
    uint8_t Err = 1, SID = eService_eraseFlash;
    uint8_t Buff[2];
    if (uartMsg->Length == 0)
    {
        FLASH_Unlock(FLASH_UNLOCK_KEY);
        
        while ((i < FLASH_APPLI_PAGES) && (Err != 1)) /* break loop in case of error */
        {
            /* Page start address % 0x400 */
            Err &= FLASH_ErasePage(ADDR_FLASH_LOGISTIC + (i * FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS));
            i++;
        }
        if ((Err == 1) && (i == FLASH_APPLI_PAGES))
        {
            BlankFlashFlag = 1;
        }
        else
        {
            /* Do nothing */
        }
        Buff[0] = Err;
        
        FLASH_Lock();
    }
    else
    {
        SID |= eMaskError_IncorrectFrameLength;
        Buff[0] = 0xFF;
    }
    constructFrame(SID, Buff, 1, &uartTxMsg);
    sendFrame(&uartTxMsg);
    return 0;
}

uint8_t serviceDataTransfer(UARTmsg_t * uartMsg)
{
    if (uartMsg->Length != 0)
    {
        
    }
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
    uint8_t SID = eService_readPin;
    if (uartMsg->Length == 0)
    {
        
    }
    return 0;
}

uint8_t createDataBlock(UARTmsg_t * MSG, DataBlock_t * Row)
{
    uint16_t u16Tmp, u16i;
    uint8_t u8Temp, u8RetVal = 1;
    
    if (MSG->Length > 7)
    {
        Row->blockAddress = MSG->Data[3] & 0xFF; /* ADDR24 low */
        Row->blockAddress |= (MSG->Data[2] << 8) & 0xFF00; /* ADDR24 mid */
        Row->blockAddress |= (MSG->Data[1] << 16) & 0xFF0000; /* ADDR24 high */
        Row->blockAddress >>= 1;
  
        Row->u32WordArray = u32Buffer;
        
        Row->blockSize = u16Tmp - 6;
        Row->blockCRC = MSG->Data[u16Tmp - 1] & 0xFF; /* CRC16 low */
        Row->blockCRC |= (MSG->Data[u16Tmp - 2] << 8) & 0xFF00; /* CRC16 high */
        
        if((Row->blockAddress % 2) == 0)
        {
            /* Correct block address */
        }
        else
        {
            /* Start address is not aligned */
            u8RetVal = 0;
        }
        u16Tmp = MSG->Length;
        
    }
    return u8Temp;
}
