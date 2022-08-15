#include "../../mcc_generated_files/system.h"
#include "../../mcc_generated_files/memory/flash.h"
#include "../01-MAIN/main.h"
#include "../01-MAIN/bootloader.h"
#include "flash_routines.h"

uint32_t readAppFlag(void)
{
    uint32_t u32;
    uint16_t _tmp;
    
    _tmp = FLASH_ReadWord16(0x4000);
    u32 = _tmp;
    _tmp = FLASH_ReadWord16(0x4002);
    u32 |= (uint32_t)_tmp << 16L;
    
    return u32;
}

void readLogisticChar(uint8_t buffer[])
{
    uint8_t i, j;
    uint16_t u16;
    j = 0;
    for (i = 0; i < 32; i++)
    {
        u16 = FLASH_ReadWord16(0x4080 + (i * 2));
        buffer[j] = u16 & 0x00FF;
        j++;
        buffer[j] = (u16 >> 8) & 0x00FF;
        j++;
    }
}

uint16_t readSWVersion(void)
{
    uint16_t u16;
    u16 = FLASH_ReadWord16(0x40C0);
    return u16;
}

uint8_t extractDataBlock(UARTmsg_t *RxMsg, datablock_t *Datablock)
{
    
}