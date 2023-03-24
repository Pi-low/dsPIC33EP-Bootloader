/* 
 * The dsPIC33EP-Bootloader is a basic and simple UART bootloaloader that
 * is designed to work with all dsPIC33EP 16bit Microchip MCU family.
 * 
 * Copyright (C) 2023  Nello Chommanivong
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 * File: bootloader.c
 * Author: Nello
 * Mail: nello.chom@protonmail.com
 * 
 */

/******************************************************************************/
/* INCLUDE                                                                    */
/******************************************************************************/
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

/******************************************************************************/
/* GLOBAL                                                                     */
/******************************************************************************/
static uint8_t u8AppPresentFlag;
static uint8_t u8BlankFlashFlag;
static uint8_t u8BootloadingFlag;
static uint16_t u16Timeout1;
static DataBlock_t tsDataBlock;
static uint32_t pu32SaveRstVector[2] = {0};
static uint32_t u32PrevBlockAddr;
static tsGenericMsg tsInternalMsg;
static void Ms_SendBootFrame(uint8_t Fu8Byte0);

/**
 * 
 */
void Mbootloader_ResetState(void)
{
    u8BlankFlashFlag = 0;
    u8BootloadingFlag = 0;
    u32PrevBlockAddr = 0;
    pu32SaveRstVector[0] = 0;
    pu32SaveRstVector[1] = 0;
}

/**
 * 
 */
void Mbootloader_updateTimeout(void)
{
    u16Timeout1 = TMR1_SoftwareCounterGet() + BOOT_TIMEOUT;
}

/**
 * 
 */
void Mbootloader_TimeoutMng(void)
{
    TMR1_Tasks_16BitOperation();
    if (u16Timeout1 < TMR1_SoftwareCounterGet())
    {
        if ((u8AppPresentFlag != 0) || (u8BootloadingFlag != 0))
        {
            Mbootloader_ResetState();
            MMisc_WatchdogDisable();
            RESET();
        }
    }
}

/**
 * 
 * @param Fu8Byte0
 */
void Ms_SendBootFrame(uint8_t Fu8Byte0)
{
    tsGenericMsg tsTxMsg;
    tsTxMsg.u8ID = 0;
    tsTxMsg.pu8Data[0] = Fu8Byte0;
    tsTxMsg.u16Length = 1;
    MTarget_SendFrame(&tsTxMsg);
}

/**
 * 
 */
void Mbootloader_InitBoot(void)
{
    uint32_t AppliPresent = (uint32_t) FLASH_ReadWord16(ADDR_APPL_FLAG) | ((uint32_t) FLASH_ReadWord16(ADDR_APPL_FLAG + 2) << 16);
    u8AppPresentFlag = (AppliPresent == APPLIVALID) ? 1 : 0;
    Mbootloader_ResetState();
    ClrWdt();
}

/**
 * 
 * @return 
 */
teMainStates Mbootloader_StateTransition(void)
{
    teMainStates eRetState = eStateTransition;
    teOperationRetVal eRetVal = eOperationNotAvailable;
    tsGenericMsg tsRxMsg;
    static uint16_t su16TM = 0;
    static uint8_t su8Cnt = 0;
    Mbootloader_updateTimeout();
    eRetVal = MTarget_FrameAvailable(&tsRxMsg);
    if (u8AppPresentFlag != 0) /* Application is present */
    {
        /* Sending boot attention message every 100ms, waiting for any rx message */
        if (TMR1_SoftwareCounterGet() > su16TM)
        {
            Ms_SendBootFrame(eBootAttention);
            su16TM = TMR1_SoftwareCounterGet() + 100;
            su8Cnt++;
        }
        if (eRetVal == eOperationSuccess)
        {
            Ms_SendBootFrame(eBootIdle);
            eRetState = eStateIdle;
        }
        else if ((TMR1_SoftwareCounterGet() > 300) || (su8Cnt > 3))
        {
            TMR1_Stop();
            MMisc_WatchdogDisable();
            StartApplication();
        }
    }
    else
    {
        Ms_SendBootFrame(eBootIdle);
        eRetState = eStateIdle;
    }
    
    return eRetState;
}

/**
 * 
 * @return 
 */
