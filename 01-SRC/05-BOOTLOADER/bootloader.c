#include <stdlib.h>
#include <string.h>
#include "../../mcc_generated_files/system.h"
#include "../../mcc_generated_files/pin_manager.h"
#include "../../mcc_generated_files/tmr1.h"
#include "../../mcc_generated_files/memory/flash.h"
#include "../03-TARGET/target.h"
#include "../02-FLAH_ROUTINES/flash_routines.h"
#include "../04-CRC/crc.h"
#include "../01-MAIN/Misc.h"
#include "BootloaderTypes.h"
#include "bootloader.h"

static uint8_t u8AppPresentFlag;
static uint8_t u8BlankFlashFlag;
static uint8_t u8BootloadingFlag;
static uint16_t u16Timeout1, u16TransitionTimeout;
static DataBlock_t tsDataBlock;
static uint32_t pu32SaveRstVector[2] = {0};
static uint32_t u32PrevBlockAddr;
static tsGenericMsg tsInternalMsg;

static void SendBootFrame(uint8_t Fu8Byte0);

void resetBootState(void)
{
    u8BlankFlashFlag = 0;
    u8BootloadingFlag = 0;
    u32PrevBlockAddr = 0;
    pu32SaveRstVector[0] = 0;
    pu32SaveRstVector[1] = 0;
    BootRequest = 0;
}

void updateTimeout(void)
{
    u16Timeout1 = TMR1_SoftwareCounterGet() + BOOT_TIMEOUT;
}

void manageTimeout(void)
{
    TMR1_Tasks_16BitOperation();
    if (u16Timeout1 < TMR1_SoftwareCounterGet())
    {
        if ((u8AppPresentFlag != 0) || (u8BootloadingFlag != 0))
        {
            //SendBootFrame(eBootSessionTimeout);
            resetBootState();
            RESET();
        }
    }
}

void SendBootFrame(uint8_t Fu8Byte0)
{
    tsGenericMsg tsTxMsg;
    tsTxMsg.u8ID = 0;
    tsTxMsg.pu8Data[0] = Fu8Byte0;
    tsTxMsg.u16Length = 1;
    sendFrame(&tsTxMsg);
}

void InitBootloader(void)
{
    uint32_t AppliPresent = (uint32_t) FLASH_ReadWord16(ADDR_APPL_FLAG) | ((uint32_t) FLASH_ReadWord16(ADDR_APPL_FLAG + 2) << 16);
    u8AppPresentFlag = (AppliPresent == APPLIVALID) ? 1 : 0;
    resetBootState();
    u16TransitionTimeout = TMR1_SoftwareCounterGet() + 250;
    SendBootFrame(eBootAttention);
    ClrWdt();
}

teMainStates State_Transition(void)
{
    teMainStates eRetState = eStateTransition;
    teOperationRetVal eRetVal = eOperationNotAvailable;
    tsGenericMsg tsRxMsg;
    TMR1_Tasks_16BitOperation();
    if ((u8AppPresentFlag != 0) && (BootRequest != BOOTFLAG)) /* Application is present, no bootloader flag*/
    {
        if (((TMR1_SoftwareCounterGet() % 100) == 0) && (TMR1_SoftwareCounterGet() != 0))
        {
            SendBootFrame(eBootAttention);
        }
        eRetVal = FrameAvailable(&tsRxMsg);
        if (eRetVal == eOperationSuccess)
        {
            SendBootFrame(eBootIdle);
            eRetState = eStateIdle;
        }
        else if ((u16TransitionTimeout < TMR1_SoftwareCounterGet()) && (eRetVal != eOperationSuccess))
        {
            TMR1_Stop();
            StartApplication();
        }
    }
    else
    {
        eRetState = eStateIdle;
    }
    
    return eRetState;
}

teMainStates State_BootIdle(void)
{
    tsGenericMsg tsRxMsg;
    teMainStates eRetState = eStateIdle;
    teOperationRetVal eRetVal = eOperationNotAvailable;
    ClrWdt();
    if (FrameAvailable(&tsRxMsg) == eOperationSuccess) /* On Rx frame */
    {
        updateTimeout();
        switch (tsRxMsg.u8ID)
        {
        case eService_gotoBoot:
            eRetVal = serviceGoToBoot(&tsRxMsg);
            break;
            
        case eService_echo:
            eRetVal = serviceEcho(&tsRxMsg);
            break;

        case eService_getInfo:
            eRetVal = serviceGetInfo(&tsRxMsg);
            break;

        case eService_writePin:
            break;

        case eService_readPin:
            break;
            
        default :
            break;
        }
        if (u8BootloadingFlag != 0)
        {
            eRetState = eStateFlash;
        }
    }
    return eRetState;
}

