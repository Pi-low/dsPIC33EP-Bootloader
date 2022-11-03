#include "../../mcc_generated_files/system.h"
#include "../01-MAIN/main.h"
#include "../01-MAIN/bootloader.h"
#include "crc.h"

void initCRCengine(uint16_t Poly)
{
    CRCCON1 = 0x1008;
    CRCCON2 = 0x070F;
    CRCXORL = Poly;
    CRCXORH = 0;
}

uint16_t runCRCOnBuffer(uint8_t buffer[], uint16_t buflen)
{
    uint16_t i;
    for (i = 0; i < buflen; i++)
    {
        CRCDATL = buffer[i];
    }
    
    return 0;
}