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
 * File: target.c
 * Author: Nello
 * Mail: nello.chom@protonmail.com
 * 
 */

/******************************************************************************/
/* INCLUDE                                                                    */
/******************************************************************************/
#include <stdlib.h>
#include <string.h>
#include "../../mcc_generated_files/system.h"
#include "../../mcc_generated_files/uart1.h"
#include "../../mcc_generated_files/tmr1.h"
#include "../05-BOOTLOADER/BootloaderTypes.h"
#include "FrameMng.h"
#include "target.h"

/******************************************************************************/
/* GLOBAL                                                                     */
/******************************************************************************/
static teBackTaskStates teCurrentState;
static tsGenericMsg tsBootMsg;
static tsUartFrm tsUartFrame;

void MTarget_InitBackTask(void)
{
    teCurrentState = eBackTask_Idle;
}

void MTarget_BackTaskMng(void)
{
    uint8_t RxData;
    teOperationRetVal eRetVal;
    ClrWdt();
    if ((Mcr_GetStartFrame(tsUartFrame) == 1) && 
            (TMR1_SoftwareCounterGet() > tsUartFrame.u16Timeout) && 
            (Mcr_GetAvailable(tsUartFrame) == 0))
    {
        /* Frame timeout */
        teCurrentState = eBackTask_Idle;
        Mcr_ResetStartFrame(tsUartFrame);
    }
    
    if (U1STAbits.URXDA == 1)
    {
        if (U1STAbits.OERR == 1)
        {
            U1STAbits.OERR = 0;
        }
        else
        {
            /* Do nothing */
        }
        RxData = U1RXREG; /* Read/flush incoming Rx data */
        switch (teCurrentState)
        {
        case eBackTask_Idle:
            if ((RxData == 0xA5) && 
                    (Mcr_GetAvailable(tsUartFrame) == 0) &&
                    (Mcr_GetStartFrame(tsUartFrame) == 0)) 
            {
                /* frame begins with 0xA5, no frame available, no previous SOF */
                Mcr_SetStartFrame(tsUartFrame);
                tsUartFrame.u16Index = 0; /* reset byte counter */
                tsUartFrame.u16Timeout = TMR1_SoftwareCounterGet() + FRAME_TIMEOUT; /* Set timeout */
                teCurrentState = eBackTask_Data;
            }
            break;

        case eBackTask_Data:
            tsUartFrame.pu8Data[tsUartFrame.u16Index] = RxData; /* Load incoming data into frame buffer */
            tsUartFrame.u16Index ++;
            eRetVal = MFrameMng_RxHandler(&tsUartFrame, &tsBootMsg);
            
            if (eRetVal == eOperationSuccess)
            {
                /* Notice frame available */
                Mcr_ResetStartFrame(tsUartFrame);
                Mcr_SetAvailable(tsUartFrame);
                teCurrentState = eBackTask_Idle;
            }
            else if(eRetVal > eOperationNotAvailable)
            {
                /* Manage error code */
                Mcr_ResetStartFrame(tsUartFrame);
                Mcr_ResetAvailable(tsUartFrame);
                teCurrentState = eBackTask_Idle;
            }
            break;

        default:
            break;
        }
    }
}

void MTarget_SendFrame(tsGenericMsg* FptsTxMsg)
{
    uint8_t u8Checksum = 0;
    uint8_t pu8TxBuffer[256];
    uint16_t u16FrmLength = FptsTxMsg->u16Length + 1;
    uint16_t u16i;
    
    pu8TxBuffer[0] = 0x5A;
    pu8TxBuffer[1] = FptsTxMsg->u8ID;
    pu8TxBuffer[2] = u16FrmLength >> 8;
    pu8TxBuffer[3] = u16FrmLength;
    
    u8Checksum = pu8TxBuffer[1] + pu8TxBuffer[2] + pu8TxBuffer[3];
    
    for (u16i = 0; u16i < FptsTxMsg->u16Length; u16i++)
    {
        pu8TxBuffer[4 + u16i] = FptsTxMsg->pu8Data[u16i];
        u8Checksum += FptsTxMsg->pu8Data[u16i];
    }
    
    u8Checksum = ~u8Checksum;
    pu8TxBuffer[4 + u16i] = u8Checksum;
    ClrWdt();
    for (u16i = 0; u16i < u16FrmLength + 4; u16i++)
    {
        UART1_Write(pu8TxBuffer[u16i]);
    }
    
    FptsTxMsg->u8ID = 0;
    FptsTxMsg->u16Length = 0;
//    for (u16i = 0; u16i < MAX_FRM_LEN; u16i++)
//    {
//        FptsTxMsg->pu8Data[u16i] = 0;
//    }
    memset(FptsTxMsg->pu8Data, 0, MAX_FRM_LEN);
}

teOperationRetVal MTarget_FrameAvailable(tsGenericMsg* FptsBootMsg)
{
    teOperationRetVal teRetVal = eOperationNotAvailable;
//    uint16_t u16i = 0;
    if (Mcr_GetAvailable(tsUartFrame) == 1)
    {
        Mcr_ResetAvailable(tsUartFrame);
        teRetVal = eOperationSuccess;
        FptsBootMsg->u8ID = tsBootMsg.u8ID;
        FptsBootMsg->u16Length = tsBootMsg.u16Length;
        memcpy(FptsBootMsg->pu8Data, tsBootMsg.pu8Data, tsBootMsg.u16Length);
    }
    else
    {
        teRetVal = eOperationNotAvailable;
    }
    return teRetVal;
}