teMainStates State_Bootloading(void)
{
    tsGenericMsg tsRxMsg;
    teOperationRetVal eRetVal;
    teMainStates eRetState = eStateFlash;
    if (FrameAvailable(&tsRxMsg) == eOperationSuccess) /* On Rx frame */
    {
        updateTimeout();
        ClrWdt();
        switch (tsRxMsg.u8ID)
        {
            
        case eService_eraseFlash:
            eRetVal = serviceEraseFlash(&tsRxMsg);
            break;

        case eService_dataTransfer:
            eRetVal = serviceDataTransfer(&tsRxMsg);
            break;

        case eService_checkFlash:
            eRetVal = serviceCheckFlash(&tsRxMsg);
            break;

        default:
            break;
        }
        if (eRetVal != eOperationSuccess)
        {
            SendBootFrame(eBootIdle);
            eRetState = eStateIdle;
            resetBootState();
        }
    }
    return eRetState;
}

teOperationRetVal serviceGoToBoot(tsGenericMsg* FptsGenMsg)
{
    teOperationRetVal eRetVal = eOperationSuccess;
    
    u8BootloadingFlag = 1;
    updateTimeout();
    TMR1_Tasks_16BitOperation();
    tsInternalMsg.u8ID = FptsGenMsg->u8ID | 0x80;
    tsInternalMsg.pu8Data[0] = eRetVal;
    tsInternalMsg.u16Length = 1;
    
    sendFrame(&tsInternalMsg);
    
    return eRetVal;
}

teOperationRetVal serviceEcho(tsGenericMsg* FptsGenMsg)
{
    teOperationRetVal eRetVal = eOperationSuccess;
    
    tsInternalMsg.u8ID = FptsGenMsg->u8ID | 0x80;
    tsInternalMsg.u16Length = 1 + FptsGenMsg->u16Length;
    
//    for (u8i = 0; u8i < FptsGenMsg->u16Length; u8i++)
//    {
//        u8TmpBuff[u8i + 1] = FptsGenMsg->pu8Data[u8i];
//        tsInternalMsg.u16Length++;
//    }
    tsInternalMsg.pu8Data[0] = eRetVal;
    memcpy(&tsInternalMsg.pu8Data[1], FptsGenMsg->pu8Data, FptsGenMsg->u16Length);
    
//    BufCopy(tsInternalMsg.pu8Data, u8TmpBuff, tsInternalMsg.u16Length);
    
    sendFrame(&tsInternalMsg);
    
    return eRetVal;
}

