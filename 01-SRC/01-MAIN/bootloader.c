#include "../../mcc_generated_files/system.h"
#include "../../mcc_generated_files/pin_manager.h"
#include "../../mcc_generated_files/tmr1.h"
#include "../../mcc_generated_files/memory/flash.h"
#include "../03-TARGET/target.h"
#include "../02-FLAH_ROUTINES/flash_routines.h"
#include "../04-CRC/crc.h"
#include "BootloaderTypes.h"
#include "bootloader.h"

static uint8_t u8BlankFlashFlag = 0;
static uint8_t u8BootloadingFlag = 0;
static uint16_t u16BootTimeout = 0;
static uint16_t u16TimeoutConst = 0;
static DataBlock_t tsDataBlock;
static uint32_t pu32SaveRstVector[2] = {0};
static uint32_t u32LastBlockAddr = 0;
static tsGenericMsg tsInternalMsg;

void resetBootState(void)
{
    u8BlankFlashFlag = 0;
    u8BootloadingFlag = 0;
    u16BootTimeout = 0;
    u16TimeoutConst = 0;
    u32LastBlockAddr = 0;
    pu32SaveRstVector[0] = 0;
    pu32SaveRstVector[1] = 0;
    tsInternalMsg.u8ID = 0xF0;
    tsInternalMsg.u16Length = 1;
    tsInternalMsg.pu8Data[0] = 0;
    sendFrame(&tsInternalMsg);
}

void updateTimeout(void)
{
    u16BootTimeout = TMR1_SoftwareCounterGet() + u16TimeoutConst;
}

void setBootSession(void)
{
    u8BootloadingFlag = 1;
    updateTimeout();
}

void manageTimeout(void)
{
    if ( (TMR1_SoftwareCounterGet() >= u16TimeoutConst) && (u8BootloadingFlag == 1))
    {
        resetBootState();
        tsInternalMsg.u8ID = 0xF0;
        tsInternalMsg.u16Length = 1;
        tsInternalMsg.pu8Data[0] = eBootSessionTimeout;
        sendFrame(&tsInternalMsg);
    }
}

teOperationRetVal serviceGoToBoot(tsGenericMsg* FptsGenMsg)
{
    teOperationRetVal eRetVal = eOperationSuccess;
    
    u16TimeoutConst = (((uint16_t)FptsGenMsg->pu8Data[0] << 8) & 0xFF00) | ((uint16_t)FptsGenMsg->pu8Data[1] & 0xFF);
    
    if (u16TimeoutConst >= 1000)
    {
        setBootSession();
    }
    else
    {
        eRetVal = eOperationNotAllowed;
    }
    
    tsInternalMsg.u8ID = FptsGenMsg->u8ID | 0x80;
    tsInternalMsg.pu8Data[0] = eRetVal;
    tsInternalMsg.u16Length = 1;
    
    sendFrame(&tsInternalMsg);
    
    return eRetVal;
}

teOperationRetVal serviceEcho(tsGenericMsg* FptsGenMsg)
{
    teOperationRetVal eRetVal = eOperationSuccess;
    uint8_t u8TmpBuff[32];
    uint8_t u8i;
    
    u8TmpBuff[0] = eOperationSuccess;
    tsInternalMsg.u8ID = FptsGenMsg->u8ID | 0x80;
    tsInternalMsg.u16Length = 1;
    
    for (u8i = 0; u8i < FptsGenMsg->u16Length; u8i++)
    {
        u8TmpBuff[u8i + 1] = FptsGenMsg->pu8Data[u8i];
        tsInternalMsg.u16Length++;
    }
    
    
    
    BufCopy(tsInternalMsg.pu8Data, u8TmpBuff, tsInternalMsg.u16Length);
    
    
    sendFrame(&tsInternalMsg);
    
    return eRetVal;
}

teOperationRetVal serviceGetInfo(tsGenericMsg * FptsGenMsg)
{
    teOperationRetVal eRetVal = eOperationSuccess;
    uint8_t u8TmpBuff[65];
    uint16_t u16SwVersion;
    uint32_t u32ApplicationPresentFlag;
    
    u8TmpBuff[0] = eRetVal;
    tsInternalMsg.u8ID = FptsGenMsg->u8ID | 0x80;
    tsInternalMsg.u16Length = 1;
    
    switch (FptsGenMsg->pu8Data[0])
    {
    case 0: /* Get application present flag */
        u32ApplicationPresentFlag = (uint32_t)FLASH_ReadWord16(ADDR_APPL_FLAG)| 
                ((uint32_t)FLASH_ReadWord16(ADDR_APPL_FLAG + 2) << 16);
        
        u8TmpBuff[1] = u32ApplicationPresentFlag >> 24;
        u8TmpBuff[2] = u32ApplicationPresentFlag >> 16;
        u8TmpBuff[3] = u32ApplicationPresentFlag >> 8;
        u8TmpBuff[4] = u32ApplicationPresentFlag;
        tsInternalMsg.u16Length += 4;
        break;

    case 1: /* Get application version */
        u16SwVersion = FLASH_ReadWord16(ADDR_APPL_VERSION);
        u8TmpBuff[1] = u16SwVersion >> 8;
        u8TmpBuff[2] = u16SwVersion;
        tsInternalMsg.u16Length += 2;
        break;

    case 2: /* Get application logistic ascii string */
        FlashReadBufferU8(&u8TmpBuff[1], FLASH_LOGISTIC_CHAR_SIZE, ADDR_APPL_DESC);
        tsInternalMsg.u16Length += 64;
        break;

    default:
        eRetVal = eOperationFail;
        break;
    }
    
    if (eRetVal == eOperationSuccess)
    {
        BufCopy(tsInternalMsg.pu8Data, u8TmpBuff, FptsGenMsg->u16Length);
    }
    else
    {
        tsInternalMsg.pu8Data[0] = eRetVal;
    }
    
    sendFrame(&tsInternalMsg);
    
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
    
    tsInternalMsg.u8ID = FptsGenMsg->u8ID | 0x80;
    tsInternalMsg.u16Length = 1;
    tsInternalMsg.pu8Data[0] = eRetVal;
    
    sendFrame(&tsInternalMsg);
    
    return eRetVal;
}

