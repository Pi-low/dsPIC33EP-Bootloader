#include "../../mcc_generated_files/system.h"
#include "../../mcc_generated_files/uart1.h"
#include "../../mcc_generated_files/tmr1.h"
#include "../01-MAIN/main.h"
#include "../01-MAIN/bootloader.h"
#include "target.h"

static uint8_t UART_States;
static uint8_t DataBuffer[MAX_FRM_LEN];
static uint16_t BufferIndex;

static uint8_t CheckHeader (void);
static uint8_t CheckPayloadLength (void);
static uint8_t CheckIdleState (uint8_t DataIN);

static TimeProtocol_t TimeMng;

UARTmsg_t RxMsg, TxMsg;

void InitBackTask(void)
{
    uint16_t i;
    UART_States = eBackTask_Idle;
    RxMsg.Data = DataBuffer;
    BufferIndex = 0;
    for (i = 0; i < MAX_FRM_LEN; i++)
    {
        DataBuffer[i] = 0;
    }
    TimeMng.SOF_Flag = 0;
    TimeMng.SOF_Timestamp = 0;
}

void ManageBackTask(void)
{
    uint8_t RxData;
    if (TimeMng.SOF_Flag != 0 && (TMR1_SoftwareCounterGet() > TimeMng.Timeout))
    {
        UART_States = eBackTask_Idle;
        TimeMng.SOF_Flag = 0;
    }
    if (U1STAbits.URXDA == 1)
    {
        if (U1STAbits.OERR == 1)
        {
            U1STAbits.OERR = 0;
        }
        RxData = U1RXREG; /* Read/flush incoming Rx data */
        switch (UART_States)
        {
        case eBackTask_Idle:
            UART_States = CheckIdleState(RxData);
            break;

        case eBackTask_StartOfFrame:
            DataBuffer[BufferIndex] = RxData; /* Load incoming data into frame buffer */
            BufferIndex ++;
            UART_States = CheckHeader();
            TimeMng.Timeout += INTER_FRAME_TIMEOUT;
            break;

        case eBackTask_Data:
            DataBuffer[BufferIndex] = RxData; /* Load incoming data into frame buffer */
            BufferIndex ++;
            UART_States = CheckPayloadLength();
            TimeMng.Timeout += INTER_FRAME_TIMEOUT;
            break;

        default:
            break;
        }
    }
}

uint8_t CheckIdleState (uint8_t DataIN)
{
    uint16_t CurrentTime;
    uint8_t Res = eBackTask_Idle;
    if (DataIN == 0xA5) /* Proper frame begins with 0xA5 */
    {
        CurrentTime = TMR1_SoftwareCounterGet();
        UART_States = eBackTask_StartOfFrame; /* Start state machine */
        BufferIndex = 0;
        Res = eBackTask_StartOfFrame;
        TimeMng.SOF_Flag = 1;
        TimeMng.SOF_Timestamp = CurrentTime;
        TimeMng.Timeout = CurrentTime + INTER_FRAME_TIMEOUT;
    }
    return Res;
}

uint8_t CheckHeader (void)
{
    uint8_t res = eBackTask_StartOfFrame;
    uint16_t length = 0;
    if (DataBuffer[0] == eService_gotoBoot || DataBuffer[0] == eService_echo ||
            DataBuffer[0] == eService_getInfo || DataBuffer[0] == eService_eraseFlash ||
            DataBuffer[0] == eService_dataTransfer || DataBuffer[0] == eService_checkFlash ||
            DataBuffer[0] == eService_writePin || DataBuffer[0] == eService_readPin )
    {
        if (BufferIndex >= 3)
        {
            length = (DataBuffer[1] << 8) & 0xFF00;
            length |= DataBuffer[2] & 0x00FF;
            if (length <= MAX_FRM_LEN)
            {
                res = eBackTask_Data; /* frame length in range */
                RxMsg.Length = length;
                RxMsg.ID = DataBuffer[0];
            }
            else
            {
                /* Frame too long */
                res &= eBackTask_Idle;
                TimeMng.SOF_Flag = 0;
            }
        }
        else
        {
            /* Continue */
        }
    }
    else
    {
        /* Unknown ID */
        res = eBackTask_Idle;
    }
    return res;
}

uint8_t CheckPayloadLength (void)
{
    uint16_t CRC16;
    uint8_t Res = eBackTask_Data;
    if (BufferIndex >= RxMsg.Length + 3)
    {
        CRC16 = DataBuffer[BufferIndex - 1];
        CRC16 |= (DataBuffer[BufferIndex - 2] << 8) & 0xFF00;
        if (CRC16 == 0xEC2C)
        {
            Res = eBackTask_Idle;
            RxMsg.Status = 1;
        }
        else
        {
            /* Frame CRC error */
            Res = eBackTask_Idle;
            RxMsg.Status = 1 << 1;
        }
        TimeMng.SOF_Flag = 0;
    }
    return Res;
}

uint8_t FrameAvailable(void)
{
    uint8_t Res = 0;
    if(RxMsg.Status == 1)
    {
      Res = 1;  
    }
    return Res;
}

void AcknowledgeFrame(void)
{
    RxMsg.Status = 0;
}

void constructFrame(uint8_t FrameID, uint8_t *Data, uint16_t PayloadLength, UARTmsg_t *MakeFrame)
{
    MakeFrame->ID = FrameID;
    MakeFrame->Length = PayloadLength;
    MakeFrame->Data = Data;
    MakeFrame->Status = 0;
}

void sendFrame(UARTmsg_t *TxFrame)
{
    uint8_t TxBuffer[MAX_FRM_LEN];
    uint16_t FrmLength = TxFrame->Length + 2;
    uint16_t i;
    TxBuffer[0] = 0x5A;
    TxBuffer[1] = TxFrame->ID;
    TxBuffer[2] = (FrmLength >> 8) & 0x00FF;
    TxBuffer[3] = FrmLength & 0x00FF;
    for (i = 0; i < TxFrame->Length; i++)
    {
        TxBuffer[4+i] = TxFrame->Data[i];
    }
    for (i = 0; i < TxFrame->Length + 6; i++)
    {
        UART1_Write(TxBuffer[i]);
    }
}

void constructErrFrame(uint8_t FrameID, uint8_t CodeErr, UARTmsg_t *MakeFrame)
{
    MakeFrame->ID = FrameID;
    MakeFrame->Length = 1;
    MakeFrame->Data = CodeErr;
}