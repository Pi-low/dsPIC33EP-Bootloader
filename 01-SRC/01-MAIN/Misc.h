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
 * File:   Misc.h
 * Author: Nello
 * Mail: nello.chom@protonmail.com
 *
 * Created on January 15, 2023, 2:27 PM
 */

#ifndef MISC_H
#define	MISC_H

#include <xc.h>
#include <stdint.h>

void BlockingDelay(uint16_t u16Timeout);
void WatchdogEnable(void);
void WatchdogDisable(void);

#endif	/* MISC_H */

