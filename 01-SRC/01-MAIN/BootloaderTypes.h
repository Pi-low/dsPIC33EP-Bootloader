#ifndef BOOT_TYPES_H
#define BOOT_TYPES_H

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
    eService_readPin =          0x0B
}teBootServices;

typedef struct
{
    uint32_t u32BlockAddr;
    uint16_t u16BlockSize8;
    uint8_t pu8Data[256];
    uint32_t pu32Word[32];
    uint16_t u16CRC;
}DataBlock_t;


#endif