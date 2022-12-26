#ifndef FLASH_ROUTINE_H
#define FLASH_ROUTINE_H

#include <stdint.h>
#include "../01-MAIN/BootloaderTypes.h"

teOperationRetVal FlashReadBufferU8(uint8_t* Fpu8Buffer, uint16_t Fu16Size, uint32_t Fu32FlashAddr);
teOperationRetVal FlashReadBufferU32(uint32_t* Fpu32Buffer, uint16_t Fu16Size, uint32_t Fu32FlashAddr);
uint16_t CharToWordBuffer(uint32_t* Fpu32WordData, uint8_t* Fpu8CharData, uint16_t Fu16CharSize);
teOperationRetVal FlashCheckRow(DataBlock_t* FptsWrittenBlock);
#endif