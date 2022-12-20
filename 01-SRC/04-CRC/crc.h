#ifndef CRC_H
#define CRC_H

#inlude <stdint.h>

void initCRCengine(uint16_t Poly);
uint16_t RunCRC(uint8_t buffer[], uint16_t buflen);
#endif