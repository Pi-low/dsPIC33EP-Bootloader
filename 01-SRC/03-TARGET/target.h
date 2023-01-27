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
 * File: target.h
 * Author: Nello
 * Mail: nello.chom@protonmail.com
 * 
 */

#ifndef TARGET_H
#define TARGET_H

#include <stdint.h>
#include "../05-BOOTLOADER/BootloaderTypes.h"

typedef enum
{
    eBackTask_Idle = 0,
    eBackTask_Data = 1
}teBackTaskStates;

void InitBackTask(void);
void ManageBackTask(void);
teOperationRetVal FrameAvailable(tsGenericMsg* FptsBootMsg);
void sendFrame(tsGenericMsg *FptsTxMsg);
//void BufCopy(uint8_t* pu8Dest, uint8_t* pu8Src, uint16_t u16Size);

#endif