teOperationRetVal serviceDataTransfer(tsGenericMsg * FptsGenMsg)
{
    teOperationRetVal eRetVal = eOperationSuccess;
    
    if (u8BlankFlashFlag == 0)
    {
        eRetVal = eOperationFail;
    }
    
    if(eRetVal == eOperationSuccess)
    {
        eRetVal = manageDataBlock(FptsGenMsg, &tsDataBlock);
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
    
    tsInternalMsg.u8ID = FptsGenMsg->u8ID | 0x80;
    tsInternalMsg.u16Length = 1;
    tsInternalMsg.pu8Data[0] = eRetVal;
    sendFrame(&tsInternalMsg);
    
    return eRetVal;
}

teOperationRetVal serviceCheckFlash(tsGenericMsg * FptsGenMsg)
{
    teOperationRetVal eRetVal = eOperationSuccess;
    if (u8BlankFlashFlag == 0)
    {
        eRetVal = eOperationFail;
    }
    
    tsInternalMsg.u8ID = FptsGenMsg->u8ID | 0x80;
    tsInternalMsg.u16Length = 1;
    tsInternalMsg.pu8Data[0] = eRetVal;
    sendFrame(&tsInternalMsg);
    
    return eRetVal;
}

teOperationRetVal serviceWritePin(tsGenericMsg * FptsGenMsg)
{
    teOperationRetVal eRetVal = eOperationSuccess;
    return eRetVal;
}

teOperationRetVal serviceReadPin(tsGenericMsg * FptsGenMsg)
{

    teOperationRetVal eRetVal = eOperationSuccess;
    return eRetVal;
}

teOperationRetVal manageDataBlock(tsGenericMsg * FptsGenMsg, DataBlock_t * FptsBlock)
{
    teOperationRetVal eRetVal = eOperationSuccess;
    uint8_t u8DataCnt = 0;
    uint16_t u16Tmp = 0;
    uint16_t u16CRC = 0;
    uint32_t u32RowAddress = 0;
    uint32_t u32Offset = 0;
    
   
    /* STEP 1: CRC check */
    BufUpdateCrc16(&u16CRC, FptsGenMsg->pu8Data, FptsGenMsg->u16Length);
    if (u16CRC != 0xF0B8)
    {
        eRetVal = eBadCRCBlock;
    }
    
    /* STEP 2: Block address check */
    if (eRetVal == eOperationSuccess)
    {
        FptsBlock->u32BlockAddr = (uint32_t) FptsGenMsg->pu8Data[2] & 0xFF; /* ADDR24 low */
        FptsBlock->u32BlockAddr |= ((uint32_t) FptsGenMsg->pu8Data[1] << 8) & 0xFF00; /* ADDR24 mid */
        FptsBlock->u32BlockAddr |= ((uint32_t) FptsGenMsg->pu8Data[0] << 16) & 0xFF0000; /* ADDR24 high */
        FptsBlock->u32BlockAddr >>= 1; /* Virtual addressing transform */

        FptsBlock->u16BlockSize8 = FptsGenMsg->u16Length - 5; /* escape block address (3 bytes) + CRC16 (2 bytes) */;
        
        if (((FptsBlock->u32BlockAddr % 2) != 0) || /* Address aligned to program counter */
                ((FptsBlock->u32BlockAddr >= ADDR_FLASH_BOOT) && (FptsBlock->u32BlockAddr < ADDR_FLASH_APPLI)) || /* prevent from writing into boot section */
                (FptsBlock->u32BlockAddr >= ADDR_FLASH_END) ||  /* prevent from writing over application area */
                (u32LastBlockAddr > FptsBlock->u32BlockAddr)) /* Prevent from writing back */
        {
            eRetVal = eBadBlockAddr;
        }
    }
    
    /* STEP 3: Generate flash row from received data */
    if (eRetVal == eOperationSuccess)
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
        u32LastBlockAddr = FptsBlock->u32BlockAddr + 2;
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