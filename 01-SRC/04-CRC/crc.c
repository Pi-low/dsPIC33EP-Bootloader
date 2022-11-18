#include "../../mcc_generated_files/system.h"
#include "../01-MAIN/BootloaderTypes.h"
#include "../01-MAIN/bootloader.h"
#include "crc.h"

void initCRCengine(uint16_t Poly)
{
    CRCCON1 = 0x1008;
    CRCCON2 = 0x070F; /* Data length = 8, Poly length = 16 */
    CRCXORL = Poly;
    CRCXORH = 0;
}

uint16_t RunCRC(uint8_t buffer[], uint16_t buflen)
{
    uint16_t i;
    CRCCON1bits.CRCGO = 1;
    for (i = 0; i < buflen; i++)
    {
        CRCDATL = buffer[i];
        while (CRCCON1bits.CRCFUL == 1); /* Wait FIFO to empty to continue */
    }
    CRCCON1bits.CRCGO = 0;
    return 0;
}