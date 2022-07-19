#include "../../mcc_generated_files/system.h"
#include "main.h"
#include "bootloader.h"
#include "BackTask.h"
#include "../02-FLAH_ROUTINES/flash_routines.h"
#include "../03-TARGET/target.h"
#include "../04-CRC/crc.h"

uint32_t ulBootFlag __attribute__((address(0x1000), persistent));

void main(void)
{
    SYSTEM_Initialize();
    InitBackTask();
    
    while(1)
    {
        ManageBackTask();
    }
}