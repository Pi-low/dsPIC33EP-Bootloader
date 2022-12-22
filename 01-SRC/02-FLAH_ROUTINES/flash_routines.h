#ifndef FLASH_ROUTINE_H
#define FLASH_ROUTINE_H

#include <stdint.h>

uint32_t readAppFlag(void);
void readLogisticChar(uint8_t buffer[]);
uint16_t readSWVersion(void);

#endif