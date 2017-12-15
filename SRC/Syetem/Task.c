#define TASKABLE
#include "AllHead.h"

#if 1 //test
u8 Key_Flag_0=0;
u8 Key_PersonalCalling_Flag=0;
u8 x=0;
#endif

#if 0//2号
u8 *ucSetParamConfig    = "01000069703D302E302E302E303B69643D31393830303330373437323B7077643D3131313131313B00";
#endif
#if 0//3号
u8 *ucSetParamConfig    = "01000069703D302E302E302E303B69643D31393830303330373437333B7077643D3131313131313B00";
#endif
#if 0//4号
u8 *ucSetParamConfig    = "01000069703D302E302E302E303B69643D31393830303330373437343B7077643D3131313131313B00";
#endif
#if 1//5号
u8 *ucSetParamConfig    = "01000069703D302E302E302E303B69643D31393830303330373437353B7077643D3131313131313B00";
#endif
#if 0//6号
u8 *ucSetParamConfig    = "01000069703D302E302E302E303B69643D31393830303330373437363B7077643D3131313131313B00";
#endif
#if 0//7号
u8 *ucSetParamConfig    = "01000069703d302e302e302e303b69643d31393830303330383636373b7077643d3131313131313b00";
#endif
#if 0//8号
u8 *ucSetParamConfig    = "01000069703d302e302e302e303b69643d31393830303330383636383b7077643d3131313131313b00";
#endif
#if 0//9号
u8 *ucSetParamConfig    = "01000069703d302e302e302e303b69643d31393830303330383636393b7077643d3131313131313b00";
#endif
#if 0//0号//调度员
u8 *ucSetParamConfig    = "01000069703d302e302e302e303b69643d31393830303330383637303b7077643d3131313131313b00";
#endif
#if 0//1号
u8 *ucSetParamConfig    = "01000069703d302e302e302e303b69643d31393830303330383637313b7077643d3131313131313b00";
#endif
u8 *ucStartPTT                  = "0B0000";
u8 *ucEndPTT                    = "0C0000";
u8 *ucRequestUserListInfo       = "0E000000000064";
u8 *ucCLVL                       = "AT+CLVL=3";//音量增益7
u8 *ucVGR                       = "AT+VGR=3";//音量增益7
u8 *ucCODECCTL                  = "at^codecctl=4000,1e00,0";//音量增益 3c00
u8 *ucOSSYSHWID                 = "AT^OSSYSHWID=1";
u8 *ucPrefmode                  = "AT^prefmode=4";
u8 *ucCSQ                       = "AT+CSQ?";
u8 *ucPPPCFG                    = "AT^PPPCFG=echat,ctnet@mycdma.cn,vnet.mobi";
//u8 *ucZTTS_Abell                = "AT+ZTTS=1,\"276b07687F5EDF57F95BB28B3A67\"";欧标广域对讲机
u8 *ucZTTS_Abell                = "AT+ZTTS=1,\"2d4e745113663d6d20007f5edf57f95bb28b\"";
u8 *ucPocOpenConfig             = "0000000101";


