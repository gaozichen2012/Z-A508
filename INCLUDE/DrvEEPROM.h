#ifndef __DRVEEPROM_H
#define __DRVEEPROM_H

#include "ALLHead.h"
#include "stm8s_flash.h"

void WriteEEPROMByte(uint32_t Address, uint8_t Data);
#endif