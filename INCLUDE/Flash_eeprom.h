#ifndef __FLASH_EEPROM_H
#define __FLASH_EEPROM_H

#include "stm8s.h"
#include "stm8s_flash.h"

void WriteMultiBlockByte(uint8_t block_count,FLASH_MemType_TypeDef FLASH_MemType, 
                FLASH_ProgramMode_TypeDef FLASH_ProgMode, uint8_t *Buffer,uint8_t BlockNum);
void Flash_WriteDataBlock(uint8_t block_count, uint8_t *Buffer);
void ReadMultiBlockByte(uint8_t BlockNum,uint8_t ReadBlockByte[]);
void WriteEEPROMByte(uint32_t Address, uint8_t Data);
bool FILE_Write(u16 iAdr,u16 iLen,u8 *pBuf);
bool FILE_Read(u16 iAdr,u16 iLen,u8 *pBuf);//如果pBuf是字符串,则最后一位为\0
#endif