teOperationRetVal serviceGetInfo(tsGenericMsg * FptsGenMsg)
{
    teOperationRetVal eRetVal = eOperationSuccess;
    uint8_t* pu8Buff = tsInternalMsg.pu8Data;
    uint16_t u16SwVersion;
    uint32_t u32ApplicationPresentFlag;
    
    pu8Buff[0] = eRetVal;
    tsInternalMsg.u8ID = FptsGenMsg->u8ID | 0x80;
    tsInternalMsg.u16Length = 1;
    
    switch (FptsGenMsg->pu8Data[0])
    {
    case 0: /* Get application present flag */
        u32ApplicationPresentFlag = (uint32_t)FLASH_ReadWord16(ADDR_APPL_FLAG)| 
                ((uint32_t)FLASH_ReadWord16(ADDR_APPL_FLAG + 2) << 16);
        
        pu8Buff[1] = u32ApplicationPresentFlag >> 24;
        pu8Buff[2] = u32ApplicationPresentFlag >> 16;
        pu8Buff[3] = u32ApplicationPresentFlag >> 8;
        pu8Buff[4] = u32ApplicationPresentFlag;
        tsInternalMsg.u16Length += 4;
        break;

    case 1: /* Get application version */
        u16SwVersion = FLASH_ReadWord16(ADDR_APPL_VERSION);
        pu8Buff[1] = u16SwVersion >> 8;
        pu8Buff[2] = u16SwVersion;
        tsInternalMsg.u16Length += 2;
        break;

    case 2: /* Get application logistic ascii string */
        FlashReadBufferU8(&pu8Buff[1], FLASH_LOGISTIC_CHAR_SIZE, ADDR_APPL_DESC);
        tsInternalMsg.u16Length += FLASH_LOGISTIC_CHAR_SIZE;
        break;

    default:
        eRetVal = eOperationFail;
        break;
    }
    
    if (eRetVal != eOperationSuccess)
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
    
    if (u8BootloadingFlag == 0)
    {
        eRetVal = eOperationNotAllowed;
    }

    if (eRetVal == eOperationSuccess)
    {
        FLASH_Unlock(FLASH_UNLOCK_KEY);

        u8Err = FLASH_ErasePage(0);
        u8Err &= FLASH_WriteDoubleWord24(0, pu32SaveRstVector[0], pu32SaveRstVector[1]);

        while ((u32i < FLASH_APPLI_PAGES) && (u8Err == 1)) /* break loop in case of error */
        {
            /* Page start address % 0x400 */
            u8Err &= FLASH_ErasePage(ADDR_FLASH_APPLI + (u32i * FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS));
            u32i++;
            TMR1_Tasks_16BitOperation();
            updateTimeout();
            ClrWdt();
        }
        if ((u8Err == 1) && (u32i == FLASH_APPLI_PAGES))
        {
            u8BlankFlashFlag = 1;
            u32PrevBlockAddr = 0;
        }
        else
        {
            eRetVal = eFlashEraseError;
        }

        FLASH_Lock();
    }
    
    tsInternalMsg.u8ID = FptsGenMsg->u8ID | 0x80;
    tsInternalMsg.u16Length = 1;
    tsInternalMsg.pu8Data[0] = eRetVal;
    
    sendFrame(&tsInternalMsg);
    
    return eRetVal;
}

