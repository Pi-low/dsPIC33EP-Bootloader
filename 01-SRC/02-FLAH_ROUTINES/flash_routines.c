
#include <stdlib.h>
#include "../../mcc_generated_files/system.h"
#include "../../mcc_generated_files/memory/flash.h"
#include "../01-MAIN/BootloaderTypes.h"
#include "../01-MAIN/bootloader.h"
#include "flash_routines.h"

teOperationRetVal FlashReadBufferU8(uint8_t* Fpu8Buffer, uint16_t Fu16Size, uint32_t Fu32FlashAddr)
{
    teOperationRetVal eRetVal = eOperationSuccess;
    uint16_t u16W = 0;
    uint16_t u8WordCnt = 0;
    uint16_t u8CharCnt = 0;
    uint16_t u16WordAmount = Fu16Size >> 1;
    
    for (u8WordCnt = 0; u8WordCnt < u16WordAmount; u8WordCnt++)
    {
        u16W = FLASH_ReadWord16(Fu32FlashAddr + (u8WordCnt * 2)); /* One flash word is 2 PC addr */
        if (u8CharCnt < Fu16Size)
        {
            Fpu8Buffer[u8CharCnt] = u16W & 0x00FF;
            u8CharCnt++;
        }
        if (u8CharCnt < Fu16Size)
        {
            Fpu8Buffer[u8CharCnt] = (u16W >> 8) & 0x00FF;
            u8CharCnt++;
        }
    }
    return eRetVal;
}

teOperationRetVal FlashReadBufferU32(uint32_t* Fpu32Buffer, uint8_t* Fpu8Buffer, uint32_t Fu32FlashAddr, uint16_t Fu16Size)
{
    teOperationRetVal eRetVal = eOperationSuccess;
    uint16_t u16WCnt = 0;
    uint32_t u32Word = 0;
    uint8_t* pu8buf = Fpu8Buffer;
    uint32_t* pu32buf = Fpu32Buffer;
    
    if (pu8buf != NULL || pu32buf != NULL)
    {
        for (u16WCnt = 0; u16WCnt < Fu16Size; u16WCnt++)
        {
            u32Word = FLASH_ReadWord24(Fu32FlashAddr + (u16WCnt * 2));
            if (pu8buf != NULL)
            {
                *pu8buf = u32Word & 0xFF;
                pu8buf++;
                *pu8buf = (u32Word >> 8) & 0xFF;
                pu8buf++;
                *pu8buf = (u32Word >> 16) & 0xFF;
                pu8buf++;
                *pu8buf = 0;
                pu8buf++;
            }
            if (pu32buf != NULL)
            {
                *pu32buf = u32Word;
                pu32buf++;
            }
        }
    }
    else
    {
        eRetVal = eOperationFail;
    }
    
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
