#ifndef FRAME_MNG_H
#define FRAME_MNG_H

#include <stdint.h>
#include "target.h"

#define REGISTERED_FRAMES 8

#define Mcr_ResetAvailable(tsFrm)   (tsUartFrm)(tsFrm).u8Flag &= 0xFE
#define Mcr_SetAvailable(tsFrm)     (tsUartFrm)(tsFrm).u8Flag |= 0x01
#define Mcr_GetAvailable(tsFrm)     (uint8_t)((tsUartFrm)(tsFrm).u8Flag & 1)

#define Mcr_ResetStartFrame(tsFrm)  (tsUartFrm)(tsFrm).u8Flag &= 0xFD
#define Mcr_SetStartFrame(tsFrm)    (tsUartFrm)(tsFrm).u8Flag |= 0x02
#define Mcr_GetStartFrame(tsFrm)    (uint8_t)(((tsUartFrm)(tsFrm).u8Flag >> 1) & 1)

typedef struct
{
    uint16_t u16Index;
    uint8_t pu8Data[RX_BUFFER_SIZE];
    uint8_t u8Flag;
    uint16_t u16Timeout;
}tsUartFrm;

typedef struct
{
    uint8_t u8ID;
    uint16_t u16Length;
    uint8_t pu8Data[MAX_FRM_LEN];
} tsGenericMsg;

typedef struct
{
    uint8_t u8ID;
    uint16_t u8MinLen;
    uint16_t u16MaxLen;
} tsFrameSize;

teOperationRetVal RxFrameHandler(tsUartFrm * UARTFrm, tsGenericMsg * BootMsg);

#endif