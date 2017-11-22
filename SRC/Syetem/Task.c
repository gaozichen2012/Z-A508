#define TASKABLE
#include "AllHead.h"

#if 1 //test

#endif


u8 *ucOSSYSHWID         = "AT^OSSYSHWID=1";
u8 *ucPPPCFG            = "AT^PPPCFG=echat,ctnet@mycdma.cn,vnet.mobi";
u8 *ucZTTS_Abell        = "AT+ZTTS=1,\"276b07687F5EDF57F95BB28B3A67\"";

u8 *ucSetParamConfig    = "01000069703D302E302E302E303B69643D31393830303330373437333B7077643D3131313131313B00";
u8 *ucPocOpenConfig     = "0000000101";

void Task_RunStart(void)
{
  u8 v;
  if(BootProcess_SIMST_Flag==1)//收到模块开机指令:SIMST:1
  {
    BEEP_Time(50);
    v=ApiAtCmd_WritCommand(ATCOMM0_OSSYSHWID,(u8 *)ucOSSYSHWID,strlen((char const *)ucOSSYSHWID));
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
  if(ReadInput_KEY_3==0)//按某个按键，当前群组为
  {
    VOICE_SetOutput(ATVOICE_FreePlay,ApiAtCmd_GetMainWorkName(),ApiAtCmd_GetMainWorkNameLen());
  }
  Keyboard_Test();
}

void TASK_WriteFreq(void)
{

}