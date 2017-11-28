#include "AllHead.h"

#define         EEPMASS1     0xAE                   //密码钥匙1
#define         EEPMASS2     0x56                   //密码钥匙2
#define         EEP_PROG_OFF      FLASH_IAPSR &=~0x08   //EEPROM区域写保护使能
#define         FLASH_PROG_OFF      FLASH_IAPSR &=~0x02   //flash主程序区写保护使能
#define        EEP_BASE         0x4000


/*******************************************************************************
 * 名称: WriteMultiBlockByte
 * 功能: 任意写多个Block字节
 * 形参: BlockStartAddress    字节被写入的Block首地址
         FLASH_MemType        FLASH Memory操作类型
         FLASH_ProgMode       FLASH 编程模式
         Buffer               要写进flash eeprom 的字节数组
         BlockNum             要写进flash eeprom 的Block个数
 * 返回: 无
 * 说明: 每种型号的EEPROM的大小不一样，调用此函数的时候要注意将要写进的字节数组
         的大小是否超过该型号的EEPROM的地址。
         大容量的EEPROM的型号是STM8S208, STM8S207, STM8S007, STM8AF52Ax, STM8AF62Ax 
         EEPROM的地址是从0x004000到0x0047ff，共2048Byte,每个Block是128Byte,一共16个Block.
         中容量的EEPROM的型号是STM8S105, STM8S005, STM8AF626x
         EEPROM的地址是从0x004000到0x0043ff，共1024Byte,每个Block是128Byte,一共8个Block.
         小容量的EEPROM的型号是STM8S103, STM8S003, STM8S903 
         EEPROM的地址是从0x004000到0x00427f，共1024Byte,每个Block是64Byte,一共10个Block.

 ******************************************************************************/
void WriteMultiBlockByte(BlockStartAddress_TypeDef BlockStartAddress,FLASH_MemType_TypeDef FLASH_MemType, 
                FLASH_ProgramMode_TypeDef FLASH_ProgMode, uint8_t *Buffer,uint8_t BlockNum)
{
  uint8_t  BlockNum_Temp;
  /* 解锁 flash data eeprom memory */
  FLASH_Unlock(FLASH_MEMTYPE_DATA);
  /* 等待 Data EEPROM area 解锁标志位置位*/
  while (FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET)
        ;
  for(BlockNum_Temp=BlockStartAddress;BlockNum_Temp<BlockNum;BlockNum_Temp++)
  {
    if(BlockNum_Temp>FLASH_DATA_BLOCKS_NUMBER)
      break;
    FLASH_ProgramBlock(BlockNum_Temp, FLASH_MemType, FLASH_ProgMode,Buffer+BlockNum_Temp*FLASH_BLOCK_SIZE);
    FLASH_WaitForLastOperation(FLASH_MemType);
  }
  
  FLASH_Lock(FLASH_MEMTYPE_DATA);/*操作完要加锁*/

}

/*******************************************************************************
 * 名称: ReadMultiBlockByte
 * 功能: 任意读多个Block字节
 * 形参: BlockStartAddress    读Block首地址
         BlockNum             读多少Block
         ReadBlockByte[]      存放读到字节的数组       
 * 返回: 无
 * 说明: 每种型号的EEPROM的大小不一样，调用此函数的时候要注意将要写进的字节数组
         的大小是否超过该型号的EEPROM的地址。
         大容量的EEPROM的型号是STM8S208, STM8S207, STM8S007, STM8AF52Ax, STM8AF62Ax 
         EEPROM的地址是从0x004000到0x0047ff，共2048Byte,每个Block是128Byte,一共16个Block.
         BlockNum @ 1~16
         中容量的EEPROM的型号是STM8S105, STM8S005, STM8AF626x
         EEPROM的地址是从0x004000到0x0043ff，共1024Byte,每个Block是128Byte,一共8个Block.
         BlockNum @ 1~8
         小容量的EEPROM的型号是STM8S103, STM8S003, STM8S903 
         EEPROM的地址是从0x004000到0x00427f，共1024Byte,每个Block是64Byte,一共10个Block.
         BlockNum @ 1~10

 ******************************************************************************/
void ReadMultiBlockByte(BlockStartAddress_TypeDef BlockStartAddress,uint8_t BlockNum,
                        uint8_t ReadBlockByte[])
{
  uint32_t add, start_add, stop_add;
  start_add = FLASH_DATA_START_PHYSICAL_ADDRESS+(u32)((BlockNum-1)*FLASH_BLOCK_SIZE);
  stop_add = FLASH_DATA_START_PHYSICAL_ADDRESS + (u32)(BlockNum*FLASH_BLOCK_SIZE);
 
  for (add = start_add; add < stop_add; add++)
      ReadBlockByte[add-FLASH_DATA_START_PHYSICAL_ADDRESS]=FLASH_ReadByte(add);
 
  
}
