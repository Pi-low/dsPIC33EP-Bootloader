#ifndef FRAME_MNG_H
#define FRAME_MNG_H

#include "target.h"

#define MAX_FRAMES 8

typedef struct
{
    uint8_t SOF : 1;
    uint8_t Tmout : 1;
    uint8_t Available : 1;
}tsbFlags;

typedef struct
{
    uint8_t u8ID;
    uint16_t u16Length;
    uint8_t u8Data[MAX_FRM_LEN];
    uint8_t ValidFalg;
}tsBootMsg;

typedef struct
{
    uint16_t u16BufferIndex;
    uint16_t u16Timeout;
    uint16_t u16FrameSize;
    uint8_t u8Status;
    uint8_t* pu8RawData;
    tsbFlags tsStatus;
}tsUartFrm;

typedef struct
{
    uint8_t u8ID;
    uint16_t u8MinLen;
    uint16_t u16MaxLen;
} tsFrameSize;

teOperationRetVal RxFrameHandler(tsUartFrm * UARTFrm, tsBootMsg * BootMsg);

#endif