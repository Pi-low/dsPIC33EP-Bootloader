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
 * File: FrameMng.h
 * Author: Nello
 * Mail: nello.chom@protonmail.com
 * 
 */

#ifndef FRAME_MNG_H
#define FRAME_MNG_H

#include <stdint.h>
#include "../05-BOOTLOADER/BootloaderTypes.h"


#define Mcr_ResetAvailable(tsFrm)   ((tsFrm).u8Flag &= 0xFE)
#define Mcr_SetAvailable(tsFrm)     ((tsFrm).u8Flag |= 0x01)
#define Mcr_GetAvailable(tsFrm)     (uint8_t)((tsFrm).u8Flag & 1)

#define Mcr_ResetStartFrame(tsFrm)  ((tsFrm).u8Flag &= 0xFD)
#define Mcr_SetStartFrame(tsFrm)    ((tsFrm).u8Flag |= 0x02)
#define Mcr_GetStartFrame(tsFrm)    (uint8_t)(((tsFrm).u8Flag >> 1) & 1)

teOperationRetVal MFrameMng_RxHandler(tsUartFrm * UARTFrm, tsGenericMsg * BootMsg);

#endif