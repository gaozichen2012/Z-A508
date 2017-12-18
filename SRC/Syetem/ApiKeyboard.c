#include "AllHead.h"
u8 *ucKeyUp                = "10000003";
u8 *ucKeyDown              = "10000004";
u8 *ucQuitPersonalCalling  = "0A0000ffffffff";
s8 GroupCallingNum=1;
s8 PersonalCallingNum=0;
s8 KeyUpDownCount=0;//组呼上下键计数
s8 KeyPersonalCallingCount=0;//个呼上下键计数
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
    //api_lcd_pwr_on_hint("欧标按键:1     ");
    break; 
  case 0x00000008://3
    //api_lcd_pwr_on_hint("欧标按键:3     ");
    break;
  case 0x00000080://4
    //api_lcd_pwr_on_hint("欧标按键:4     ");
    break;
  case 0x00000200://6
    //api_lcd_pwr_on_hint("欧标按键:6     ");
    break;
  case 0x00002000://7
    //api_lcd_pwr_on_hint("欧标按键:7     ");
    break;
  case 0x00008000://9
    //api_lcd_pwr_on_hint("欧标按键:9     ");
    break;
  case 0x00010000://dn
    if(Key_PersonalCalling_Flag==1)//如果按下个呼键
    {
      KeyPersonalCallingCount--;
      PersonalCallingNum=KeyPersonalCallingCount;//个呼计数从零开始
      if(PersonalCallingNum<0)
      {
        PersonalCallingNum=ApiAtCmd_GetUserNum()-1;
        KeyPersonalCallingCount=ApiAtCmd_GetUserNum()-1;
      }
      VOICE_SetOutput(ATVOICE_FreePlay,ApiAtCmd_GetUserName(PersonalCallingNum),ApiAtCmd_GetUserNameLen(PersonalCallingNum));//播报按上键之后对应的用户名
      api_lcd_pwr_on_hint("对象:   选择个呼");
      api_lcd_pwr_on_hint2(HexToChar_PersonalCallingNum());
      KeyDownUpChoose_GroupOrUser_Flag=2;
    }
    else
    {
      KeyUpDownCount--;
      GroupCallingNum=ApiAtCmd_GetMainGroupId()+KeyUpDownCount;
      if(GroupCallingNum<=0)
      {
        GroupCallingNum=ApiAtCmd_GetGroupNum();
        KeyUpDownCount=ApiAtCmd_GetGroupNum()-ApiAtCmd_GetMainGroupId();//
      }
      VOICE_SetOutput(ATVOICE_FreePlay,ApiAtCmd_GetGroupName(GroupCallingNum),ApiAtCmd_GetGroupNameLen(GroupCallingNum));
      api_lcd_pwr_on_hint("群组:   选择群组");//显示汉字
      api_lcd_pwr_on_hint2(HexToChar_GroupCallingNum());//显示数据
      KeyDownUpChoose_GroupOrUser_Flag=1;
    }
    Key_Flag_1=1;
    //api_lcd_pwr_on_hint("欧标按键:Down  ");
    break;  
  case 0x00000010://ok
    //api_lcd_pwr_on_hint("欧标按键:OK     ");
    break;
  case 0x00800000://menu   
    //api_lcd_pwr_on_hint("欧标按键:Menu   ");
    break;   
  case 0x00000004://2
    //api_lcd_pwr_on_hint("欧标按键:2      ");
    break;  
  case 0x00080000://*
    //api_lcd_pwr_on_hint("欧标按键:*      ");
    break;  
  case 0x00000100://5
    //api_lcd_pwr_on_hint("欧标按键:5      ");
    break;  
  case 0x00100000://0
    //api_lcd_pwr_on_hint("欧标按键:0      ");
    break;  
  case 0x00004000://8
    //api_lcd_pwr_on_hint("欧标按键:8      ");
    break;  
  case 0x00200000://#
    //api_lcd_pwr_on_hint("欧标按键:#      ");
    break;  
  case 0x00000400://up
    if(Key_PersonalCalling_Flag==1)//如果按下个呼键
    {
    KeyPersonalCallingCount++;
    PersonalCallingNum=KeyPersonalCallingCount;//个呼计数从零开始
    if(PersonalCallingNum>ApiAtCmd_GetUserNum()-1)
    {
      KeyPersonalCallingCount=0;
      PersonalCallingNum=0;
    }
    VOICE_SetOutput(ATVOICE_FreePlay,ApiAtCmd_GetUserName(PersonalCallingNum),ApiAtCmd_GetUserNameLen(PersonalCallingNum));//播报按上键之后对应的用户名
    api_lcd_pwr_on_hint("对象:   个呼选择");
    api_lcd_pwr_on_hint2(HexToChar_PersonalCallingNum());
    KeyDownUpChoose_GroupOrUser_Flag=2;
    }
    else
    {
      KeyUpDownCount++;
    GroupCallingNum=ApiAtCmd_GetMainGroupId()+KeyUpDownCount;
    if(GroupCallingNum>ApiAtCmd_GetGroupNum())
    {
      GroupCallingNum=1;
      KeyUpDownCount=1-ApiAtCmd_GetMainGroupId();
    }
    VOICE_SetOutput(ATVOICE_FreePlay,ApiAtCmd_GetGroupName(GroupCallingNum),ApiAtCmd_GetGroupNameLen(GroupCallingNum));
    api_lcd_pwr_on_hint("群组:   群组选择");//显示汉字
    api_lcd_pwr_on_hint2(HexToChar_GroupCallingNum());//显示数据
    KeyDownUpChoose_GroupOrUser_Flag=1;
    }
    Key_Flag_1=1;
   
    break;
  case 0x00400000://cancel
    api_lcd_pwr_on_hint("    退出单呼    ");
    Delay_100ms(5);
    r=ApiPocCmd_WritCommand(PocComm_Cancel,(u8 *)ucQuitPersonalCalling,strlen((char const *)ucQuitPersonalCalling));
    api_lcd_pwr_on_hint("群组:   组呼模式");
    Key_Flag_1=1;//按键延时标志位
    Key_PersonalCalling_Flag=0;//进入组呼标志位
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