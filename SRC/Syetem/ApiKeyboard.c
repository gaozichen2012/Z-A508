#include "AllHead.h"

#define KeyCountNum 50//数字键盘消抖-解决了上下键误读数字键问题
u8 AkeyvolumeCount=7;
u8 *ucVGR1                       = "AT+VGR=1";//音量增益1
u8 *ucVGR7                       = "AT+VGR=7";//音量增益7
u8 *ucKeyUp                = "10000003";
u8 *ucKeyDown              = "10000004";
u8 *ucQuitPersonalCalling  = "0A0000ffffffff";
s8 GroupCallingNum=1;
s8 PersonalCallingNum=0;
s8 KeyUpDownCount=0;//组呼上下键计数
s8 KeyPersonalCallingCount=0;//个呼上下键计数
u32 get_key_value(u8 scan_value);
u8 Key_Flag_1=0;
u8 MenuMode_Flag=0;
u8 MenuModeCount=1;
bool NumberKeyboardPressDown_flag=FALSE;
bool LockingState_EnterOK_Flag=FALSE;
u8 TestNum1,TestNum2,TestNum3,TestNum4,TestNum5,TestNum6;
u8 TestNum7,TestNum8,TestNum9,TestNum10,TestNum11,TestNum12;
u8 TestBuf1[6];//测试显示屏短号号码使用
  u8 num1=0;//测试显示屏短号号码使用
  u8 num2=0;//测试显示屏短号号码使用
  u8 num3=0;//测试显示屏短号号码使用
  u8 num4=0;//测试显示屏短号号码使用
  u8 num5=0;//测试显示屏短号号码使用
  u8 num6=0;//测试显示屏短号号码使用
  u8 numCount=0;//测试显示屏短号号码使用
