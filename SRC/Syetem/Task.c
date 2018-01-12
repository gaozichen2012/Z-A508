#define TASKABLE
#include "AllHead.h"
bool NoUseNum=FALSE;
u8 AlarmCount=4;//2G3G切换计数,默认为3G模式

#if 1 //test
u8 Key_Flag_0=0;
u8 Key_PersonalCalling_Flag=0;
bool TASK_Ptt_StartPersonCalling_Flag=FALSE;
u8 x=0;
#endif

u8 *ucStartPTT                  = "0B0000";
u8 *ucEndPTT                    = "0C0000";
u8 *ucRequestUserListInfo       = "0E000000000064";
u8 *ucCLVL                       = "AT+CLVL=7";//音量增益7
u8 *ucVGR                       = "AT+VGR=7";//音量增益7
u8 *ucCODECCTL                  = "at^codecctl=9000,1c00,0";//音量增益4000 3c00
u8 *ucOSSYSHWID                 = "AT^OSSYSHWID=1";
u8 *ucPrefmode2                  = "AT^prefmode=2";//网络模式2G
u8 *ucPrefmode4                  = "AT^prefmode=4";//网络模式3G
u8 *ucPrefmode8                  = "AT^prefmode=8";//网络模式2/3G自动切换
u8 *ucCSQ                       = "AT+CSQ?";
u8 *ucPPPCFG                    = "AT^PPPCFG=echat,ctnet@mycdma.cn,vnet.mobi";
//u8 *ucZTTS_Abell                = "AT+ZTTS=1,\"276b07687F5EDF57F95BB28B3A67\"";欧标广域对讲机
u8 *ucZTTS_Abell                = "AT+ZTTS=1,\"2d4e745113663d6d20007f5edf57f95bb28b\"";
u8 *ucPocOpenConfig             = "0000000101";
//u8 *ucPocOpenConfig             = "00000001010101";//双全工

void Task_RunStart(void)
{
  
  UART3_ToMcuMain();
  if(BootProcess_SIMST_Flag==1)//收到模块开机指令:SIMST:1
  {
    api_disp_icoid_output( eICO_IDRXNULL, TRUE, TRUE);//GPRS无信号图标
    
    BEEP_Time(50);
    
    NoUseNum=ApiAtCmd_WritCommand(ATCOMM7_VGR,(u8 *)ucCLVL,strlen((char const *)ucCLVL));//
    Delay_100ms(1);//0.1s
    NoUseNum=ApiAtCmd_WritCommand(ATCOMM7_VGR,(u8 *)ucVGR,strlen((char const *)ucVGR));//
    Delay_100ms(1);//0.1s
    NoUseNum=ApiAtCmd_WritCommand(ATCOMM5_CODECCTL,(u8 *)ucCODECCTL,strlen((char const *)ucCODECCTL));//
    Delay_100ms(1);//0.1s
    NoUseNum=ApiAtCmd_WritCommand(ATCOMM0_OSSYSHWID,(u8 *)ucOSSYSHWID,strlen((char const *)ucOSSYSHWID));//
    Delay_100ms(1);//0.1s
    NoUseNum=ApiAtCmd_WritCommand(ATCOMM4_GD83Mode,(u8 *)ucPrefmode4,strlen((char const *)ucPrefmode4));//
    Delay_100ms(1);//0.1s
    NoUseNum=ApiAtCmd_WritCommand(ATCOMM2_ZTTS_Abell,(u8 *)ucZTTS_Abell,strlen((char const *)ucZTTS_Abell));//播报游标广域对讲机
    api_lcd_pwr_on_hint("中兴易洽广域对讲");
      //api_lcd_pwr_on_hint("中1 b c 广域对讲");
    Delay_100ms(25);//2.5s
    NoUseNum=ApiAtCmd_WritCommand(ATCOMM1_PPPCFG,(u8 *)ucPPPCFG,strlen((char const *)ucPPPCFG));//1.发送PPPCFG
    BootProcess_SIMST_Flag=0;
    VOICE_SetOutput(ATVOICE_FreePlay,"1c64227d517fdc7e",16);//播报搜索网络
    //DEL_SetTimer(0,100);
    //while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
    api_lcd_pwr_on_hint("   搜索网络...  ");
    NoUseNum=ApiAtCmd_WritCommand(ATCOMM6_CSQ,(u8 *)ucCSQ,strlen((char const *)ucCSQ));//CSQ?
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
    api_disp_icoid_output( eICO_IDEmergency, TRUE, TRUE);//开机搜到信号，显示3G图标
    api_lcd_pwr_on_hint("   正在登陆..     ");
    if(BootProcess_PPPCFG_Flag_Zanshi==1)//如果收到^PPPCFG//因为有时收不到该指令，临时屏蔽，后期加上
    {
      Delay_100ms(10);//1s
      ApiPocCmd_WritCommand(PocComm_SetParam,ucPocOpenConfig,strlen((char const *)ucPocOpenConfig));//配置echat账号、IP
      Delay_100ms(40);//4s
      VOICE_SetOutput(ATVOICE_FreePlay,"636b28577b764696",16);//播报正在登陆
     api_lcd_pwr_on_hint("   正在登陆...    ");
    //  Delay_100ms(10);//1s
      ApiPocCmd_WritCommand(PocComm_OpenPOC,ucPocOpenConfig,strlen((char const *)ucPocOpenConfig));
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
      NoUseNum=ApiAtCmd_WritCommand(ATCOMM6_CSQ,(u8 *)ucCSQ,strlen((char const *)ucCSQ));//CSQ?
    }
  }
}

