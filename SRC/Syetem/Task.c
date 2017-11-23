#define TASKABLE
#include "AllHead.h"


u8 Key_Flag_0=0;
u8 GroupOrPersonalCalling_Flag=0;


u8 *ucStartPTT                  = "0B0000";
u8 *ucEndPTT                    = "0C0000";

u8 *ucRequestUserListInfo       = "0E000000000001";
u8 *ucOSSYSHWID                 = "AT^OSSYSHWID=1";
u8 *ucPPPCFG                    = "AT^PPPCFG=echat,ctnet@mycdma.cn,vnet.mobi";
u8 *ucZTTS_Abell                = "AT+ZTTS=1,\"276b07687F5EDF57F95BB28B3A67\"";

u8 *ucSetParamConfig            = "01000069703D302E302E302E303B69643D31393830303330373437333B7077643D3131313131313B00";
u8 *ucPocOpenConfig             = "0000000101";

void Task_RunStart(void)
{
  u8 v;
  if(BootProcess_SIMST_Flag==1)//收到模块开机指令:SIMST:1
  {
    BEEP_Time(50);

    v=ApiAtCmd_WritCommand(ATCOMM2_ZTTS_Abell,(u8 *)ucZTTS_Abell,strlen((char const *)ucZTTS_Abell));//播报游标广域对讲机
    
    DEL_SetTimer(0,250);
    while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
    
    v=ApiAtCmd_WritCommand(ATCOMM1_PPPCFG,(u8 *)ucPPPCFG,strlen((char const *)ucPPPCFG));//1.发送PPPCFG
    BootProcess_SIMST_Flag=0;
  }
  if(BootProcess_PPPCFG_Flag==1)//如果收到^PPPCFG
  {
    BootProcess_PPPCFG_Flag=0;
    ApiPocCmd_WritCommand(PocComm_SetParam,ucSetParamConfig,strlen((char const *)ucSetParamConfig));//配置echat账号、IP
    DEL_SetTimer(0,500);
    while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
    v=ApiPocCmd_WritCommand(PocComm_OpenPOC,ucPocOpenConfig,strlen((char const *)ucPocOpenConfig));
  }

}

void Task_RunNormalOperation(void)
{
  u8 v;
  if(ReadInput_KEY_PTT==0)
  {
    if(KeyDownUpChooseGroup_Flag==1)
    {
      VOICE_SetOutput(ATVOICE_FreePlay,"f25d09902d4e",12);//播报已选中
      DEL_SetTimer(0,100);
      while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
      v=ApiPocCmd_WritCommand(PocComm_EnterGroup,ucPocOpenConfig,strlen((char const *)ucPocOpenConfig));

      Key_Flag_0=1;
      KeyDownUpChooseGroup_Flag=0;
    }
    else
    {
      Set_RedLed(LED_ON);
      v=ApiPocCmd_WritCommand(PocComm_StartPTT,ucStartPTT,strlen((char const *)ucStartPTT));
      while(ReadInput_KEY_PTT==0);
      v=ApiPocCmd_WritCommand(PocComm_EndPTT,ucEndPTT,strlen((char const *)ucEndPTT));
    }
    
        if(Key_Flag_0==1)
      {
        DEL_SetTimer(0,100);
        while(1)
        {
          Key_Flag_0=0;
          if(DEL_GetTimer(0) == TRUE) {break;}
        }
      }
  }
  else
  {
    Set_RedLed(LED_OFF);
  }
  
  if(ReadInput_KEY_3==0)//组呼键
  {
    GroupOrPersonalCalling_Flag=1;
    VOICE_SetOutput(ATVOICE_FreePlay,"A47FC47E0990E962",16);//群组选择
    DEL_SetTimer(0,100);
    while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
    VOICE_SetOutput(ATVOICE_FreePlay,ApiAtCmd_GetMainWorkName(),ApiAtCmd_GetMainWorkNameLen());
  }
 /* if(ReadInput_KEY_2==0)//个呼键
  {
    GroupOrPersonalCalling_Flag=2;
    VOICE_SetOutput(ATVOICE_FreePlay,"2a4e7c542000106258540990e962",28);//个呼成员选择
    DEL_SetTimer(0,200);
    while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
    v=ApiPocCmd_WritCommand(PocComm_UserListInfo,ucRequestUserListInfo,strlen((char const *)ucRequestUserListInfo));
    
    v=ApiPocCmd_WritCommand(PocComm_Invite,ucRequestUserListInfo,strlen((char const *)ucRequestUserListInfo));
      
  }*/
  
  Keyboard_Test();
}

void TASK_WriteFreq(void)
{

}