void Task_RunStart(void)
{
  u8 v;
  
  UART3_ToMcuMain();
  if(BootProcess_SIMST_Flag==1)//收到模块开机指令:SIMST:1
  {
    api_disp_icoid_output( eICO_IDRXNULL, TRUE, TRUE);//GPRS无信号图标
    api_disp_icoid_output( eICO_IDBATT3, TRUE, TRUE);//电池电量3级
    BEEP_Time(50);
    
    v=ApiAtCmd_WritCommand(ATCOMM7_VGR,(u8 *)ucCLVL,strlen((char const *)ucCLVL));//
    Delay_100ms(1);//0.1s
    v=ApiAtCmd_WritCommand(ATCOMM7_VGR,(u8 *)ucVGR,strlen((char const *)ucVGR));//
    Delay_100ms(1);//0.1s
    v=ApiAtCmd_WritCommand(ATCOMM5_CODECCTL,(u8 *)ucCODECCTL,strlen((char const *)ucCODECCTL));//
    Delay_100ms(1);//0.1s
    v=ApiAtCmd_WritCommand(ATCOMM0_OSSYSHWID,(u8 *)ucOSSYSHWID,strlen((char const *)ucOSSYSHWID));//
    Delay_100ms(1);//0.1s
    v=ApiAtCmd_WritCommand(ATCOMM4_GD83Mode,(u8 *)ucPrefmode,strlen((char const *)ucPrefmode));//
    Delay_100ms(1);//0.1s
    v=ApiAtCmd_WritCommand(ATCOMM2_ZTTS_Abell,(u8 *)ucZTTS_Abell,strlen((char const *)ucZTTS_Abell));//播报游标广域对讲机
    api_lcd_pwr_on_hint("中兴易洽广域对讲");
    Delay_100ms(25);//2.5s
    v=ApiAtCmd_WritCommand(ATCOMM1_PPPCFG,(u8 *)ucPPPCFG,strlen((char const *)ucPPPCFG));//1.发送PPPCFG
    BootProcess_SIMST_Flag=0;
    VOICE_SetOutput(ATVOICE_FreePlay,"1c64227d517fdc7e",16);//播报搜索网络
    //DEL_SetTimer(0,100);
    //while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
    api_lcd_pwr_on_hint("   搜索网络...  ");
    v=ApiAtCmd_WritCommand(ATCOMM6_CSQ,(u8 *)ucCSQ,strlen((char const *)ucCSQ));//CSQ?
    //Delay_100ms(100);//0.1s
    //DEL_SetTimer(0,100);
    //while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
  }
  else
  {
    if(BootProcess_SIMST_Flag==2)
    {
      VOICE_SetOutput(ATVOICE_FreePlay,"c0680d4e30526153",16);//播报检不到卡
      api_lcd_pwr_on_hint("    检不到卡    ");
      Delay_100ms(100);//10s
    }
  }
  if(CSQ_Flag==1)//CSQ?
  {
    api_disp_icoid_output( eICO_IDRXFULL, TRUE, TRUE);//GPRS三格信号图标
    api_lcd_pwr_on_hint("   正在登陆..     ");
    if(BootProcess_PPPCFG_Flag_Zanshi==1)//如果收到^PPPCFG//因为有时收不到该指令，临时屏蔽，后期加上
    {
      
      Delay_100ms(10);//1s
      ApiPocCmd_WritCommand(PocComm_SetParam,ucSetParamConfig,strlen((char const *)ucSetParamConfig));//配置echat账号、IP
      Delay_100ms(40);//4s
      VOICE_SetOutput(ATVOICE_FreePlay,"636b28577b764696",16);//播报正在登陆
      api_lcd_pwr_on_hint("   正在登陆...    ");
    //  Delay_100ms(10);//1s
      v=ApiPocCmd_WritCommand(PocComm_OpenPOC,ucPocOpenConfig,strlen((char const *)ucPocOpenConfig));
      BootProcess_PPPCFG_Flag_Zanshi=0;
    }
  }
  else
  {
    if(CSQ_Flag==2)
    {
      Delay_100ms(50);//5s
      VOICE_SetOutput(ATVOICE_FreePlay,"1c64227d517fdc7e",16);//播报搜索网络
      api_lcd_pwr_on_hint("   搜索网络...  ");
      v=ApiAtCmd_WritCommand(ATCOMM6_CSQ,(u8 *)ucCSQ,strlen((char const *)ucCSQ));//CSQ?
    }
  }
}

