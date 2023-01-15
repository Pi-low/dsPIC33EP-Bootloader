#ifndef BOOT_TYPES_H
#define BOOT_TYPES_H

#include <stdint.h>

/* Max frame length:
 * - Block NVM address : 3 bytes
 * - data : up to 256 bytes
 * - CRC : 2 bytes
 * - Frame checksum : 1 byte
 */
#define MAX_FRM_LEN (262U)
#define RX_BUFFER_SIZE (266U)
#define FRAME_TIMEOUT (60U) /* 60ms */

typedef enum
{
    eOperationSuccess =         0,
    eOperationNotAvailable =    1,
    eOperationNotAllowed =      2,
    eOperationFail =            3,
    eBadChecksum =              4,
    eBadFrameLength =           5,
    eUnknownFrameID =           6,
    eFrameTimeout =             7,
    eFlashEraseError =          8,
    eMemoryNotBlanked =         9,
    eBadBlockAddr =             10,
    eBadCRCBlock =              11,
    eFlashWriteError =          12,
    eAppliCheckError =          13,
    eBootSessionTimeout =       14
}teOperationRetVal;

typedef enum
{
    eService_gotoBoot =         0x01,
    eService_echo =             0x02,
    eService_getInfo =          0x03,
    eService_eraseFlash =       0x04,
    eService_dataTransfer =     0x05,
    eService_checkFlash =       0x06,
    eService_writePin =         0x0A,
    eService_readPin =          0x0B,
}teBootServices;

typedef enum
{
    eStateTransition,
    eStateIdle,
    eStateFlash
}teMainStates;

typedef struct
{
    uint32_t u32BlockAddr;
    uint16_t u16BlockSize8;
    uint8_t pu8Data[256];
    uint32_t pu32Word[32];
    uint16_t u16CRC;
}DataBlock_t;

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
    uint16_t u16MinLen;
    uint16_t u16MaxLen;
} tsFrameSize;


#endif