void Task_RunNormalOperation(void)
{
  Keyboard_Test();
  UART3_ToMcuMain();
/***********PTT状态检测*************************************************************************************************************************/
  if(ReadInput_KEY_PTT==0)
  {
    switch(KeyDownUpChoose_GroupOrUser_Flag)
    {
    case 0://默认PTT状态
      Set_RedLed(LED_ON);
      Set_GreenLed(LED_OFF);
      ApiPocCmd_WritCommand(PocComm_StartPTT,ucStartPTT,strlen((char const *)ucStartPTT));
      while(ReadInput_KEY_PTT==0)
      {
        api_disp_icoid_output( eICO_IDTX, TRUE, TRUE);//发射信号图标
        api_disp_all_screen_refresh();// 全屏统一刷新
      }
      api_disp_icoid_output( eICO_IDTALKAR, TRUE, TRUE);//默认无发射无接收信号图标
      api_disp_all_screen_refresh();// 全屏统一刷新
      ApiPocCmd_WritCommand(PocComm_EndPTT,ucEndPTT,strlen((char const *)ucEndPTT));
      break;
    case 1://=1，进入某群组
      VOICE_SetOutput(ATVOICE_FreePlay,"f25d09902d4e",12);//播报已选中
      DEL_SetTimer(0,100);
      while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
      ApiPocCmd_WritCommand(PocComm_EnterGroup,ucPocOpenConfig,strlen((char const *)ucPocOpenConfig));
      Key_Flag_0=1;
      KeyDownUpChoose_GroupOrUser_Flag=0;
      KeyUpDownCount=0;
      break;
    case 2://=2,呼叫某用户
      VOICE_SetOutput(ATVOICE_FreePlay,"f25d09902d4e",12);//播报已选中
      DEL_SetTimer(0,100);
      while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
      ApiPocCmd_WritCommand(PocComm_Invite,ucPocOpenConfig,strlen((char const *)ucPocOpenConfig));
      Key_Flag_0=1;
      KeyDownUpChoose_GroupOrUser_Flag=0;
      TASK_Ptt_StartPersonCalling_Flag=TRUE;//判断主动单呼状态（0a）
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
/*******组呼键状态检测***********************************************************************************************************************************/
  if(ReadInput_KEY_3==0)//组呼键
  {
    if(GetPersonalCallingMode()==1)//如果是单呼模式，则退出单呼
    {
      api_lcd_pwr_on_hint("    退出单呼    ");
      Delay_100ms(5);
      ApiPocCmd_WritCommand(PocComm_Cancel,(u8 *)ucQuitPersonalCalling,strlen((char const *)ucQuitPersonalCalling));
      api_lcd_pwr_on_hint("群组:   组呼模式");
    }
    else
    {
      api_lcd_pwr_on_hint("                ");//显示当前群组昵称
      api_lcd_pwr_on_hint(HexToChar_MainGroupId());//显示当前群组ID
      api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//显示当前群组昵称
    
      Key_PersonalCalling_Flag=0;
      VOICE_SetOutput(ATVOICE_FreePlay,"535f4d52A47FC47E",16);//当前群组
      DEL_SetTimer(0,100);
      while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
      //VOICE_SetOutput(ATVOICE_FreePlay,ApiAtCmd_GetMainWorkName(),ApiAtCmd_GetMainWorkNameLen());
      VOICE_SetOutput(ATVOICE_FreePlay,ApiAtCmd_GetMainWorkName(),strlen((char const *)ApiAtCmd_GetMainWorkName()));
      // VOICE_SetOutput(ATVOICE_FreePlay,"13663d6d4b6dd58bc47e3100",strlen((char const *)"13663d6d4b6dd58bc47e3100"));
      
    }
  }
/*******个呼键状态检测***************************************************************************************************************************************/
  if(ReadInput_KEY_2==0)//个呼键
  {
    api_lcd_pwr_on_hint("对象:   选择个呼");
    api_lcd_pwr_on_hint2(HexToChar_MainUserId());
    
    x++;
    
    if(x>=2)
    {
      Key_PersonalCalling_Flag=1;
      VOICE_SetOutput(ATVOICE_FreePlay,"2a4e7c542000106258540990e962",28);//个呼成员选择
    DEL_SetTimer(0,200);
    while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
    x=1;
    }

    
    ApiPocCmd_WritCommand(PocComm_UserListInfo,ucRequestUserListInfo,strlen((char const *)ucRequestUserListInfo));
    VOICE_SetOutput(ATVOICE_FreePlay,ApiAtCmd_GetUserName(0),ApiAtCmd_GetUserNameLen(0));
    KeyDownUpChoose_GroupOrUser_Flag=2;
  }
/*******报警键状态检测********************************************************************************************************************************************/
  if(ReadInput_KEY_4==0)//报警键
  {
    switch(AlarmCount)
    {
    case 4://切换为2G模式
      VOICE_SetOutput(ATVOICE_FreePlay,"517fdc7e075262633a4e32004700216a0f5f",36);//网络切换为2G模式
      api_disp_icoid_output( eICO_IDPOWERL, TRUE, TRUE);//图标：2G
      api_disp_all_screen_refresh();// 全屏统一刷新
      DEL_SetTimer(0,200);
      while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
      NoUseNum=ApiAtCmd_WritCommand(ATCOMM4_GD83Mode,(u8 *)ucPrefmode2,strlen((char const *)ucPrefmode2));//
      AlarmCount=8;
      break;
    /*case 2://切换为自动模式
      VOICE_SetOutput(ATVOICE_FreePlay,"517fdc7e075262633a4eea81a852216a0f5f",36);//网络切换为自动模式
      DEL_SetTimer(0,200);
      while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
      NoUseNum=ApiAtCmd_WritCommand(ATCOMM4_GD83Mode,(u8 *)ucPrefmode8,strlen((char const *)ucPrefmode8));//
      AlarmCount=8;
      break;*/
    case 8://切换为3G模式
      VOICE_SetOutput(ATVOICE_FreePlay,"517fdc7e075262633a4e33004700216a0f5f",36);//网络切换为3G模式
      api_disp_icoid_output( eICO_IDEmergency, TRUE, TRUE);//图标：3G
      api_disp_all_screen_refresh();// 全屏统一刷新
      DEL_SetTimer(0,200);
      while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
      NoUseNum=ApiAtCmd_WritCommand(ATCOMM4_GD83Mode,(u8 *)ucPrefmode4,strlen((char const *)ucPrefmode4));//
      AlarmCount=4;
      break;
    default:
      break;
    }
  }
  
/***********判断正常进组；正常退出组;被单呼模式；退出单呼模式；主动开始单呼；单呼；主动退出单呼*********************************************************************************************************************/
  /*if(POC_AtEnterPersonalCalling_Flag==2)//主动开始单呼模式
  {
      api_lcd_pwr_on_hint(HexToChar_MainUserId());//显示当前群组ID
      api_lcd_pwr_on_hint4("             ");//清屏
      api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//显示当前群组昵称
      api_disp_icoid_output( eICO_IDPOWERH, TRUE, TRUE);//显示个呼图标
      api_disp_all_screen_refresh();// 全屏统一刷新
      POC_AtEnterPersonalCalling_Flag=1;
  }
  else
  {
    if(POC_AtQuitPersonalCalling_Flag==2)
    {
      POC_AtQuitPersonalCalling_Flag=0;
      //退出单呼无需处理，进入群组处理
    }
  }*/



  if(POC_EnterPersonalCalling_Flag==2)//如果是正在接入单呼-------------------------------
    {
      
      api_lcd_pwr_on_hint(HexToChar_MainUserId());//显示当前群组ID
      api_lcd_pwr_on_hint4("             ");//清屏
      api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//显示当前群组昵称
      api_disp_icoid_output( eICO_IDPOWERH, TRUE, TRUE);//显示个呼图标
      api_disp_all_screen_refresh();// 全屏统一刷新
      POC_EnterPersonalCalling_Flag=1;//在单呼模式
    }
    else//如果是正常进组；组内；正常退出组;单呼；退出单呼模式；
    {
      if(POC_EnterPersonalCalling_Flag==0)//如果是正常进组；组内；正常退出组
      {
        if(POC_EnterGroupCalling_Flag==2)//正在进入群组
        {

          if(POC_AtEnterPersonalCalling_Flag==2)//主动开始单呼模式
          {
          api_lcd_pwr_on_hint("                ");//清屏
          api_lcd_pwr_on_hint(HexToChar_MainGroupId());//显示当前群组ID
          api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//显示当前群组昵称
          api_disp_icoid_output( eICO_IDPOWERH, TRUE, TRUE);//显示个呼图标
          api_disp_all_screen_refresh();// 全屏统一刷新//可能会对POC开机PoC指令识别有影响
            POC_AtEnterPersonalCalling_Flag=1;
          }
          else
          {
            if(POC_AtEnterPersonalCalling_Flag==1)
            {}
            else
            {
              api_lcd_pwr_on_hint("                ");//清屏
              api_lcd_pwr_on_hint(HexToChar_MainGroupId());//显示当前群组ID
              api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//显示当前群组昵称
              api_disp_icoid_output( eICO_IDPOWERM, TRUE, TRUE);//显示组呼图标
              api_disp_all_screen_refresh();// 全屏统一刷新//可能会对POC开机PoC指令识别有影响
            }
          }
          POC_EnterGroupCalling_Flag=1;
          
        }
        else if(POC_EnterGroupCalling_Flag==1)//组内
        {
        }
        else//退出组别
        {
          if(POC_QuitGroupCalling_Flag==2)
          {
              api_lcd_pwr_on_hint("                ");//清屏
              api_lcd_pwr_on_hint(HexToChar_MainGroupId());//显示当前群组ID
              api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//显示当前群组昵称
              api_disp_icoid_output( eICO_IDPOWERM, TRUE, TRUE);//显示组呼图标
              api_disp_all_screen_refresh();// 全屏统一刷新//可能会对POC开机PoC指令识别有影响
              POC_QuitGroupCalling_Flag=0;
          }

        }
      }
      else//单呼模式；退出单呼模式；
      {
        if(POC_QuitPersonalCalling_Flag==2)//如果是退出单呼模式----------------------------
        {
          POC_QuitPersonalCalling_Flag=0;
          //退出单呼无需处理，进入群组处理 
        }
        else//单呼模式
        {

        }
      }
    }

/*********判断发射接收图标状态****************************************************************************************************************************/
if(POC_ReceivedVoiceStart_Flag==2)//刚接收语音状态
{
  api_disp_icoid_output( eICO_IDVOX, TRUE, TRUE);//接收信号图标
  api_disp_all_screen_refresh();// 全屏统一刷新
  POC_ReceivedVoiceStart_Flag=1;//接收语音状态
}
else//0空闲状态；1接收状态
{
  if(POC_ReceivedVoiceEnd_Flag==2)//空闲状态
  {
    api_disp_icoid_output( eICO_IDTALKAR, TRUE, TRUE);//默认无发射无接收信号图标
    api_disp_all_screen_refresh();// 全屏统一刷新
    POC_ReceivedVoiceEnd_Flag=0;//默认无语音状态
  }
  else//空闲状态
  {}
}
}

void TASK_WriteFreq(void)
{
  UART3_ToMcuMain();
}
void TASK_RunLoBattery(void)
{
  api_lcd_pwr_on_hint(" 电量低  请充电  ");
  VOICE_SetOutput(ATVOICE_FreePlay,"3575606c3575cf914e4f0cfff78b45513575",36);//群组选择
  DEL_SetTimer(0,1000);
  while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
  BEEP_Time(10);
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

