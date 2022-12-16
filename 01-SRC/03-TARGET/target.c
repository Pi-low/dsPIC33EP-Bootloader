#include "../../mcc_generated_files/system.h"
#include "../../mcc_generated_files/uart1.h"
#include "../../mcc_generated_files/tmr1.h"
#include "../01-MAIN/BootloaderTypes.h"
#include "../01-MAIN/bootloader.h"
#include "target.h"

static teBackTaskStates teCurrentState;
static uint8_t pu8Buffer[RX_BUFFER_SIZE];
static tsUartFrm tsUartFrame;
static tsBootMsg tsBootMessage;

static uint8_t CheckHeader (void);
static uint8_t CheckPayloadLength (void);
static uint8_t CheckIdleState (uint8_t DataIN);


void InitBackTask(void)
{
    uint16_t i;
    teCurrentState = eBackTask_Idle;
    tsUartFrame.pu8RawData = pu8Buffer;
    tsUartFrame.u16BufferIndex = 0;
    tsUartFrame.tsStatus = 0;
}

void ManageBackTask(void)
{
    uint8_t RxData;
    if (tsUartFrame.tsStatus.SOF != 0 && (TMR1_SoftwareCounterGet() > tsUartFrame.u16Timeout))
    {
        /* Frame timeout */
        teCurrentState = eBackTask_Idle;
        tsUartFrame.tsStatus.SOF = 0;
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
            teCurrentState = CheckIdleState(RxData); /* escape SOF byte */
            break;

        case eBackTask_StartOfFrame:
            pu8Buffer[tsUartFrame.u16BufferIndex] = RxData; /* Load incoming data into frame buffer */
            tsUartFrame.u16BufferIndex ++;
            teCurrentState = CheckHeader();
            break;

        case eBackTask_Data:
            pu8Buffer[tsUartFrame.u16BufferIndex] = RxData; /* Load incoming data into frame buffer */
            tsUartFrame.u16BufferIndex ++;
            if (tsUartFrame.u16BufferIndex > tsUartFrame.u16FrameSize)
            {
                /* End of frame */
                teCurrentState = eBackTask_Idle;
                RxFrameHandler(&tsUartFrame, &tsBootMessage);
            }
            
            break;

        default:
            break;
        }
    }
}

teBackTaskStates CheckIdleState (uint8_t DataIN)
{
    teBackTaskStates RetVal = eBackTask_Idle;
    if (DataIN == 0xA5) /* frame begins with 0xA5 */
    {
        teCurrentState = eBackTask_StartOfFrame; /* Start state machine */
        tsUartFrame.u16BufferIndex = 0; /* reset byte counter */
        tsUartFrame.tsStatus.SOF = 1;
        tsUartFrame.u16Timeout = TMR1_SoftwareCounterGet() + FRAME_TIMEOUT;
        RetVal = eBackTask_StartOfFrame;
    }
    return RetVal;
}

teBackTaskStates CheckHeader (void)
{
    teBackTaskStates RetVal = eBackTask_Data;
    uint16_t u16Length;
    if (tsUartFrame.u16BufferIndex >= 3)
    {
        u16Length = (pu8Buffer[1] << 8) & 0xFF00;
        u16Length |= pu8Buffer[2] & 0x00FF;
        tsUartFrame.u16FrameSize = u16Length + 3;
        if ((u16Length == 0) || (u16Length > MAX_FRM_LEN))
        {
            /* frame length is too long */
            RetVal = eBackTask_Idle;
            tsUartFrame.tsStatus.SOF = 0;
        }
    }
    return RetVal;
}

void constructFrame(uint8_t FrameID, uint8_t *Data, uint16_t BuffLength, tsUartFrm *TxMsg)
{
    uint16_t i;
    TxMsg->pu8RawData = FrameID;
    TxMsg->u16FrameSize = BuffLength;
}

void sendFrame(tsBootMsg *TxMsg)
{
    uint8_t Checksum = 0;
    uint8_t TxBuffer[100];
    uint16_t FrmLength = TxMsg->u16Length + 1;
    uint16_t i;
    
    TxBuffer[0] = 0x5A;
    TxBuffer[1] = TxMsg->u8ID;
    TxBuffer[2] = FrmLength >> 8;
    TxBuffer[3] = FrmLength;
    
    Checksum = TxBuffer[1] + TxBuffer[2] + TxBuffer[3];
    
    for (i = 0; i < TxMsg->u16Length; i++)
    {
        TxBuffer[4 + i] = TxMsg->u8Data[i];
        Checksum += TxMsg->u8Data[i];
    }
    
    Checksum = ~Checksum;
    TxBuffer[4 + i] = Checksum;
    
    for (i = 0; i < FrmLength + 4; i++)
    {
        UART1_Write(TxBuffer[i]);
    }
}

void FrameAvailable(tsBootMsg* FpBootMsg)
{
    
}
