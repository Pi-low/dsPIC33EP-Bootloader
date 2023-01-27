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
 * File: crc.h
 * Author: Nello
 * Mail: nello.chom@protonmail.com
 * 
 */

#ifndef CRC_H
#define CRC_H

#include <stdint.h>

void updateCrc16(uint16_t* Fpu16Input, uint8_t Fu8Data);
void BufUpdateCrc16(uint16_t* Fpu16Input, uint8_t* Fpu8Data, uint16_t Fu16Length);

#endif