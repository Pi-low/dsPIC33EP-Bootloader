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
 * File: main.c
 * Author: Nello
 * Mail: nello.chom@protonmail.com
 * 
 */

#include "../../mcc_generated_files/system.h"
#include "../../mcc_generated_files/pin_manager.h"
#include "../../mcc_generated_files/tmr1.h"
#include "../../mcc_generated_files/memory/flash.h"
#include "../02-FLAH_ROUTINES/flash_routines.h"
#include "../03-TARGET/target.h"
#include "../05-BOOTLOADER/BootloaderTypes.h"
#include "../05-BOOTLOADER/bootloader.h"
#include "Misc.h"

#ifndef _IS_RELEASE
const char __attribute__((address(ADDR_APPL_DESC), space(prog))) text[FLASH_LOGISTIC_CHAR_SIZE] = __DATE__" "__TIME__" : Bootloader standalone";
const uint16_t SWVersion __attribute__((address(ADDR_APPL_VERSION), space(prog))) = 0x0001;
#endif

void main(void)
{
    teMainStates teCurrentState = eStateTransition;
    
    SYSTEM_Initialize();
    WatchdogEnable();
    MCU_FPWM_SetHigh();
    InitBackTask();
    TMR1_Start();
    InitBootloader();
    while(1)
    {
        TMR1_Tasks_16BitOperation(); /* SW timer management */
        ManageBackTask(); /* UART frame management */
        manageTimeout();
        switch(teCurrentState)
        {
        case eStateTransition:
            teCurrentState = State_Transition();
            break;
            
        case eStateIdle:
            teCurrentState = State_BootIdle();
            break;
            
        case eStateFlash:
            teCurrentState = State_Bootloading();
            break;
            
        default:
            break;
        }
    }
}