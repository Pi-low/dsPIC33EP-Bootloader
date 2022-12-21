#ifndef CRC_H
#define CRC_H

#inlude <stdint.h>

void initCRCengine(uint16_t u16Poly);
uint16_t RunCRC(uint8_t pu8Data[], uint16_t u16buflen);
#endif