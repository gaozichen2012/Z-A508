#include "AllHead.h"

/*******************************************************************************
         每种型号的EEPROM的大小不一样，调用此函数的时候要注意将要写进的字节数组
         的大小是否超过该型号的EEPROM的地址。
         大容量的EEPROM的型号是STM8S208, STM8S207, STM8S007, STM8AF52Ax, STM8AF62Ax 
         EEPROM的地址是从0x004000到0x0047ff，共2048Byte,每个Block是128Byte,一共16个Block.
         中容量的EEPROM的型号是STM8S105, STM8S005, STM8AF626x
         EEPROM的地址是从0x004000到0x0043ff，共1024Byte,每个Block是128Byte,一共8个Block.
         小容量的EEPROM的型号是STM8S103, STM8S003, STM8S903 
         EEPROM的地址是从0x004000到0x00427f，共1024Byte,每个Block是64Byte,一共10个Block.

 ******************************************************************************/
void WriteEEPROMByte(uint32_t Address, uint8_t Data)
{
  FLASH_Unlock(FLASH_MEMTYPE_DATA);//可以擦写EEPROM或Flash:FLASH_Unlock(FLASH_MemType_Program);
  while (FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET);//等待解锁成功
  FLASH_ProgramByte(Address,Data);//向指定地址写入数据Data
  while (FLASH_GetFlagStatus(FLASH_FLAG_HVOFF) == RESET);//写入完成
  FLASH_Lock(FLASH_MEMTYPE_DATA);//上锁
}