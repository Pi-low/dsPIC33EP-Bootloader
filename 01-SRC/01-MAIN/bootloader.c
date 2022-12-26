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
static uint32_t pu32SaveRstVector[2];

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
    uint8_t u8TmpBuff[65];
    uint16_t u16SwVersion;
    uint32_t u32ApplicationPresentFlag;
    
    FptsGenMsg->u8ID += 0x80;
    u8TmpBuff[0] = eRetVal;
    
    switch (FptsGenMsg->pu8Data[0])
    {
    case 0: /* Get application present flag */
        u32ApplicationPresentFlag = (FLASH_ReadWord16(ADDR_APPL_FLAG) & 0xFFFF)| 
                FLASH_ReadWord16(ADDR_APPL_FLAG + 2);
        
        u8TmpBuff[1] = u32ApplicationPresentFlag >> 24;
        u8TmpBuff[2] = u32ApplicationPresentFlag >> 16;
        u8TmpBuff[3] = u32ApplicationPresentFlag >> 8;
        u8TmpBuff[4] = u32ApplicationPresentFlag;
        FptsGenMsg->u16Length = 4;
        break;

    case 1: /* Get application version */
        u16SwVersion = FLASH_ReadWord16(ADDR_APPL_VERSION);
        u8TmpBuff[1] = u16SwVersion >> 8;
        u8TmpBuff[2] = u16SwVersion;
        FptsGenMsg->u16Length = 2;
        break;

    case 2: /* Get application logistic ascii string */
        FlashReadBufferU8(&u8TmpBuff[1], FLASH_LOGISTIC_CHAR_SIZE, ADDR_APPL_DESC);
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
    
    pu32SaveRstVector[0] = FLASH_ReadWord24(0);
    pu32SaveRstVector[1] = FLASH_ReadWord24(2);
    
    FLASH_Unlock(FLASH_UNLOCK_KEY);
    
    u8Err = FLASH_ErasePage(0);
    u8Err &= FLASH_WriteDoubleWord24(0, pu32SaveRstVector[0], pu32SaveRstVector[1]);

    while ((u32i < FLASH_APPLI_PAGES) && (u8Err == 1)) /* break loop in case of error */
    {
        /* Page start address % 0x400 */
        u8Err &= FLASH_ErasePage(ADDR_FLASH_APPLI + (u32i * FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS));
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
    
    FptsGenMsg->u8ID += 0x80;
    FptsGenMsg->u16Length = 1;
    FptsGenMsg->pu8Data[0] = eRetVal;
    
    sendFrame(FptsGenMsg);
    
    return eRetVal;
}

teOperationRetVal serviceDataTransfer(tsGenericMsg * FptsGenMsg)
{
    teOperationRetVal eRetVal = eOperationSuccess;
    
    if (u8BlankFlashFlag != 0)
    {
        eRetVal = eOperationFail;
    }
    
    if(eRetVal == eOperationSuccess)
    {
        eRetVal = createDataBlock(FptsGenMsg, &tsDataBlock);
        FLASH_Unlock(FLASH_UNLOCK_KEY);
    }

    if (eRetVal == eOperationSuccess)
    {
        if (FLASH_WriteRow24(tsDataBlock.u32BlockAddr, tsDataBlock.pu32Word) != true)
        {
            eRetVal = eOperationFail;
        }
        FLASH_Lock();
    }
    
    if (eRetVal == eOperationSuccess)
    {
        eRetVal = FlashCheckRow(&tsDataBlock);
    }
    
    FptsGenMsg->u8ID += 0x80;
    FptsGenMsg->u16Length = 1;
    FptsGenMsg->pu8Data[0] = eRetVal;
    
    sendFrame(FptsGenMsg);
    
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
    uint8_t u8DataCnt = 0;
    uint16_t u16Tmp;
    uint16_t u16CRC = 0;
    uint32_t u32RowAddress = 0;
    uint32_t u32Offset = 0;
    
    FptsBlock->u32BlockAddr = (uint32_t)FptsGenMsg->pu8Data[3] & 0xFF; /* ADDR24 low */
    FptsBlock->u32BlockAddr |= ((uint32_t)FptsGenMsg->pu8Data[2] << 8) & 0xFF00; /* ADDR24 mid */
    FptsBlock->u32BlockAddr |= ((uint32_t)FptsGenMsg->pu8Data[1] << 16) & 0xFF0000; /* ADDR24 high */
    FptsBlock->u32BlockAddr >>= 1; /* Virtual addressing transform */
    
    u16Tmp = FptsGenMsg->u16Length - 6;
    
    FptsBlock->u16BlockSize8 = u16Tmp;
    FptsBlock->u16CRC = FptsGenMsg->pu8Data[FptsGenMsg->u16Length - 1] & 0xFF; /* CRC16 low */
    FptsBlock->u16CRC |= (FptsGenMsg->pu8Data[FptsGenMsg->u16Length - 2] << 8) & 0xFF00; /* CRC16 high */
    
    if ((FptsBlock->u32BlockAddr % 2) == 0 && /* Address aligned to program counter */
            (((FptsBlock->u32BlockAddr >= ADDR_FLASH_APPLI) && (FptsBlock->u32BlockAddr < ADDR_FLASH_END)) || /* Application area */
            (FptsBlock->u32BlockAddr < ADDR_FLASH_BOOT))) /* IVT area + application logistic data */
    {
        u32Offset = FptsBlock->u32BlockAddr % BOOT_ROW_OFFSET_ADDR;
        u32RowAddress = FptsBlock->u32BlockAddr - u32Offset;
        u32Offset <<= 1; /* Transform address offset to byte count (to be 0xff)*/

        /* Fill 0xFF till' block start address */
        for (u16Tmp = 0; u16Tmp < BOOT_ROW_SIZE_BYTE; u16Tmp++)
        {
            if ((u16Tmp >= u32Offset) && (u8DataCnt < FptsBlock->u16BlockSize8))
            {
                FptsBlock->pu8Data[u16Tmp] = FptsGenMsg->pu8Data[u8DataCnt + 3]; /* Escape block address data into frame */
                u8DataCnt++;
            }
            else
            {
                FptsBlock->pu8Data[u16Tmp] = 0xFF;
            }
        }
    }
    else
    {
        /* Start address is not aligned */
        eRetVal = eBadBlockAddr;
    }
    
    if (eRetVal == eOperationSuccess)
    {
        BufUpdateCrc16(&u16CRC, (uint8_t*)(FptsGenMsg->pu8Data + 3), FptsBlock->u16BlockSize8);
        if (u16CRC != FptsBlock->u16CRC)
        {
            eRetVal = eBadCRCBlock;
        }
        else
        {
            u16Tmp = CharToWordBuffer(FptsBlock->pu32Word, FptsBlock->pu8Data, FptsBlock->u16BlockSize8);
            if (FptsBlock->u32BlockAddr == 0) /* Restor reset vector */
            {
                FptsBlock->pu32Word[0] = pu32SaveRstVector[0];
                FptsBlock->pu32Word[1] = pu32SaveRstVector[1];
            }
            if (u16Tmp == 0)
            {
                eRetVal = eOperationFail;
            }
        }
    }
    
    return eRetVal;
}

#ifndef _IS_REALEASE
teOperationRetVal serviceCRC(tsGenericMsg* FptsGenMsg)
{
    teOperationRetVal eRetVal = eOperationSuccess;
    uint16_t u16CRC = 0;
    BufUpdateCrc16(&u16CRC, FptsGenMsg->pu8Data, FptsGenMsg->u16Length);
    FptsGenMsg->u8ID += 0x80;
    FptsGenMsg->pu8Data[0] = eRetVal;
    FptsGenMsg->pu8Data[1] = u16CRC >> 8;
    FptsGenMsg->pu8Data[2] = u16CRC;
    FptsGenMsg->u16Length = 3;
    
    sendFrame(FptsGenMsg);
    return eRetVal;
}
#endif