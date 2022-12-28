#include <stdlib.h>
#include "../../mcc_generated_files/system.h"
#include "../../mcc_generated_files/uart1.h"
#include "../../mcc_generated_files/tmr1.h"
#include "../01-MAIN/BootloaderTypes.h"
#include "FrameMng.h"
#include "target.h"

static teBackTaskStates teCurrentState;
static tsGenericMsg tsBootMsg;
static tsUartFrm tsUartFrame;

void InitBackTask(void)
{
    teCurrentState = eBackTask_Idle;
}

void ManageBackTask(void)
{
    tsGenericMsg tsReturnMsg;
    uint8_t RxData;
    teOperationRetVal eRetVal;
    if ((Mcr_GetStartFrame(tsUartFrame) == 1) && 
            (TMR1_SoftwareCounterGet() > tsUartFrame.u16Timeout) && 
            (Mcr_GetAvailable(tsUartFrame) == 0))
    {
        /* Frame timeout */
        teCurrentState = eBackTask_Idle;
        Mcr_ResetStartFrame(tsUartFrame);
        tsReturnMsg.u8ID = 0x90;
        tsReturnMsg.u16Length = 1;
        tsReturnMsg.pu8Data[0] = eFrameTimeout;
        sendFrame(&tsReturnMsg);
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
            eRetVal = RxFrameHandler(&tsUartFrame, &tsBootMsg);
            
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

void sendFrame(tsGenericMsg* FptsTxMsg)
{
    uint8_t u8Checksum = 0;
    uint8_t pu8TxBuffer[100];
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
    
    for (u16i = 0; u16i < u16FrmLength + 4; u16i++)
    {
        UART1_Write(pu8TxBuffer[u16i]);
    }
    
    FptsTxMsg->u8ID = 0;
    FptsTxMsg->u16Length = 0;
    for (u16i = 0; u16i < MAX_FRM_LEN; u16i++)
    {
        FptsTxMsg->pu8Data[u16i] = 0;
    }
}

teOperationRetVal FrameAvailable(tsGenericMsg* FptsBootMsg)
{
    teOperationRetVal teRetVal = eOperationNotAvailable;
    uint16_t u16i = 0;
    if (Mcr_GetAvailable(tsUartFrame) == 1)
    {
        Mcr_ResetAvailable(tsUartFrame);
        teRetVal = eOperationSuccess;
        FptsBootMsg->u8ID = tsBootMsg.u8ID;
        FptsBootMsg->u16Length = tsBootMsg.u16Length;
        for (u16i = 0; u16i < tsBootMsg.u16Length; u16i++)
        {
            FptsBootMsg->pu8Data[u16i] = tsBootMsg.pu8Data[u16i];
        }
    }
    else
    {
        teRetVal = eOperationNotAvailable;
    }
    return teRetVal;
}

void BufCopy(uint8_t* pu8Dest, const uint8_t* pu8Src, uint16_t u16Size)
{
    uint16_t u16i;
    for (u16i = 0; u16i < u16Size; u16i++)
    {
        *(pu8Dest + u16i) = *(pu8Src + u16i);
    }
}
