#ifndef BOOT_TYPES_H
#define BOOT_TYPES_H

#include <stdint.h>
#include "bootloader.h"
#include "../03-TARGET/target.h"

typedef enum
{
    eOperationSuccess = 0,
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
    uint32_t blockAddress;
    uint16_t blockSize;
    uint32_t *u32WordArray;
    uint16_t blockCRC;
}DataBlock_t;


#endif