teMainStates Mbootloader_StateIdle(void)
{
    tsGenericMsg tsRxMsg;
    teMainStates eRetState = eStateIdle;
    teOperationRetVal eRetVal = eOperationNotAvailable;
    ClrWdt();
    if (MTarget_FrameAvailable(&tsRxMsg) == eOperationSuccess) /* On Rx frame */
    {
        Mbootloader_updateTimeout();
        switch (tsRxMsg.u8ID)
        {
        case eReq_gotoBoot:
            eRetVal = Mbootloader_RqStartBoot(&tsRxMsg);
            break;

        case eReq_getInfo:
            eRetVal = Mbootloader_RqGetInfo(&tsRxMsg);
            break;

        default :
            tsInternalMsg.u8ID = tsRxMsg.u8ID | 0x80;
            tsInternalMsg.pu8Data[0] = eOperationNotAllowed;
            tsInternalMsg.u16Length = 1;
            MTarget_SendFrame(&tsInternalMsg);
            break;
        }
        if (u8BootloadingFlag != 0)
        {
            eRetState = eStateFlash;
        }
    }
    return eRetState;
}

/**
 * 
 * @return 
 */
teMainStates Mbootloader_StateLoading(void)
{
    tsGenericMsg tsRxMsg;
    teOperationRetVal eRetVal;
    teMainStates eRetState = eStateFlash;
    if (MTarget_FrameAvailable(&tsRxMsg) == eOperationSuccess) /* On Rx frame */
    {
        Mbootloader_updateTimeout();
        ClrWdt();
        switch (tsRxMsg.u8ID)
        {
            
        case eReq_eraseFlash:
            eRetVal = Mbootloader_RqEraseFlash(&tsRxMsg);
            break;

        case eReq_dataTransfer:
            eRetVal = Mbootloader_RqDataTransfer(&tsRxMsg);
            break;

        case eReq_checkFlash:
            eRetVal = Mbootloader_RqCheckFlash(&tsRxMsg);
            break;

        default:
            tsInternalMsg.u8ID = tsRxMsg.u8ID | 0x80;
            tsInternalMsg.pu8Data[0] = eOperationNotAllowed;
            tsInternalMsg.u16Length = 1;
            MTarget_SendFrame(&tsInternalMsg);
            break;
        }
        if (eRetVal != eOperationSuccess)
        {
            Ms_SendBootFrame(eBootIdle);
            eRetState = eStateIdle;
            Mbootloader_ResetState();
        }
    }
    return eRetState;
}

/**
 * 
 * @param FptsGenMsg
 * @return 
 */
teOperationRetVal Mbootloader_RqStartBoot(tsGenericMsg* FptsGenMsg)
{
    teOperationRetVal eRetVal = eOperationSuccess;
    
    u8BootloadingFlag = 1;
    Mbootloader_updateTimeout();
    TMR1_Tasks_16BitOperation();
    tsInternalMsg.u8ID = FptsGenMsg->u8ID | 0x80;
    tsInternalMsg.pu8Data[0] = eRetVal;
    tsInternalMsg.u16Length = 1;
    
    MTarget_SendFrame(&tsInternalMsg);
    
    return eRetVal;
}

/**
 * 
 * @param FptsGenMsg
 * @return 
 */
teOperationRetVal Mbootloader_RqGetInfo(tsGenericMsg * FptsGenMsg)
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
        MFlash_ReadBufferU8(&pu8Buff[1], FLASH_LOGISTIC_CHAR_SIZE, ADDR_APPL_DESC);
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
    
    MTarget_SendFrame(&tsInternalMsg);
    
    return eRetVal;
}

/**
 * 
 * @param FptsGenMsg
 * @return 
 */
teOperationRetVal Mbootloader_RqEraseFlash(tsGenericMsg * FptsGenMsg)
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
            Mbootloader_updateTimeout();
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
    
    MTarget_SendFrame(&tsInternalMsg);
    
    return eRetVal;
}

/**
 * 
 * @param FptsGenMsg
 * @return 
 */
