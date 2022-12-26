#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include <stdint.h>
#include "BootloaderTypes.h"

#define BOOT_ROW_SIZE_BYTE          (256)
#define BOOT_PAGE_SIZE_BYTE         (2048)
#define BOOT_ROW_SIZE_WORD          (64)
#define BOOT_PAGE_SIZE_WORD         (512)
#define BOOT_ROW_OFFSET_ADDR        (0x80)
#define BOOT_PAGE_OFFSET_ADDR       (0x200)

#define ADDR_FLASH_LOGISTIC         (0x200)
#define ADDR_FLASH_BOOT             (0x400)
#define ADDR_FLASH_APPLI            (0x4000)
#define ADDR_FLASH_END              (0x55400)

#define FLASH_APPLI_PAGES           ((ADDR_FLASH_END - ADDR_FLASH_APPLI) / (BOOT_PAGE_SIZE_WORD * 2))
#define FLASH_LOGISTIC_CHAR_SIZE    (64)

#define BOOTFLAG (0xC0DEFEED)
#define APPLIVALID (0xA1B2C3D4)
#define RESET() asm ("RESET")
#define StartApplication() __asm__ volatile("goto %0"::"i"(ADDR_FLASH_APPLI))

#define ADDR_APPL_FLAG      (0x200)
#define ADDR_APPL_DESC      (0x280)
#define ADDR_APPL_VERSION   (0x300)

teOperationRetVal serviceEcho(tsGenericMsg* FptsGenMsg);
teOperationRetVal serviceGetInfo(tsGenericMsg* FptsGenMsg);
teOperationRetVal serviceEraseFlash(tsGenericMsg* FptsGenMsg);
teOperationRetVal serviceDataTransfer(tsGenericMsg* FptsGenMsg);
teOperationRetVal serviceCheckFlash(tsGenericMsg* FptsGenMsg);
teOperationRetVal serviceWritePin(tsGenericMsg* FptsGenMsg);
teOperationRetVal serviceReadPin(tsGenericMsg* FptsGenMsg);
teOperationRetVal createDataBlock(tsGenericMsg * FptsGenMsg, DataBlock_t * FptsBlock);
#ifndef _IS_RELEASE
teOperationRetVal serviceCRC(tsGenericMsg* FptsGenMsg);
#endif
#endif