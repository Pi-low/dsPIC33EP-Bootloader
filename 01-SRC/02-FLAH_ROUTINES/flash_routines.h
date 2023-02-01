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
 * File: flash_routines.h
 * Author: Nello
 * Mail: nello.chom@protonmail.com
 * 
 */

#ifndef FLASH_ROUTINE_H
#define FLASH_ROUTINE_H

#include <stdint.h>
#include "../05-BOOTLOADER/BootloaderTypes.h"

teOperationRetVal MFlash_ReadBufferU8(uint8_t* Fpu8Buffer, uint16_t Fu16Size, uint32_t Fu32FlashAddr);
teOperationRetVal MFlash_ReadRow(uint8_t* Fpu8Buffer, uint32_t Fu32FlashAddr);
uint16_t MFlash_CharToWordBuffer(uint32_t* Fpu32WordData, uint8_t* Fpu8CharData, uint16_t Fu16CharSize);
teOperationRetVal MFlash_CheckRow(DataBlock_t* FptsWrittenBlock);
void MFlash_WordToCharBuffer(uint32_t* Fpu32WordData, uint8_t* Fpu8CharData, uint16_t Fu16WordSize);
#endif