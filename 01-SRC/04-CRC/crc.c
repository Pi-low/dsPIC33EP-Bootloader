#include "../../mcc_generated_files/system.h"
#include "../01-MAIN/BootloaderTypes.h"
#include "../01-MAIN/bootloader.h"
#include "crc.h"

void initCRCengine(uint16_t u16Poly)
{
    CRCCON1 = 0x1008;
    CRCCON2 = 0x070F; /* Data length = 8, Poly length = 16 */
    CRCXORL = u16Poly;
    CRCXORH = 0;
}

uint16_t RunCRC(uint8_t pu8Data[], uint16_t u16buflen)
{
    uint16_t u16i;
    CRCCON1bits.CRCGO = 1;
    for (u16i = 0; u16i < u16buflen; u16i++)
    {
        CRCDATL = pu8Data[u16i];
        while (CRCCON1bits.CRCFUL == 1); /* Wait FIFO to empty to continue */
    }
    CRCCON1bits.CRCGO = 0;
    return 0;
}