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
 * File: FrameMng.c
 * Author: Nello
 * Mail: nello.chom@protonmail.com
 * 
 */

#include <stdint.h>
#include "../05-BOOTLOADER/BootloaderTypes.h"
#include "target.h"
#include "FrameMng.h"

static tsFrameSize tsFrameLength[REGISTERED_FRAMES] = 
{
    /* Service ID          MIN MAX  */
    {eBoot,                 2,  2   },
    {eService_gotoBoot,     1,  1   },
    {eService_getInfo,      2,  2   },
    {eService_eraseFlash,   1,  1   },
    {eService_dataTransfer, 7,  MAX_FRM_LEN },
    {eService_checkFlash,   5,  5   },
};

teOperationRetVal RxFrameHandler(tsUartFrm * UARTFrm, tsGenericMsg * FpGenMsg)
{
    uint16_t u16i = 0;
    uint16_t u16j = 0;
    teOperationRetVal RetVal = eOperationSuccess;
    uint8_t u8Checksum = 0;

    if (UARTFrm->u16Index < 3)
    {
        /* Waiting for more data */
        RetVal = eOperationNotAvailable;
    }
    
    if (RetVal == eOperationSuccess)
    {
        /* Check frame length */
        FpGenMsg->u8ID = UARTFrm->pu8Data[0];
        FpGenMsg->u16Length = ((uint16_t) UARTFrm->pu8Data[1] << 8) & 0xFF00;
        FpGenMsg->u16Length |= (uint16_t) UARTFrm->pu8Data[2] & 0xFF;
        if (FpGenMsg->u16Length > MAX_FRM_LEN)
        {
            /* Payload will be too long */
            RetVal = eBadFrameLength;
        }
        else
        {
            u16i = 0;
            /* Check ID and payload length */
            while ((u16i < REGISTERED_FRAMES) && (FpGenMsg->u8ID != tsFrameLength[u16i].u8ID))
            {
                /* Look for registered ID list */
                u16i++;
            }

            if (FpGenMsg->u8ID == tsFrameLength[u16i].u8ID)
            {
                if ((FpGenMsg->u16Length < tsFrameLength[u16i].u16MinLen) || 
                        (FpGenMsg->u16Length > tsFrameLength[u16i].u16MaxLen))
                {
                    RetVal = eBadFrameLength;
                }
            }
            else
            {
                RetVal = eUnknownFrameID;
            }
        }
    }
    
    if (RetVal == eOperationSuccess)
    {
        if (UARTFrm->u16Index == (FpGenMsg->u16Length + 3))
        {
            /* Data fully received */
            u16j = 0;
            for (u16i = 0; u16i < (FpGenMsg->u16Length + 3); u16i++) /* copy payload */
            {
                if (u16i >= 3)
                {
                    FpGenMsg->pu8Data[u16j] = UARTFrm->pu8Data[u16i];
                    u16j++;
                }
                
                u8Checksum += UARTFrm->pu8Data[u16i];
                
            }
            if (u8Checksum != 0xFF)
            {
                RetVal = eBadChecksum;
            }
            FpGenMsg->u16Length --; /* Substract checksum value from length */
        }
        else if(UARTFrm->u16Index > (FpGenMsg->u16Length + 3))
        {
            RetVal = eOperationFail;
        }
        else
        {
            /* Continue to receive data  */
            RetVal = eOperationNotAvailable;
        }
    }
    
    if (RetVal > eOperationNotAvailable)
    {
        FpGenMsg->u8ID += 0x90;
        FpGenMsg->pu8Data[0] = RetVal;
        FpGenMsg->u16Length = 1;
        sendFrame(FpGenMsg);
    }
    return RetVal;
}
