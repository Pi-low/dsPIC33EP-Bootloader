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
 * File: bootloader.h
 * Author: Nello
 * Mail: nello.chom@protonmail.com
 * 
 */

#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include <stdint.h>
#include "BootloaderTypes.h"

#define BOOT_ROW_SIZE_BYTE          (256)
#define BOOT_PAGE_SIZE_BYTE         (2048)
#define BOOT_ROW_SIZE_WORD          (64)
#define BOOT_PAGE_SIZE_WORD         (512)
#define BOOT_ROW_OFFSET_ADDR        (0x80)
#define BOOT_PAGE_OFFSET_ADDR       (0x200)

#define ADDR_FLASH_LOGISTIC         (0x200)
#define ADDR_FLASH_BOOT             (0x400)
#define ADDR_FLASH_APPLI            (0x4000)
#define ADDR_FLASH_END              (0x55400)

#define FLASH_APPLI_PAGES           ((ADDR_FLASH_END - ADDR_FLASH_APPLI) / (BOOT_PAGE_SIZE_WORD * 2))
#define FLASH_LOGISTIC_CHAR_SIZE    (128u)

#define BOOT_TIMEOUT    (5000) /* ms */
#define STARTUP_DELAY   (300) /* ms */
#define BOOTFLAG (0xC0DEFEED)
#define APPLIVALID (0xA1B2C3D4L)
#define RESET() asm ("RESET")
#define StartApplication() __asm__ volatile("goto %0"::"i"(ADDR_FLASH_APPLI))
#define MAIN_TM_VALUE       (5000) /* 5 seconds */

#define ADDR_APPL_FLAG      (0x200)
#define ADDR_APPL_DESC      (0x280)
#define ADDR_APPL_VERSION   (0x300)

void resetBootState(void);
void updateTimeout(void);
void manageTimeout(void);
void InitBootloader(void);

teMainStates State_Transition(void);
teMainStates State_Bootloading(void);
teMainStates State_BootIdle(void);

teOperationRetVal serviceGoToBoot(tsGenericMsg* FptsGenMsg);
teOperationRetVal serviceGetInfo(tsGenericMsg* FptsGenMsg);
teOperationRetVal serviceEraseFlash(tsGenericMsg* FptsGenMsg);
teOperationRetVal serviceDataTransfer(tsGenericMsg* FptsGenMsg);
teOperationRetVal serviceCheckFlash(tsGenericMsg* FptsGenMsg);
teOperationRetVal manageDataBlock(tsGenericMsg * FptsGenMsg, DataBlock_t * FptsBlock);

#endif