teOperationRetVal serviceDataTransfer(tsGenericMsg * FptsGenMsg)
{
    teOperationRetVal eRetVal = eOperationSuccess;
    
    if (u8BootloadingFlag == 0)
    {
         eRetVal = eOperationNotAllowed;
    }
    else if (u8BlankFlashFlag == 0)
    {
        eRetVal = eMemoryNotBlanked;
    }
    
    if(eRetVal == eOperationSuccess)
    {
        eRetVal = manageDataBlock(FptsGenMsg, &tsDataBlock);
    }

    if (eRetVal == eOperationSuccess)
    {
        FLASH_Unlock(FLASH_UNLOCK_KEY);
        if (FLASH_WriteRow24(tsDataBlock.u32BlockAddr, tsDataBlock.pu32Word) != true)
        {
            eRetVal = eFlashWriteError;
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
    uint16_t u16Cnt = 0;
    uint8_t pu8DataRowByte[256];
    uint16_t u16AppliRowCnt = (((uint16_t)FptsGenMsg->pu8Data[2] << 8) & 0xFF00) | ((uint16_t)FptsGenMsg->pu8Data[3] & 0x00FF); /* amount of rows after app start address*/
    uint16_t u16CRC = 0, u16CrcIvt = 0, u16CrcApp = 0;
    uint32_t u32RowAddr = 0;
    uint16_t pu16AppliFlag[2] = {APPLIVALID & 0xFFFF, (APPLIVALID >> 16) & 0xFFFF};
    uint16_t u16DelayTime = 0;
    bool bRetVal = true;
    
    if (u8BootloadingFlag == 0)
    {
        eRetVal = eOperationNotAllowed;
    }
    else if (u8BlankFlashFlag == 0)
    {
        eRetVal = eMemoryNotBlanked;
    }
    
    if (eRetVal == eOperationSuccess)
    {
        for (u16Cnt = 0; u16Cnt < 8; u16Cnt++)
        {
            /* Calc first page (IVT + logistic info data), one page is 8 rows */
            u32RowAddr = u16Cnt * BOOT_ROW_SIZE_WORD * 2;
            FlashReadRow(pu8DataRowByte, u32RowAddr);
            if (u32RowAddr == 0)
            {
                /* Blank reset vector address */
                pu8DataRowByte[0] = 0xFF;
                pu8DataRowByte[1] = 0xFF;
                pu8DataRowByte[2] = 0xFF;
                pu8DataRowByte[3] = 0x00;
                pu8DataRowByte[4] = 0xFF;
                pu8DataRowByte[5] = 0xFF;
                pu8DataRowByte[6] = 0xFF;
                pu8DataRowByte[7] = 0x00;
            }
            BufUpdateCrc16(&u16CRC, pu8DataRowByte, BOOT_ROW_SIZE_BYTE);
        }
        u16CrcIvt = u16CRC;
        ClrWdt();
        for (u16Cnt = 0; u16Cnt < u16AppliRowCnt - 8; u16Cnt++)
        {
            u32RowAddr = ADDR_FLASH_APPLI + (u16Cnt * BOOT_ROW_SIZE_WORD * 2);
            FlashReadRow(pu8DataRowByte, u32RowAddr);
            BufUpdateCrc16(&u16CRC, pu8DataRowByte, BOOT_ROW_SIZE_BYTE);
        }
        u16CrcApp = u16CRC;
        updateCrc16(&u16CRC, FptsGenMsg->pu8Data[0]);
        updateCrc16(&u16CRC, FptsGenMsg->pu8Data[1]);
        if (u16CRC == 0xF0B8)
        {
            ClrWdt();
            FLASH_Unlock(FLASH_UNLOCK_KEY);
            bRetVal = FLASH_WriteDoubleWord24(ADDR_APPL_FLAG, pu16AppliFlag[0], pu16AppliFlag[1]);
            FLASH_Lock();
            if (bRetVal != true)
            {
                eRetVal = eOperationFail;
            }
            else
            {
                tsInternalMsg.u8ID = FptsGenMsg->u8ID | 0x80;
                tsInternalMsg.u16Length = 1;
                tsInternalMsg.pu8Data[0] = eRetVal;
                sendFrame(&tsInternalMsg);
                BlockingDelay(50);
                resetBootState();
                RESET();
            }
        }
        else
        {
            eRetVal = eAppliCheckError;
            tsInternalMsg.u8ID = FptsGenMsg->u8ID | 0x80;
            tsInternalMsg.u16Length = 7;

            tsInternalMsg.pu8Data[0] = eRetVal;
            tsInternalMsg.pu8Data[1] = u16CrcIvt >> 8;
            tsInternalMsg.pu8Data[2] = u16CrcIvt;
            tsInternalMsg.pu8Data[3] = u16CrcApp >> 8;
            tsInternalMsg.pu8Data[4] = u16CrcApp;
            tsInternalMsg.pu8Data[5] = u16CRC >> 8;
            tsInternalMsg.pu8Data[6] = u16CRC;
            sendFrame(&tsInternalMsg);
        }
    }
    
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
        
        if ((FptsBlock->u32BlockAddr % BOOT_ROW_OFFSET_ADDR) != 0)
        {
            /* Address is not aligned to row boudary */
            eRetVal = eBadBlockAddr;
        }
        if ((FptsBlock->u32BlockAddr >= ADDR_FLASH_BOOT) && (FptsBlock->u32BlockAddr < ADDR_FLASH_APPLI))
        {
            /* Block is located in boot area */
            eRetVal = eBadBlockAddr;
        }
        if (FptsBlock->u32BlockAddr >= ADDR_FLASH_END)
        {
            /* Block is located over applicatino area */
            eRetVal = eBadBlockAddr;
        }
        if (u32PrevBlockAddr > FptsBlock->u32BlockAddr) /* Prevent from writing back */
        {
            eRetVal = eBadBlockAddr;
        }
    }
    
    /* STEP 3: Generate flash row from received data */
    if (eRetVal == eOperationSuccess)
    {
        /* Fill 0xFF till' block start address */
        for (u16Tmp = 0; u16Tmp < BOOT_ROW_SIZE_BYTE; u16Tmp++)
        {
            if (u8DataCnt < FptsBlock->u16BlockSize8)
            {
                FptsBlock->pu8Data[u16Tmp] = FptsGenMsg->pu8Data[u8DataCnt + 3]; /* Escape block address data into frame */
                u8DataCnt++;
            }
            else
            {
                FptsBlock->pu8Data[u16Tmp] = 0xFF;
            }
        }
        
        u16Tmp = CharToWordBuffer(FptsBlock->pu32Word, FptsBlock->pu8Data, BOOT_ROW_SIZE_BYTE);
        
        if (FptsBlock->u32BlockAddr == 0) /* Restor reset vector */
        {
            FptsBlock->pu32Word[0] = pu32SaveRstVector[0];
            FptsBlock->pu32Word[1] = pu32SaveRstVector[1];
        }
        if (u16Tmp == 0)
        {
            eRetVal = eOperationFail;
        }
        u32PrevBlockAddr = FptsBlock->u32BlockAddr + 2;
    }
    return eRetVal;
}