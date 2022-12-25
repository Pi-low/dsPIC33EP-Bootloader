#ifndef CRC_H
#define CRC_H

#include <stdint.h>

void WupdateCRC16(uint16_t* Fpu16Input, uint8_t Fu8Data);
void BufUpdateCrc16(uint16_t* Fpu16Input, uint8_t* Fpu8Data, uint16_t Fu16Length);

#endif