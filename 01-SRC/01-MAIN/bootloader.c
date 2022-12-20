#include <stdlib.h>
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
    teOperationRetVal eRetVal = eOperationSuccess;
    tsGenericMsg tsRetMsg;
    uint16_t u16i;
    
    for (u16i = 0; u16i < FptsGenMsg->u16Length; u16i++)
    {
        tsRetMsg->pu8Data[u16i] = FptsGenMsg->pu8Data[u16i];
    }
    tsRetMsg.u16Length = FptsGenMsg->u16Length;
    
    FptsRetMsg = &tsRetMsg;
    
    return eRetVal;
}

teOperationRetVal serviceGetInfo(tsGenericMsg * FptsGenMsg, tsGenericMsg* FptsRetMsg)
{
    teOperationRetVal eRetVal = eOperationSuccess;
    tsGenericMsg tsRetMsg;
    uint16_t u16SwVersion;
    uint32_t u32ApplicationPresentFlag;
    FptsRetMsg = &tsRetMsg;
    switch (FptsGenMsg->pu8Data[0])
    {
    case 0: /* Get application present flag */
        u32ApplicationPresentFlag = readAppFlag();
        tsRetMsg.pu8Data[0] = u32ApplicationPresentFlag >> 24;
        tsRetMsg.pu8Data[1] = u32ApplicationPresentFlag >> 16;
        tsRetMsg.pu8Data[2] = u32ApplicationPresentFlag >> 8;
        tsRetMsg.pu8Data[3] = u32ApplicationPresentFlag;
        tsRetMsg.u16Length = 4;
        break;

    case 1: /* Get application version */
        u16SwVersion = readSWVersion();
        tsRetMsg.pu8Data[0] = u16SwVersion >> 8;
        tsRetMsg.pu8Data[1] = u16SwVersion;
        tsRetMsg.u16Length = 2;
        break;

    case 2: /* Get application logistic ascii string */
        readLogisticChar(tsRetMsg.pu8Data);
        tsRetMsg.u16Length = 64;
        break;

    default:
        eRetVal = eOperationFail;
        FptsRetMsg = NULL;
        break;
    }
    
    return eRetVal;
}

teOperationRetVal serviceEraseFlash(tsGenericMsg * FptsGenMsg, tsGenericMsg* FptsRetMsg)
{
    teOperationRetVal eRetVal = eOperationSuccess;
    uint32_t u32i = 0;
    uint8_t u8Err = 1;
    

    FLASH_Unlock(FLASH_UNLOCK_KEY);

    while ((u32i < FLASH_APPLI_PAGES) && (u8Err != 1)) /* break loop in case of error */
    {
        /* Page start address % 0x400 */
        u8Err &= FLASH_ErasePage(ADDR_FLASH_LOGISTIC + (u32i * FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS));
        u32i++;
    }
    if ((u8Err == 1) && (u32i == FLASH_APPLI_PAGES))
    {
        u8BlankFlashFlag = 1;
    }
    else
    {
        eRetVal = eFlashEraseError;
    }

    FLASH_Lock();
    
    return eRetVal;
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
