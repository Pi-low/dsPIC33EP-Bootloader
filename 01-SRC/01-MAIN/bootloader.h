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

uint8_t serviceEcho(tsBootMsg * uartMsg);
uint8_t serviceGetInfo(tsBootMsg * uartMsg);
uint8_t serviceEraseFlash(tsBootMsg * uartMsg);
uint8_t serviceDataTransfer(tsBootMsg * uartMsg);
uint8_t serviceCheckFlash(tsBootMsg * uartMsg);
uint8_t serviceWritePin(tsBootMsg * uartMsg);
uint8_t serviceReadPin(tsBootMsg * uartMsg);

#endif