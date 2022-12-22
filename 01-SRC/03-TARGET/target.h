#ifndef TARGET_H
#define TARGET_H

#include <stdint.h>
#include "../01-MAIN/BootloaderTypes.h"

typedef enum
{
    eBackTask_Idle = 0,
    eBackTask_Data = 1
}teBackTaskStates;

void InitBackTask(void);
void ManageBackTask(void);
teOperationRetVal FrameAvailable(tsGenericMsg* FptsBootMsg);
void sendFrame(tsGenericMsg *FptsTxMsg);
void BufCopy(uint8_t* pu8Dest, const uint8_t* pu8Src, uint16_t u16Size);

#endif