void Task_RunNormalOperation(void)
{
  u8 v;
  UART3_ToMcuMain();
  if(ReadInput_KEY_PTT==0)
  {
    switch(KeyDownUpChoose_GroupOrUser_Flag)
    {
    case 0://默认PTT状态
      Set_RedLed(LED_ON);
      v=ApiPocCmd_WritCommand(PocComm_StartPTT,ucStartPTT,strlen((char const *)ucStartPTT));
      
      while(ReadInput_KEY_PTT==0)
      {
        api_disp_icoid_output( eICO_IDTX, TRUE, TRUE);//发射信号图标
        api_disp_all_screen_refresh();//刷新屏幕数据
      }
      api_disp_icoid_output( eICO_IDTX, TRUE, FALSE);//清除发射信号图标
      api_disp_all_screen_refresh();//刷新屏幕数据
      v=ApiPocCmd_WritCommand(PocComm_EndPTT,ucEndPTT,strlen((char const *)ucEndPTT));
      break;
    case 1://=1，进入某群组
      VOICE_SetOutput(ATVOICE_FreePlay,"f25d09902d4e",12);//播报已选中
      DEL_SetTimer(0,100);
      while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
      v=ApiPocCmd_WritCommand(PocComm_EnterGroup,ucPocOpenConfig,strlen((char const *)ucPocOpenConfig));
      Key_Flag_0=1;
      KeyDownUpChoose_GroupOrUser_Flag=0;
      KeyUpDownCount=0;
      break;
    case 2://=2,呼叫某用户
      VOICE_SetOutput(ATVOICE_FreePlay,"f25d09902d4e",12);//播报已选中
      DEL_SetTimer(0,100);
      while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
      v=ApiPocCmd_WritCommand(PocComm_Invite,ucPocOpenConfig,strlen((char const *)ucPocOpenConfig));
      Key_Flag_0=1;
      KeyDownUpChoose_GroupOrUser_Flag=0;
     // KeyPersonalCallingCount=0;
      break;
    default:
      break;
    }
    if(Key_Flag_0==1)
    {
     // DEL_SetTimer(0,100);
      //while(1)
      {
        //Key_Flag_0=0;
        //if(DEL_GetTimer(0) == TRUE) {break;}
      }
    }
  }
  else
  {
    Set_RedLed(LED_OFF);
  }
  
  if(ReadInput_KEY_3==0)//组呼键
  {
    api_lcd_pwr_on_hint("群组:   组呼模式");//显示汉字
    api_lcd_pwr_on_hint2(HexToChar_MainGroupId());//显示数据
    
    Key_PersonalCalling_Flag=0;
    VOICE_SetOutput(ATVOICE_FreePlay,"A47FC47E0990E962",16);//群组选择
    DEL_SetTimer(0,100);
    while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
    VOICE_SetOutput(ATVOICE_FreePlay,ApiAtCmd_GetMainWorkName(),ApiAtCmd_GetMainWorkNameLen());
  }
  
  if(ReadInput_KEY_2==0)//个呼键
  {
    api_lcd_pwr_on_hint("对象:   个呼模式");
    api_lcd_pwr_on_hint2(HexToChar_MainUserId());
    
    x++;
    Key_PersonalCalling_Flag=1;
    VOICE_SetOutput(ATVOICE_FreePlay,"2a4e7c542000106258540990e962",28);//个呼成员选择
    DEL_SetTimer(0,200);
    while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
    
    v=ApiPocCmd_WritCommand(PocComm_UserListInfo,ucRequestUserListInfo,strlen((char const *)ucRequestUserListInfo));
    VOICE_SetOutput(ATVOICE_FreePlay,ApiAtCmd_GetUserName(0),ApiAtCmd_GetUserNameLen(0));
    KeyDownUpChoose_GroupOrUser_Flag=2;
  }
  Keyboard_Test();
  if(POC_GetGroupInformationFlag==1)//收到86则显示群组信息，解决开机不按按键不显示群组信息
  {
    POC_GetGroupInformationFlag=0;
    api_lcd_pwr_on_hint("群组:   组呼模式");//显示汉字
    api_lcd_pwr_on_hint2(HexToChar_MainGroupId());//显示数据
  }
}

void TASK_WriteFreq(void)
{
  UART3_ToMcuMain();
}
void Delay_100ms(u8 T)
{
  u16 i;
  u8 j,k,l;
    for(j = 0; j < 83; j++)
    for(l = 0; l < 10; l++)
        for(k = 0; k < 100; k++)
          for(i = 0; i < T; i++)
      {
        nop();
      }
  return;
}

