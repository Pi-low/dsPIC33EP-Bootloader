#ifndef TARGET_H
#define TARGET_H

/* Max frame length:
 * - Block NVM address : 3 bytes
 * - data : up to 256 bytes
 * - CRC : 2 bytes
 * - Frame checksum : 1 byte
 */
#define MAX_FRM_LEN (262U)
#define RX_BUFFER_SIZE (266U)
#define FRAME_TIMEOUT (1000U) /* 1000ms */

typedef enum
{
    eBackTask_Idle = 0,
    eBackTask_StartOfFrame = 1,
    eBackTask_Data = 2
}teBackTaskStates;

void InitBackTask(void);
void ManageBackTask(void);
uint8_t FrameAvailable(UARTmsg_t *ReceiveFrame);
void constructFrame(uint8_t FrameID, uint8_t *Paylod, uint16_t PayloadLength, UARTmsg_t *MakeFrame);
void sendFrame(UARTmsg_t *TxFrame);

#endif