teOperationRetVal Mbootloader_RqDataTransfer(tsGenericMsg * FptsGenMsg)
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
        eRetVal = Mbootloader_ProcessDataBlock(FptsGenMsg, &tsDataBlock);
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
        eRetVal = MFlash_CheckRow(&tsDataBlock);
    }
    
    tsInternalMsg.u8ID = FptsGenMsg->u8ID | 0x80;
    tsInternalMsg.u16Length = 1;
    tsInternalMsg.pu8Data[0] = eRetVal;
    MTarget_SendFrame(&tsInternalMsg);
    
    return eRetVal;
}

/**
 * 
 * @param FptsGenMsg
 * @return 
 */
teOperationRetVal Mbootloader_RqCheckFlash(tsGenericMsg * FptsGenMsg)
{
    teOperationRetVal eRetVal = eOperationSuccess;
    uint16_t u16Cnt = 0;
    uint8_t pu8DataRowByte[256];
    uint16_t u16AppliRowCnt = (((uint16_t)FptsGenMsg->pu8Data[2] << 8) & 0xFF00) | ((uint16_t)FptsGenMsg->pu8Data[3] & 0x00FF); /* amount of rows after app start address*/
    uint16_t u16CRC = 0, u16CrcIvt = 0, u16CrcApp = 0;
    uint32_t u32RowAddr = 0;
    uint16_t pu16AppliFlag[2] = {APPLIVALID & 0xFFFF, (APPLIVALID >> 16) & 0xFFFF};
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
            MFlash_ReadRow(pu8DataRowByte, u32RowAddr);
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
            Mcrc_UpdateBuf(&u16CRC, pu8DataRowByte, BOOT_ROW_SIZE_BYTE);
        }
        u16CrcIvt = u16CRC;
        ClrWdt();
        for (u16Cnt = 0; u16Cnt < u16AppliRowCnt - 8; u16Cnt++)
        {
            u32RowAddr = ADDR_FLASH_APPLI + (u16Cnt * BOOT_ROW_SIZE_WORD * 2);
            MFlash_ReadRow(pu8DataRowByte, u32RowAddr);
            Mcrc_UpdateBuf(&u16CRC, pu8DataRowByte, BOOT_ROW_SIZE_BYTE);
        }
        u16CrcApp = u16CRC;
        Mcrc_update(&u16CRC, FptsGenMsg->pu8Data[0]);
        Mcrc_update(&u16CRC, FptsGenMsg->pu8Data[1]);
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
        }
        else
        {
            eRetVal = eAppliCheckError;
        }
        tsInternalMsg.u8ID = FptsGenMsg->u8ID | 0x80;
        tsInternalMsg.u16Length = 7;
        /* Provide debug data to the flashing tool for analysis */
        tsInternalMsg.pu8Data[0] = eRetVal;
        tsInternalMsg.pu8Data[1] = u16CrcIvt >> 8;
        tsInternalMsg.pu8Data[2] = u16CrcIvt;
        tsInternalMsg.pu8Data[3] = u16CrcApp >> 8;
        tsInternalMsg.pu8Data[4] = u16CrcApp;
        tsInternalMsg.pu8Data[5] = u16CRC >> 8;
        tsInternalMsg.pu8Data[6] = u16CRC;
        MTarget_SendFrame(&tsInternalMsg);
        if (eRetVal == eOperationSuccess)
        {
            MMisc_DelayMs(50);
            TMR1_Stop();
            Mbootloader_ResetState();
            RESET();
        }
    }
    
    return eRetVal;
}

/**
 * 
 * @param FptsGenMsg
 * @param FptsBlock
 * @return 
 */
teOperationRetVal Mbootloader_ProcessDataBlock(tsGenericMsg * FptsGenMsg, DataBlock_t * FptsBlock)
{
    teOperationRetVal eRetVal = eOperationSuccess;
    uint8_t u8DataCnt = 0;
    uint16_t u16Tmp = 0;
    uint16_t u16CRC = 0;
   
    /* STEP 1: CRC check */
    Mcrc_UpdateBuf(&u16CRC, FptsGenMsg->pu8Data, FptsGenMsg->u16Length);
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
        
        u16Tmp = MFlash_CharToWordBuffer(FptsBlock->pu32Word, FptsBlock->pu8Data, BOOT_ROW_SIZE_BYTE);
        
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