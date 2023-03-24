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
 * File: Misc.c
 * Author: Nello
 * Mail: nello.chom@protonmail.com
 * 
 */

/******************************************************************************/
/* INCLUDE                                                                    */
/******************************************************************************/
#include <xc.h>
#include <stdint.h>
#include "Misc.h"
#include "../../mcc_generated_files/tmr1.h"

/******************************************************************************/
/* GLOBAL                                                                     */
/******************************************************************************/

/**
 * @biref Blocking delay
 * @param[in] u16msDelay Delay time in milliseconds
 */
void MMisc_DelayMs(uint16_t u16msDelay)
{
    uint16_t u16Timeout = u16msDelay + TMR1_SoftwareCounterGet();
    while (u16Timeout > TMR1_SoftwareCounterGet())
    {
        TMR1_Tasks_16BitOperation();
        ClrWdt();
    };
}

void MMisc_WatchdogEnable(void)
{
    RCONbits.SWDTEN = 1;
}

void MMisc_WatchdogDisable(void)
{
    RCONbits.SWDTEN = 0;
} 