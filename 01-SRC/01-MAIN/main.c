#include "../../mcc_generated_files/system.h"
#include "main.h"
#include "bootloader.h"
#include "../02-FLAH_ROUTINES/flash_routines.h"
#include "../03-TARGET/target.h"
#include "../04-CRC/crc.h"


void main(void)
{
    SYSTEM_Initialize();
    
    while(1)
    {
        
    }
}