#include "AllHead.h"
u8 *ucKeyUp                = "10000003";
u8 *ucKeyDown              = "10000004";
s8 GroupCallingNum=1;
u8 KeyDownCount=0;
u8 KeyUpCount=0;

u32 get_key_value(u8 scan_value);
u8 Key_Flag_1=0;
void Keyboard_Test(void)
{
  u8 r=0;

  u8 scanvalue = 0;
  u32 ulAllKeyID = 0x00000000;
  scanvalue = drv_keypad_scan();
  ulAllKeyID = get_key_value(scanvalue);
  switch(ulAllKeyID)
  {
  case 0x00000002://1
    api_lcd_pwr_on_hint("欧标按键:1     ");
    break; 
  case 0x00000008://3
    api_lcd_pwr_on_hint("欧标按键:3     ");
    break;
  case 0x00000080://4
    api_lcd_pwr_on_hint("欧标按键:4     ");
    break;
  case 0x00000200://6
    api_lcd_pwr_on_hint("欧标按键:6     ");
    break;
  case 0x00002000://7
    api_lcd_pwr_on_hint("欧标按键:7     ");
    break;
  case 0x00008000://9
    api_lcd_pwr_on_hint("欧标按键:9     ");
    break;
  case 0x00010000://dn
    KeyDownCount++;
    GroupCallingNum=ApiAtCmd_GetMainGroupId()-KeyDownCount;
    if(GroupCallingNum<=0)
    {
      GroupCallingNum=ApiAtCmd_GetGroupNum();
      KeyDownCount=ApiAtCmd_GetMainGroupId()-ApiAtCmd_GetGroupNum();
    }
      VOICE_SetOutput(ATVOICE_FreePlay,ApiAtCmd_GetGroupName(GroupCallingNum),ApiAtCmd_GetGroupNameLen(GroupCallingNum));
    KeyDownUpChooseGroup_Flag=1;
    //r=ApiPocCmd_WritCommand(PocComm_Key,ucKeyDown,strlen((char const *)ucKeyDown));
    Key_Flag_1=1;
    api_lcd_pwr_on_hint("欧标按键:Down  ");
    break;  
  case 0x00000010://ok
    api_lcd_pwr_on_hint("欧标按键:OK    ");
    break;
  case 0x00800000://menu   
    api_lcd_pwr_on_hint("欧标按键:Menu  ");
    break;   
  case 0x00000004://2
    api_lcd_pwr_on_hint("欧标按键:2     ");
    break;  
  case 0x00080000://*
    api_lcd_pwr_on_hint("欧标按键:*     ");
    break;  
  case 0x00000100://5
    api_lcd_pwr_on_hint("欧标按键:5     ");
    break;  
  case 0x00100000://0
    api_lcd_pwr_on_hint("欧标按键:0     ");
    break;  
  case 0x00004000://8
    api_lcd_pwr_on_hint("欧标按键:8     ");
    break;  
  case 0x00200000://#
    api_lcd_pwr_on_hint("欧标按键:#     ");
    break;  
  case 0x00000400://up
    KeyUpCount++;
    GroupCallingNum=ApiAtCmd_GetMainGroupId()+KeyUpCount;
    if(GroupCallingNum>ApiAtCmd_GetGroupNum())
    {
      GroupCallingNum=1;
      KeyUpCount=0;
    }
      VOICE_SetOutput(ATVOICE_FreePlay,ApiAtCmd_GetGroupName(GroupCallingNum),ApiAtCmd_GetGroupNameLen(GroupCallingNum));
    KeyDownUpChooseGroup_Flag=1;
    //r=ApiPocCmd_WritCommand(PocComm_Key,ucKeyUp,strlen((char const *)ucKeyUp));
    Key_Flag_1=1;
    api_lcd_pwr_on_hint("欧标按键:Up    ");
    break;
  case 0x00400000://cancel
    api_lcd_pwr_on_hint("欧标按键:Cancel");
    break;  
  default:
    break;
  }
    if(Key_Flag_1==1)
      {
        DEL_SetTimer(1,100);
        while(1)
        {
          Key_Flag_1=0;
          if(DEL_GetTimer(1) == TRUE) {break;}
        }
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