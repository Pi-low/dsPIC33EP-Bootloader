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
 * File: flash_routines.c
 * Author: Nello
 * Mail: nello.chom@protonmail.com
 * 
 */

#include <stdlib.h>
#include "../../mcc_generated_files/system.h"
#include "../../mcc_generated_files/memory/flash.h"
#include "../05-BOOTLOADER/BootloaderTypes.h"
#include "../05-BOOTLOADER/bootloader.h"
#include "flash_routines.h"

teOperationRetVal FlashReadBufferU8(uint8_t* Fpu8Buffer, uint16_t Fu16Size, uint32_t Fu32FlashAddr)
{
    teOperationRetVal eRetVal = eOperationSuccess;
    uint16_t u16W = 0;
    uint16_t u8WordCnt = 0;
    uint16_t u16CharCnt = 0;
    uint16_t u16WordAmount = Fu16Size >> 1;
    uint8_t *pu8Buff = Fpu8Buffer;
    
    for (u8WordCnt = 0; u8WordCnt < u16WordAmount; u8WordCnt++)
    {
        u16W = FLASH_ReadWord16(Fu32FlashAddr + (u8WordCnt * 2)); /* One flash word is 2 PC addr */
        if (u16CharCnt < Fu16Size)
        {
            pu8Buff[u16CharCnt] = u16W & 0x00FF;
            u16CharCnt++;
        }
        if (u16CharCnt < Fu16Size)
        {
            pu8Buff[u16CharCnt] = (u16W >> 8) & 0x00FF;
            u16CharCnt++;
        }
    }
    return eRetVal;
}

teOperationRetVal FlashReadRow(uint8_t* Fpu8Buffer, uint32_t Fu32FlashAddr)
{
    teOperationRetVal eRetVal = eOperationSuccess;
    uint8_t *pu8Byte = Fpu8Buffer;
    uint16_t u16Cnt = 0;
    uint32_t pu32RowData24[BOOT_ROW_SIZE_WORD] = {0};

    for (u16Cnt = 0; u16Cnt < BOOT_ROW_SIZE_WORD; u16Cnt++)
    {
        pu32RowData24[u16Cnt] = FLASH_ReadWord24(Fu32FlashAddr + (u16Cnt * 2));
    }
    WordToCharBuffer(pu32RowData24, pu8Byte, BOOT_ROW_SIZE_WORD);
    return eRetVal;
}

teOperationRetVal FlashCheckRow(DataBlock_t* FptsWrittenBlock)
{
    teOperationRetVal eRetVal = eOperationSuccess;
    uint32_t u32FlashWord;
    uint32_t u32Err = 0;
    uint16_t u16Cnt = 0;
    
    for (u16Cnt = 0; u16Cnt < BOOT_ROW_SIZE_WORD; u16Cnt++)
    {
        u32FlashWord = FLASH_ReadWord24(FptsWrittenBlock->u32BlockAddr + (2 * u16Cnt));
        u32Err |= u32FlashWord ^ FptsWrittenBlock->pu32Word[u16Cnt];
    }
    
    if (u32Err != 0)
    {
        eRetVal = eFlashWriteError;
    }
    return eRetVal;
}

uint16_t CharToWordBuffer(uint32_t* Fpu32WordData, uint8_t* Fpu8CharData, uint16_t Fu16CharSize)
{
    uint16_t u16CharCnt = 0;
    uint16_t u16WordCnt = 0;
    uint8_t u8Char0 = 0, u8Char1 = 0, u8Char2 = 0;
    
    while (u16CharCnt < Fu16CharSize)
    {
        u8Char0 = *(Fpu8CharData + u16CharCnt);
        u16CharCnt ++;
        if (u16CharCnt < Fu16CharSize)
        {
            u8Char1 = *(Fpu8CharData + u16CharCnt);
            u16CharCnt ++;
        }
        else
        {
            u8Char1 = 0;
        }
        if (u16CharCnt < Fu16CharSize)
        {
            u8Char2 = *(Fpu8CharData + u16CharCnt);
            u16CharCnt += 2; /* escape phantom byte */
        }
        else
        {
            u8Char2 = 0;
        }
        
        *(Fpu32WordData + u16WordCnt) = (uint32_t) u8Char0 | 
                (((uint32_t) u8Char1 << 8) & 0xFF00) | 
                (((uint32_t) u8Char2 << 16) & 0xFF0000) ;
        u16WordCnt ++;
    }
    
    
    return u16WordCnt;
}

void WordToCharBuffer(uint32_t* Fpu32WordData, uint8_t* Fpu8CharData, uint16_t Fu16WordSize)
{
    uint32_t *pu32Word = Fpu32WordData;
    uint8_t *pu8Byte = Fpu8CharData;
    uint16_t u16Cnt = 0;
    
    for (u16Cnt = 0; u16Cnt < Fu16WordSize; u16Cnt++)
    {
        *pu8Byte = (uint8_t) *pu32Word;
        pu8Byte++;
        *pu8Byte = (uint8_t) (*(pu32Word) >> 8);
        pu8Byte++;
        *pu8Byte = (uint8_t) (*(pu32Word) >> 16);
        pu8Byte++;
        *pu8Byte = (uint8_t) (*(pu32Word) >> 24);
        pu8Byte++;
        pu32Word++;
    }
}
