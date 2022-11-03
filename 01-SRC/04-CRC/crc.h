#ifndef CRC_H
#define CRC_H

void initCRCengine(uint16_t Poly);
uint16_t runCRCOnBuffer(uint8_t buffer[], uint16_t buflen);
#endif