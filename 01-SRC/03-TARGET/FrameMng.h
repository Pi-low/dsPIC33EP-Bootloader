#ifndef FRAME_MNG_H
#define FRAME_MNG_H

#include <stdint.h>
#include "../01-MAIN/BootloaderTypes.h"

#define REGISTERED_FRAMES 8

#define Mcr_ResetAvailable(tsFrm)   ((tsFrm).u8Flag &= 0xFE)
#define Mcr_SetAvailable(tsFrm)     ((tsFrm).u8Flag |= 0x01)
#define Mcr_GetAvailable(tsFrm)     (uint8_t)((tsFrm).u8Flag & 1)

#define Mcr_ResetStartFrame(tsFrm)  ((tsFrm).u8Flag &= 0xFD)
#define Mcr_SetStartFrame(tsFrm)    ((tsFrm).u8Flag |= 0x02)
#define Mcr_GetStartFrame(tsFrm)    (uint8_t)(((tsFrm).u8Flag >> 1) & 1)

teOperationRetVal RxFrameHandler(tsUartFrm * UARTFrm, tsGenericMsg * BootMsg);

#endif