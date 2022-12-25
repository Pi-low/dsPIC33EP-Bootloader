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
#define FRAME_TIMEOUT (1000U) /* 1000ms */

typedef enum
{
    eOperationSuccess = 0,
    eOperationNotAvailable,
    eOperationFail,
    eBadChecksum,
    eBadFrameLength,
    eUnknownFrameID,
    eFrameTimeout,
    eFlashEraseError,
    eBadBlockAddr,
    eBadCRCBlock,
    eFlashWriteError,
    eAppliCheckError
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
#ifndef _IS_RELEASE
    eService_TestCrc =          0x0C
#endif
}teBootServices;

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