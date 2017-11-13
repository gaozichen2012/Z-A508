#include "AllHead.h"

u32 get_key_value(u8 scan_value);

void Keyboard_Test(void)
{
  u8 scanvalue = 0;
  u32 ulAllKeyID = 0x00000000;
  scanvalue = drv_keypad_scan();
  ulAllKeyID = get_key_value(scanvalue);
  switch(ulAllKeyID)
  {
  case 0x00000002://1
  case 0x00000008://3
  case 0x00000080://4
  case 0x00000200://6
  case 0x00002000://7
  case 0x00008000://9
  case 0x00010000://dn
  case 0x00000010://ok
  case 0x00800000://menu   
    Set_RedLed(LED_ON);
    Set_GreenLed(LED_OFF);    
    break;   
  case 0x00000004://2
  case 0x00080000://*
  case 0x00000100://5
  case 0x00100000://0
  case 0x00004000://8
  case 0x00200000://#
  case 0x00000400://up

  case 0x00400000://cancel 
    Set_RedLed(LED_OFF);
    Set_GreenLed(LED_ON);
    break;
  default:
    Set_RedLed(LED_OFF);
    Set_GreenLed(LED_OFF);  
    break;
  }
}



u32 get_key_value(u8 scan_value)
{
  u32 value = 0;
  switch (scan_value)
  {
  case 0x11://"7"
    value = 0x00002000;
    break;
  case 0x12://"4"
    value = 0x00000080;
    break;
  case 0x13://"1"
    value = 0x00000002;
    break;
  case 0x14://OK
    value = 0x00000010;
    break;
  case 0x21://"8"
    value = 0x00004000;
    break;
  case 0x22://"5"
    value = 0x00000100;
    break;
  case 0x23://"2"
    value = 0x00000004;
    break;
  case 0x24://"DN"
    value = 0x00010000;
    break;
  case 0x31://"9"
    value = 0x00008000;
    break;
  case 0x32://"6"
    value = 0x00000200;
    break;
  case 0x33://"3"
    value = 0x00000008;
    break;
  case 0x34://"UP"
    value = 0x00000400;
    break;
  case 0x41://"#"
    value = 0x00200000;
    break;
  case 0x42://"0"
    value = 0x00100000;
    break;
  case 0x43://"*"
    value = 0x00080000;
    break;
  case 0x44://"Cancel"
    value = 0x00400000;
    break;
  case 0x54://"Menu"
    value = 0x00800000;
    break;
  default:
    break;
  }
  return value;
}