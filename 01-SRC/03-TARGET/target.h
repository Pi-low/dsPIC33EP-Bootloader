#ifndef TARGET_H
#define TARGET_H

#include "FrameMng.h"

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
teOperationRetVal FrameAvailable(tsGenericMsg* FptsBootMsg);
void sendFrame(tsGenericMsg *FptsTxMsg);


#endif