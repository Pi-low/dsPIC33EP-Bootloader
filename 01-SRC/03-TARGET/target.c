#include "../../mcc_generated_files/system.h"
#include "../../mcc_generated_files/uart1.h"
#include "../../mcc_generated_files/tmr1.h"
#include "../01-MAIN/BootloaderTypes.h"
#include "../01-MAIN/bootloader.h"
#include "target.h"

static teBackTaskStates teCurrentState;
static uint8_t pu8Buffer[RX_BUFFER_SIZE];
static tsUartFrm tsUartFrame;

static uint8_t CheckHeader (void);
static uint8_t CheckPayloadLength (void);
static uint8_t CheckIdleState (uint8_t DataIN);


void InitBackTask(void)
{
    uint16_t i;
    teCurrentState = eBackTask_Idle;
    tsUartFrame.pu8RawData = pu8Buffer;
    tsUartFrame.u16BufferIndex = 0;
    tsUartFrame.tuStatus = 0;
}

void ManageBackTask(void)
{
    uint8_t RxData;
    if (tsUartFrame.tuStatus != 0 && (TMR1_SoftwareCounterGet() > TimeMng.Timeout))
    {
        u8_UART_State = eBackTask_Idle;
        TimeMng.SOF_Flag = 0;
    }
    else
    {
        /* Continue */
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
        switch (u8_UART_State)
        {
        case eBackTask_Idle:
            u8_UART_State = CheckIdleState(RxData);
            break;

        case eBackTask_StartOfFrame:
            Checksum += RxData;
            DataBuffer[BufferIndex] = RxData; /* Load incoming data into frame buffer */
            BufferIndex ++;
            u8_UART_State = CheckHeader();
            break;

        case eBackTask_Data:
            Checksum += RxData;
            DataBuffer[BufferIndex] = RxData; /* Load incoming data into frame buffer */
            BufferIndex ++;
            if (BufferIndex == FrameLength + 3)
            {
                /* End of frame */
                u8_UART_State = CheckPayloadLength();
            }
            
            break;

        default:
            break;
        }
    }
    else
    {
        /* Do nothing */
    }
}

uint8_t CheckIdleState (uint8_t DataIN)
{
    uint16_t CurrentTime;
    uint8_t Res = eBackTask_Idle;
    if (DataIN == 0xA5) /* frame begins with 0xA5 */
    {
        CurrentTime = TMR1_SoftwareCounterGet();
        u8_UART_State = eBackTask_StartOfFrame; /* Start state machine */
        BufferIndex = 0; /* reset byte counter */
        Checksum = 0; /* Reset checksum */
        Res = eBackTask_StartOfFrame;
        TimeMng.SOF_Flag = 1;
        TimeMng.SOF_Timestamp = CurrentTime;
        TimeMng.Timeout = CurrentTime + FRAME_TIMEOUT;
    }
    else
    {
        /* Do nothing */
    }
    return Res;
}

uint8_t CheckHeader (void)
{
    uint8_t res = eBackTask_StartOfFrame;
    if (BufferIndex >= 3)
    {
        FrameLength = (DataBuffer[1] << 8) & 0xFF00;
        FrameLength |= DataBuffer[2] & 0x00FF;
        if ((FrameLength > 0) && (FrameLength <= MAX_FRM_LEN))
        {
            /* frame length in range */
            res = eBackTask_Data;
        }
        else
        {
            /* Frame too short or too long */
            res = eBackTask_Idle;
            TimeMng.SOF_Flag = 0;
        }
    }
    else
    {
        /* Continue */
    }
    return res;
}

uint8_t FrameAvailable(UARTmsg_t *ReceiveFrame)
{
    uint16_t u8 = 0;
    uint16_t i = 0;
    if (FrameStatus == 1)
    {
        u8 = 1;
        for (i = 0; i < FrameLength - 1; i++)
        {
            ReceiveFrame->Data[i] = DataBuffer[i + 3];
        }
        
        ReceiveFrame->ID =  DataBuffer[0];
        ReceiveFrame->Length = FrameLength - 1;
        ReceiveFrame->Timestamp = RxFrameTimestamp;
        
        FrameStatus = 0;
    }
    else
    {
        /* Do nothing */
    }
    
    return u8;
}

void constructFrame(uint8_t FrameID, uint8_t *Data, uint16_t BuffLength, UARTmsg_t *MakeFrame)
{
    uint16_t i;
    MakeFrame->ID = FrameID;
    MakeFrame->Length = BuffLength;
    MakeFrame->Data = Data;
}

void sendFrame(UARTmsg_t *TxFrame)
{
    uint8_t Checksum = 0;
    uint8_t TxBuffer[100];
    uint16_t FrmLength = TxFrame->Length + 1;
    uint16_t i;
    
    TxBuffer[0] = 0x5A;
    TxBuffer[1] = TxFrame->ID;
    TxBuffer[2] = FrmLength >> 8;
    TxBuffer[3] = FrmLength;
    
    Checksum = TxBuffer[1] + TxBuffer[2] + TxBuffer[3];
    
    for (i = 0; i < TxFrame->Length; i++)
    {
        TxBuffer[4 + i] = TxFrame->Data[i];
        Checksum += TxFrame->Data[i];
    }
    
    Checksum = ~Checksum;
    TxBuffer[4 + i] = Checksum;
    
    for (i = 0; i < FrmLength + 4; i++)
    {
        UART1_Write(TxBuffer[i]);
    }
}
