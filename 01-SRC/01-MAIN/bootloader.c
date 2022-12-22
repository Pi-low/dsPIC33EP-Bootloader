#include "../../mcc_generated_files/system.h"
#include "../../mcc_generated_files/pin_manager.h"
#include "../../mcc_generated_files/tmr1.h"
#include "../../mcc_generated_files/memory/flash.h"
#include "../03-TARGET/target.h"
#include "../02-FLAH_ROUTINES/flash_routines.h"
#include "../04-CRC/crc.h"
#include "BootloaderTypes.h"
#include "bootloader.h"

static uint8_t u8BlankFlashFlag;
static DataBlock_t tsDataBlock;

teOperationRetVal serviceEcho(tsGenericMsg* FptsGenMsg)
{
    teOperationRetVal eRetVal = eOperationSuccess;
    uint8_t u8TmpBuff[32];
    uint8_t u8i;
    
    u8TmpBuff[0] = eOperationSuccess;
    FptsGenMsg->u8ID += 0x80;
    
    for (u8i = 0; u8i < FptsGenMsg->u16Length; u8i++)
    {
        u8TmpBuff[u8i + 1] = FptsGenMsg->pu8Data[u8i];
    }
    
    FptsGenMsg->u16Length ++;
    
    BufCopy(FptsGenMsg->pu8Data, u8TmpBuff, FptsGenMsg->u16Length);
    
    sendFrame(FptsGenMsg);
    
    return eRetVal;
}

teOperationRetVal serviceGetInfo(tsGenericMsg * FptsGenMsg)
{
    teOperationRetVal eRetVal = eOperationSuccess;
    uint8_t u8TmpBuff[64];
    uint16_t u16SwVersion;
    uint32_t u32ApplicationPresentFlag;
    
    FptsGenMsg->u8ID += 0x80;
    
    switch (FptsGenMsg->pu8Data[0])
    {
    case 0: /* Get application present flag */
        u32ApplicationPresentFlag = readAppFlag();
        u8TmpBuff[1] = u32ApplicationPresentFlag >> 24;
        u8TmpBuff[2] = u32ApplicationPresentFlag >> 16;
        u8TmpBuff[3] = u32ApplicationPresentFlag >> 8;
        u8TmpBuff[4] = u32ApplicationPresentFlag;
        FptsGenMsg->u16Length = 4;
        break;

    case 1: /* Get application version */
        u16SwVersion = readSWVersion();
        u8TmpBuff[1] = u16SwVersion >> 8;
        u8TmpBuff[2] = u16SwVersion;
        FptsGenMsg->u16Length = 2;
        break;

    case 2: /* Get application logistic ascii string */
        readLogisticChar((uint8_t*)(u8TmpBuff+1));
        FptsGenMsg->u16Length = 64;
        break;

    default:
        eRetVal = eOperationFail;
        break;
    }
    
    if (eRetVal == eOperationSuccess)
    {
        FptsGenMsg->u16Length++;
        BufCopy(FptsGenMsg->pu8Data, u8TmpBuff, FptsGenMsg->u16Length);
        u8TmpBuff[0] = eRetVal;
    }
    else
    {
        FptsGenMsg->u16Length = 1;
        FptsGenMsg->pu8Data[0] = eRetVal;
    }
    
    sendFrame(FptsGenMsg);
    
    return eRetVal;
}

teOperationRetVal serviceEraseFlash(tsGenericMsg * FptsGenMsg)
{
    teOperationRetVal eRetVal = eOperationSuccess;
    uint32_t u32i = 0;
    uint8_t u8Err = 1;
    
    FptsGenMsg->u8ID += 0x80;
    FptsGenMsg->u16Length = 1;
    
    FLASH_Unlock(FLASH_UNLOCK_KEY);

    while ((u32i < FLASH_APPLI_PAGES) && (u8Err == 1)) /* break loop in case of error */
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
    
    FptsGenMsg->pu8Data[0] = eRetVal;
    
    sendFrame(FptsGenMsg);
    
    return eRetVal;
}

