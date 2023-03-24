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
 * File: BootloaderTypes.h
 * Author: Nello
 * Mail: nello.chom@protonmail.com
 * 
 */

#ifndef BOOT_TYPES_H
#define BOOT_TYPES_H

#include <stdint.h>

/* Max frame length:
 * - Block NVM address : 3 bytes
 * - data : up to 256 bytes
 * - CRC : 2 bytes
 * - Frame checksum : 1 byte
 */
#define MAX_FRM_LEN (262U)
#define RX_BUFFER_SIZE (266U)
#define FRAME_TIMEOUT (60U) /* 60ms */

typedef enum
{
    eOperationSuccess =         0,
    eOperationNotAvailable =    1,
    eOperationNotAllowed =      2,
    eOperationFail =            3,
    eBadChecksum =              4,
    eBadFrameLength =           5,
    eUnknownFrameID =           6,
    eFrameTimeout =             7,
    eFlashEraseError =          8,
    eMemoryNotBlanked =         9,
    eBadBlockAddr =             10,
    eBadCRCBlock =              11,
    eFlashWriteError =          12,
    eAppliCheckError =          13,
    eBootSessionTimeout =       14,
    eBootIdle =                 15,
    eBootAttention =            16,
}teOperationRetVal;

typedef enum
{
    eBoot =                 0x00,
    eReq_gotoBoot =         0x01,
    eReq_getInfo =          0x03,
    eReq_eraseFlash =       0x04,
    eReq_dataTransfer =     0x05,
    eReq_checkFlash =       0x06,
}teBootServices;

#define REGISTERED_FRAMES 6

typedef enum
{
    eStateTransition,
    eStateIdle,
    eStateFlash
}teMainStates;

typedef struct
{
    uint32_t u32BlockAddr;
    uint16_t u16BlockSize8;
    uint8_t pu8Data[256];
    uint32_t pu32Word[32];
    uint16_t u16CRC;
}DataBlock_t;

typedef struct
{
    uint16_t u16Index;
    uint8_t pu8Data[RX_BUFFER_SIZE];
    uint8_t u8Flag;
    uint16_t u16Timeout;
}tsUartFrm;

typedef struct
{
    uint8_t u8ID;
    uint16_t u16Length;
    uint8_t pu8Data[MAX_FRM_LEN];
} tsGenericMsg;

typedef struct
{
    uint8_t u8ID;
    uint16_t u16MinLen;
    uint16_t u16MaxLen;
} tsFrameSize;


#endif