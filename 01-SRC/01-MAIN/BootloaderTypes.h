#ifndef BOOT_TYPES_H
#define BOOT_TYPES_H

#include <stdint.h>
#include "bootloader.h"

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
    eForbiddenBlockAddr,
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
    eService_readPin =          0x0B
}teBootServices;

typedef struct
{
    uint32_t u32BlockAddr;
    uint16_t u16BlockSize;
    uint32_t *pu32WordArray;
    uint16_t u16BlockCRC;
}DataBlock_t;


#endif