teOperationRetVal serviceDataTransfer(tsGenericMsg * FptsGenMsg)
{
    teOperationRetVal eRetVal = eOperationSuccess;
    
    eRetVal = createDataBlock(FptsGenMsg, &tsDataBlock);
    
    if (eRetVal == eOperationSuccess)
    {
        if (u8BlankFlashFlag != 0)
        {
            
        }
        else
        {
            eRetVal = eOperationFail;
        }
    }
    
    return eRetVal;
}

teOperationRetVal serviceCheckFlash(tsGenericMsg * FptsGenMsg)
{
    return eOperationSuccess;
}

teOperationRetVal serviceWritePin(tsGenericMsg * FptsGenMsg)
{
    return eOperationSuccess;
}

teOperationRetVal serviceReadPin(tsGenericMsg * FptsGenMsg)
{

    return eOperationSuccess;
}

teOperationRetVal createDataBlock(tsGenericMsg * FptsGenMsg, DataBlock_t * FptsBlock)
{
    teOperationRetVal eRetVal = eOperationSuccess;
    uint16_t u16Tmp;
    uint32_t u32RowAddress = 0;
    uint32_t u32Offset = 0;
    uint16_t u16CRC = 0;
    
    FptsBlock->u32BlockAddr = FptsGenMsg->pu8Data[3] & 0xFF; /* ADDR24 low */
    FptsBlock->u32BlockAddr |= (FptsGenMsg->pu8Data[2] << 8) & 0xFF00; /* ADDR24 mid */
    FptsBlock->u32BlockAddr |= (FptsGenMsg->pu8Data[1] << 16) & 0xFF0000; /* ADDR24 high */
    FptsBlock->u32BlockAddr >>= 1;
    
    u16Tmp = FptsGenMsg->u16Length - 6;
    
    FptsBlock->u16BlockSize8 = u16Tmp;
    FptsBlock->u16CRC = FptsGenMsg->pu8Data[FptsGenMsg->u16Length - 2] & 0xFF; /* CRC16 low */
    FptsBlock->u16CRC |= (FptsGenMsg->pu8Data[FptsGenMsg->u16Length - 3] << 8) & 0xFF00; /* CRC16 high */
    
    if ((FptsBlock->u32BlockAddr % 2) == 0 && /* Address aligned to program counter */
            (((FptsBlock->u32BlockAddr >= ADDR_FLASH_APPLI) && (FptsBlock->u32BlockAddr < ADDR_FLASH_END)) || /* Application area */
            (FptsBlock->u32BlockAddr == 0))) /* IVT area */
    {
        u32Offset = FptsBlock->u32BlockAddr % BOOT_ROW_OFFSET_ADDR;
        u32RowAddress = FptsBlock->u32BlockAddr - u32Offset;
        u32Offset >>= 1;
        
        if ((u32Offset + FptsBlock->u16BlockSize8) <= BOOT_ROW_SIZE_BYTE)
        {
            /* Correct block address */
            for (u16Tmp = 0; u16Tmp < u32Offset; u16Tmp++)
            {
                FptsBlock->pu8Data[u16Tmp] = 0xFF;
            }
            for (u16Tmp = 0; u16Tmp < FptsBlock->u16BlockSize8; u16Tmp++)
            {
                FptsBlock->pu8Data[u16Tmp + u32Offset] = FptsGenMsg->pu8Data[u16Tmp + 3];
            }
            FptsBlock->u32BlockAddr = u32RowAddress;
        }
        else
        {
            eRetVal = eOperationFail;
        }
    }
    else
    {
        /* Start address is not aligned */
        eRetVal = eBadBlockAddr;
    }
    
    if (eRetVal == eOperationSuccess)
    {
        initCRCengine(0x1021);
        u16CRC = RunCRC((uint8_t*)(FptsGenMsg->pu8Data + 3), FptsBlock->u16BlockSize8);
        if (u16CRC != FptsBlock->u16CRC)
        {
            eRetVal = eBadCRCBlock;
        }
    }
    
    return eRetVal;
}
