#include "../../mcc_generated_files/system.h"
#include "BackTask.h"
#include "../03-TARGET/target.h"
#include "../../mcc_generated_files/uart1.h"

static uint8_t UART_States;
static uint8_t UART_Data[256];
static uint8_t DataBuffer[MAX_FRM_LEN];
static uint16_t BufferIndex;

static uint8_t CheckHeader (void);
static uint8_t CheckPayloadLength (void);
static uint8_t CheckIdleState (uint8_t DataIN);

UARTmsg_t RxMsg, TxMsg;

void InitBackTask(void)
{
    uint16_t i;
    UART_States = eBackTask_Idle;
    BufferIndex = 0;
    for (i = 0; i < MAX_FRM_LEN; i++)
    {
        DataBuffer[i] = 0;
    }
}

void ManageBackTask(void)
{
    uint8_t RxData;
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
            break;

        case eBackTask_Data:
            DataBuffer[BufferIndex] = RxData; /* Load incoming data into frame buffer */
            BufferIndex ++;
            UART_States = CheckPayloadLength();
            break;

        default:
            break;
        }
    }
}

uint8_t CheckIdleState (uint8_t DataIN)
{
    uint8_t Res = eBackTask_Idle;
    if (DataIN == 0xA5) /* Proper frame begins with 0xA5 */
    {
        UART_States = eBackTask_StartOfFrame; /* Start state machine */
        BufferIndex = 0;
        Res = eBackTask_StartOfFrame;
    }
    return Res;
}

uint8_t CheckHeader (void)
{
    uint8_t res = eBackTask_StartOfFrame;
    uint16_t length = 0;
    if (DataBuffer[0] == 0x01 || DataBuffer[0] == 0x02 ||
            DataBuffer[0] == 0x03 || DataBuffer[0] == 0x04 ||
            DataBuffer[0] == 0x05)
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
        if (CRC16 == 0xFEED)
        {
            
        }
        else
        {
            /* Frame CRC error */
            Res = eBackTask_Idle;
        }
        Res = eBackTask_Idle;
        RxMsg.Status = 1;
    }
    return Res;
}