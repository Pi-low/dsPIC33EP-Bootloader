#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include "../03-TARGET/target.h"

#define BOOT_ROW_SIZE_BYTE          (256U)
#define BOOT_PAGE_SIZE_BYTE         (2048U)
#define BOOT_ROW_SIZE_WORD          (64U)
#define BOOT_PAGE_SIZE_WORD         (512U)
#define BOOT_ROW_OFFSET_ADDR        (0x80U)
#define BOOT_PAGE_OFFSET_ADDR       (0x200U)

#define ADDR_FLASH_LOGISTIC         (0x4000L)
#define ADDR_FLASH_APPLI            (0x4400L)
#define ADDR_FLASH_END              (0x55400L)

#define FLASH_APPLI_PAGES           (144U)

#define BOOTFLAG (0xC0DEFEED)
#define APPLIVALID (0xA1B2C3D4)
#define RESET() asm ("RESET");
#define StartApplication() __asm__ volatile("goto %0"::"i"(ADDR_FLASH_APPLI))

typedef struct
{
    uint32_t blockAddress;
    uint16_t blockSize;
    uint32_t *u32WordArray;
    uint16_t blockCRC;
}DataBlock_t;

enum
{
    eService_gotoBoot =         0x01,
    eService_echo =             0x02,
    eService_getInfo =          0x03,
    eService_eraseFlash =       0x04,
    eService_dataTransfer =     0x05,
    eService_checkFlash =       0x06,
    eService_writePin =         0x0A,
    eService_readPin =          0x0B
};

enum
{
    eBootStandbyState =         0x01,
    eBootLoadingState =         0x02
};

enum
{
    eMaskError_UnknownParameter =           0x10,
    eMaskError_IncorrectFrameLength =       0x20,
    eMaskError_FlashRoutineProblem =        0x30
};

uint8_t serviceEcho(UARTmsg_t * uartMsg);
uint8_t serviceGetInfo(UARTmsg_t * uartMsg);
uint8_t serviceEraseFlash(UARTmsg_t * uartMsg);
uint8_t serviceDataTransfer(UARTmsg_t * uartMsg);
uint8_t serviceCheckFlash(UARTmsg_t * uartMsg);
uint8_t serviceWritePin(UARTmsg_t * uartMsg);
uint8_t serviceReadPin(UARTmsg_t * uartMsg);

#endif