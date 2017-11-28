#ifndef __DRVEEPROM_H
#define __DRVEEPROM_H

#include "ALLHead.h"

typedef enum{
  Block_0,
  Block_1,
  Block_2,
  Block_3,
  Block_4,
  Block_5,
  Block_6,
  Block_7,
  Block_8,
  Block_9,
  Block_10,
  Block_11,
  Block_12,
  Block_13,
  Block_14,
  Block_15
}BlockStartAddress_TypeDef;

void WriteMultiBlockByte(BlockStartAddress_TypeDef BlockStartAddress,FLASH_MemType_TypeDef FLASH_MemType, 
                FLASH_ProgramMode_TypeDef FLASH_ProgMode, uint8_t *Buffer,uint8_t BlockNum);
void ReadMultiBlockByte(BlockStartAddress_TypeDef BlockStartAddress,uint8_t BlockNum,
                        uint8_t ReadBlockByte[]);
#endif