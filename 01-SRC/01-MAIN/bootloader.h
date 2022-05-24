#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#define BOOT_ROW_SIZE_BYTE          (256U)
#define BOOT_PAGE_SIZE_BYTE         (2048U)
#define BOOT_ROW_SIZE_WORD          (64U)
#define BOOT_PAGE_SIZE_WORD         (512U)
#define BOOT_ROW_OFFSET_ADDR        (0x80)
#define BOOT_PAGE_OFFSET_ADDR       (0x200U)

#define ADDR_FLASH_LOGISTIC         (0x4000L)
#define ADDR_FLASH_APPLI            (0x4200L)
#define ADDR_FLASH_END              (0x55600L)

#define RESET() asm ("RESET");

typedef struct
{
    uint32_t blockAddress;
    uint16_t blockSize;
    uint8_t *blockDataArray;
    uint16_t blockCRC;
}datablock_t;

#endif