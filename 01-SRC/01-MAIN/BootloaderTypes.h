#ifndef BOOT_TYPES_H
#define BOOT_TYPES_H

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
    uint8_t u8ID;
    uint16_t u16Length;
    uint8_t *pu8Data;
}tsBootMsg;

typedef union
{
    struct
    {
        uint8_t SOF : 1;
        uint8_t Tmout : 1;
        uint8_t Available : 1;
    }bit;
}tuFFlags;

typedef struct
{
    uint16_t u16BufferIndex;
    uint16_t u16Timeout;
    uint8_t u8Status;
    uint8_t* pu8RawData;
}tsUartFrm;


#endif