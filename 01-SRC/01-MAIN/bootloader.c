#include "../../mcc_generated_files/system.h"
#include "../../mcc_generated_files/pin_manager.h"
#include "../../mcc_generated_files/tmr1.h"
#include "../../mcc_generated_files/memory/flash.h"
#include "../03-TARGET/target.h"
#include "../02-FLAH_ROUTINES/flash_routines.h"
#include "BootloaderTypes.h"
#include "bootloader.h"

static uint8_t u8BlankFlashFlag;

teOperationRetVal serviceEcho(tsGenericMsg* FptsGenMsg, tsGenericMsg* FptsRetMsg)
{
    uint8_t CharBuff[100];
    uint16_t u16i;
    
    for (u16i = 0; u16i < FptsGenMsg->u16Length; u16i++)
    {
        CharBuff[u16i] = FptsGenMsg->pu8Data[u16i];
    }
    
    return 0;
}

teOperationRetVal serviceGetInfo(tsGenericMsg * FptsGenMsg, tsGenericMsg* FptsRetMsg)
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

teOperationRetVal serviceEraseFlash(tsGenericMsg * FptsGenMsg, tsGenericMsg* FptsRetMsg)
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
            u8BlankFlashFlag = 1;
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

teOperationRetVal serviceDataTransfer(tsGenericMsg * FptsGenMsg, tsGenericMsg* FptsRetMsg)
{
    if (uartMsg->Length != 0)
    {
        
    }
    return 0;
}

teOperationRetVal serviceCheckFlash(tsGenericMsg * FptsGenMsg, tsGenericMsg* FptsRetMsg)
{
    return 0;
}

teOperationRetVal serviceWritePin(tsGenericMsg * FptsGenMsg, tsGenericMsg* FptsRetMsg)
{
    return 0;
}

teOperationRetVal serviceReadPin(tsGenericMsg * FptsGenMsg, tsGenericMsg* FptsRetMsg)
{
    uint8_t SID = eService_readPin;
    if (uartMsg->Length == 0)
    {
        
    }
    return 0;
}

teOperationRetVal createDataBlock(tsGenericMsg * FptsGenMsg, DataBlock_t * Row)
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
