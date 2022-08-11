#ifndef TARGET_H
#define TARGET_H

#define MAX_FRM_LEN 261u
#define INTER_FRAME_TIMEOUT ((uint16_t)250)

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
    uint16_t CRC;
    uint8_t Status;
}UARTmsg_t;

typedef struct
{
    uint8_t SOF_Flag;
    uint16_t SOF_Timestamp;
    uint16_t Timeout;
}TimeProtocol_t;



void InitBackTask(void);
void ManageBackTask(void);
uint8_t FrameAvailable(void);
void AcknowledgeFrame(void);
void constructFrame(uint8_t FrameID, uint8_t *Paylod, uint16_t PayloadLength, UARTmsg_t *MakeFrame);
void sendFrame(UARTmsg_t *TxFrame);
void constructErrFrame(uint8_t FrameID, uint8_t CodeErr, UARTmsg_t *MakeFrame);
extern UARTmsg_t RxMsg, TxMsg;

#endif