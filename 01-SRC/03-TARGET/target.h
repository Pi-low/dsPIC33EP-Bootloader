#ifndef TARGET_H
#define TARGET_H

/* Max frame length:
 * - Counter : 1 byte
 * - Block NVM address : 3 bytes
 * - data : up to 255 bytes
 * - CRC : 2 bytes
 * - Frame checksum : 1 byte
 */
#define MAX_FRM_LEN (261U)
#define RX_BUFFER_SIZE (264U)
#define INTER_FRAME_TIMEOUT ((uint16_t)20)

enum
{
    eBackTask_Idle = 0,
    eBackTask_StartOfFrame = 1,
    eBackTask_Data = 2
};

typedef struct
{
    uint8_t ID;
    uint16_t Length;
    uint8_t *Data;
    uint16_t Timestamp;
}UARTmsg_t;

typedef struct
{
    uint8_t SOF_Flag;
    uint16_t SOF_Timestamp;
    uint16_t Timeout;
}TimeProtocol_t;



void InitBackTask(void);
void ManageBackTask(void);
uint8_t FrameAvailable(UARTmsg_t *ReceiveFrame);
void constructFrame(uint8_t FrameID, uint8_t *Paylod, uint16_t PayloadLength, UARTmsg_t *MakeFrame);
void sendFrame(UARTmsg_t *TxFrame);

#endif