bool PressButton;//测试短号功能使用
bool KeyBoardState;//测试短号功能使用
static void GeHuTest(u32 KeyID);
void Keyboard_Test(void)
{
  

  u8 scanvalue = 0;
  u32 ulAllKeyID = 0x00000000;

  scanvalue = drv_keypad_scan();
  ulAllKeyID = get_key_value(scanvalue);
  switch(ulAllKeyID)
  {
  case 0x00000002://1
    TestNum1++;
    if(TestNum1>=KeyCountNum)
    {
      TestNum1=0;
    NumberKeyboardPressDown_flag=TRUE;
    }
    break; 
  case 0x00000008://3
    TestNum2++;
    if(TestNum2>=KeyCountNum)
    {
      TestNum2=0;
    NumberKeyboardPressDown_flag=TRUE;
    }
    break;
  case 0x00000080://4
    TestNum3++;
    if(TestNum3>=KeyCountNum)
    {
      TestNum3=0;
    NumberKeyboardPressDown_flag=TRUE;
    }
    break;
  case 0x00000200://6
    TestNum4++;
    if(TestNum4>=KeyCountNum)
    {
      TestNum4=0;
    NumberKeyboardPressDown_flag=TRUE;
    }
    break;
  case 0x00002000://7
    TestNum5++;
    if(TestNum5>=KeyCountNum)
    {
      TestNum5=0;
    NumberKeyboardPressDown_flag=TRUE;
    }
     break;
  case 0x00008000://9
    TestNum6++;
    if(TestNum6>=KeyCountNum)
    {
      TestNum6=0;
    NumberKeyboardPressDown_flag=TRUE;
    }
    break;
  case 0x00010000://dn
    NumberKeyboardPressDown_flag=TRUE;
    if(LockingState_Flag==TRUE)
    {}
    else
    {
    if(MenuMode_Flag==1)
    {
      MenuModeCount=MenuModeCount-1;
      if(MenuModeCount<1) {MenuModeCount=8;}
      MenuDisplay(MenuModeCount);
    }
    else
    {
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
    }
    Key_Flag_1=1;
    }
    //api_lcd_pwr_on_hint("欧标按键:Down  ");
    break;  
  case 0x00000010://ok
    //NumberKeyboardPressDown_flag=TRUE;
    if(LockingState_Flag==TRUE)
    {
      MenuDisplay(Menu_UnlockStep1_Ok);
      LockingState_EnterOK_Flag=TRUE;
    }
    else
    {
      MenuDisplay(Menu1);
      MenuMode_Flag=1;
    }
    

    break;
  case 0x00800000://menu
    NumberKeyboardPressDown_flag=TRUE;
    if(LockingState_Flag==TRUE)
    {}
    else
    {
    if(AkeyvolumeCount==7)
    {
      
      VOICE_SetOutput(ATVOICE_FreePlay,"2c54527b216a0f5f",16);//听筒模式
      DEL_SetTimer(0,100);
      while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
      NoUseNum=ApiAtCmd_WritCommand(ATCOMM7_VGR,(u8 *)ucVGR1,strlen((char const *)ucVGR1));//
      AkeyvolumeCount=1;
    }
    else
    {
      if(AkeyvolumeCount==1)
      {
        VOICE_SetOutput(ATVOICE_FreePlay,"4d51d063216a0f5f",16);//免提模式
        DEL_SetTimer(0,100);
        while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
        NoUseNum=ApiAtCmd_WritCommand(ATCOMM7_VGR,(u8 *)ucVGR7,strlen((char const *)ucVGR7));//
        AkeyvolumeCount=7;
      }
    }
    }

    break;   
  case 0x00000004://2
    TestNum7++;
    if(TestNum7>=KeyCountNum)
    {
      TestNum7=0;
    NumberKeyboardPressDown_flag=TRUE;
    }
    break;  
  case 0x00080000://*
    TestNum8++;
    if(TestNum8>=KeyCountNum)
    {
      TestNum8=0;
      if(LockingState_EnterOK_Flag==TRUE)//如果锁定状态下按了OK键然后按了#键
      {
        LockingState_EnterOK_Flag=FALSE;
        LockingState_Flag=FALSE;
        TimeCount=0;
        MenuDisplay(Menu_Locking_NoOperation);
      }
      else
      {
        NumberKeyboardPressDown_flag=TRUE;
      }
    }
    break;  
  case 0x00000100://5
    TestNum9++;
    if(TestNum9>=KeyCountNum)
    {
      TestNum9=0;
    NumberKeyboardPressDown_flag=TRUE;
    }
    break;  
  case 0x00100000://0
    TestNum10++;
    if(TestNum10>=KeyCountNum)
    {
      TestNum10=0;
    NumberKeyboardPressDown_flag=TRUE;
    }
    break;  
  case 0x00004000://8
    TestNum11++;
    if(TestNum11>=KeyCountNum)
    {
      TestNum11=0;
    NumberKeyboardPressDown_flag=TRUE;
    }
    break;  
  case 0x00200000://#
    TestNum12++;
    if(TestNum12>=KeyCountNum)
    {
      TestNum12=0;
      NumberKeyboardPressDown_flag=TRUE;
    }
    break;  
  case 0x00000400://up
    NumberKeyboardPressDown_flag=TRUE;
    if(LockingState_Flag==TRUE)
    {}
    else
    {
    if(MenuMode_Flag==1)
    {
      MenuModeCount=MenuModeCount+1;
      if(MenuModeCount>8) {MenuModeCount=1;}
      MenuDisplay(MenuModeCount);
    }
    else
    {
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
    }
    Key_Flag_1=1;
    }
   
    break;
  case 0x00400000://cancel
    NumberKeyboardPressDown_flag=TRUE;
    if(LockingState_Flag==TRUE)
    {}
    else
    {
      if(MenuMode_Flag==1)
      {
        MenuDisplay(Menu0);
        MenuMode_Flag=0;
      }
      api_lcd_pwr_on_hint("    组呼模式    ");
        ApiPocCmd_WritCommand(PocComm_Cancel,(u8 *)ucQuitPersonalCalling,strlen((char const *)ucQuitPersonalCalling));
        Key_Flag_1=1;//按键延时标志位
        Key_PersonalCalling_Flag=0;//进入组呼标志位


    }
    break;  
  default:
    
    break;
  }
  GeHuTest(ulAllKeyID);
  if(Key_Flag_1==1)
  {
    DEL_SetTimer(1,50);
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

void GeHuTest(u32 KeyID)
{
#if 0
   switch(KeyID)
  {
  case 0x00000002://1√
    TestNum1++;
    if(TestNum1>=KeyCountNum)
    {
      TestNum1=0;
      num1=1;
      PressButton=TRUE;
    }
    break; 
  case 0x00000004://2√
    TestNum7++;
    if(TestNum7>=KeyCountNum)
    {
      TestNum7=0;
      num1=2;
      PressButton=TRUE;
    }
    break;  
  case 0x00000008://3√
    TestNum2++;
    if(TestNum2>=KeyCountNum)
    {
      TestNum2=0;
      num1=3;
      PressButton=TRUE;
    }
    break;
  case 0x00000080://4√
    TestNum3++;
    if(TestNum3>=KeyCountNum)
    {
      TestNum3=0;
      num1=4;
      PressButton=TRUE;
    }
    break;
  case 0x00000100://5√
    TestNum9++;
    if(TestNum9>=KeyCountNum)
    {
      TestNum9=0;
      num1=5;
      PressButton=TRUE;
    }
    break;  
  case 0x00000200://6√
    TestNum4++;
    if(TestNum4>=KeyCountNum)
    {
      TestNum4=0;
      num1=6;
      PressButton=TRUE;
    }
    break;
  case 0x00002000://7√
    TestNum5++;
    if(TestNum5>=KeyCountNum)
    {
      TestNum5=0;
      num1=7;
      PressButton=TRUE;
    }
     break;
  case 0x00004000://8√
    TestNum11++;
    if(TestNum11>=KeyCountNum)
    {
      TestNum11=0;
      num1=8;
      PressButton=TRUE;
    }
    break;  
  case 0x00008000://9√
    TestNum6++;
    if(TestNum6>=KeyCountNum)
    {
      TestNum6=0;
      num1=9;
      PressButton=TRUE;
    }
    break;
  case 0x00100000://0
    TestNum10++;
    if(TestNum10>=KeyCountNum)
    {
      TestNum10=0;
      num1=0;
      PressButton=TRUE;
    }
    break;
  case 0x00010000://dn
    break;  
  case 0x00000010://ok
    break;
  case 0x00800000://menu   
    break;   
  case 0x00080000://*
    break;  
  case 0x00200000://#
    break;  
  case 0x00000400://up
    break;
  case 0x00400000://cancel
    numCount=0;
    break;  
  default:
    if(PressButton==TRUE)
    {
      KeyBoardState=TRUE;
      PressButton  =FALSE;
    }
    else
    {
      KeyBoardState=FALSE;
    }
    
    break;
  }
  
if(KeyBoardState==TRUE)//识别按下按键到松开按键的过程
{
  numCount++;
  num6=num5;
  num5=num4;
  num4=num3;
  num3=num2;
  num2=num1;
    switch(numCount)
    {
    case 1:
      TestBuf1[0]=num2+0x30;
      TestBuf1[1]='\0';
      break;
    case 2:
      TestBuf1[0]=num3+0x30;
      TestBuf1[1]=num2+0x30;
      TestBuf1[2]='\0';
      break;
    case 3:
      TestBuf1[0]=num4+0x30;
      TestBuf1[1]=num3+0x30;
      TestBuf1[2]=num2+0x30;
      TestBuf1[3]='\0';
      break;
    case 4:
      TestBuf1[0]=num5+0x30;
      TestBuf1[1]=num4+0x30;
      TestBuf1[2]=num3+0x30;
      TestBuf1[3]=num2+0x30;
      TestBuf1[4]='\0';
      break;
    case 5:
      TestBuf1[0]=num6+0x30;
      TestBuf1[1]=num5+0x30;
      TestBuf1[2]=num4+0x30;
      TestBuf1[3]=num3+0x30;
      TestBuf1[4]=num2+0x30;
      TestBuf1[5]='\0';
      break;
    default:
      break;
    }
   //短号呼叫有问题，暂时屏蔽
    api_lcd_pwr_on_hint3("个呼短号        ");
     api_lcd_pwr_on_hint("                ");
     api_lcd_pwr_on_hint(TestBuf1);

    
}
  #endif

}