#define TASKABLE
#include "AllHead.h"

u8 *ucOSSYSHWID         = "AT^OSSYSHWID=1";
u8 *ucPPPCFG            = "AT^PPPCFG=echat,ctnet@mycdma.cn,vnet.mobi";

u8 *ucSetParamConfig    = "01000069703D302E302E302E303B69643D31393830303330373437343B7077643D3131313131313B00";
u8 *ucPocOpenConfig     = "0000000101";

void Task_RunStart(void)
{
  u8 v;
  if(BootProcess_SIMST_Flag==1)//收到模块开机指令:SIMST:1
  {
    v=ApiAtCmd_WritCommand(ATCOMM0_OSSYSHWID,(u8 *)ucOSSYSHWID,strlen((char const *)ucOSSYSHWID));
    BootProcess_SIMST_Flag=0;
    v=ApiAtCmd_WritCommand(ATCOMM1_PPPCFG,(u8 *)ucPPPCFG,strlen((char const *)ucPPPCFG));//1.发送PPPCFG
  }
  if(BootProcess_PPPCFG_Flag==1)//如果收到^PPPCFG
  {
    BootProcess_PPPCFG_Flag=0;
    ApiPocCmd_WritCommand(PocComm_SetParam,ucSetParamConfig,strlen((char const *)ucSetParamConfig));//配置echat账号、IP
    DEL_SetTimer(0,500);
    while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
    v=ApiPocCmd_WritCommand(PocComm_OpenPOC,ucPocOpenConfig,strlen((char const *)ucPocOpenConfig));
  }
  if(ApiAtCmd_GetLoginState()==TRUE)//登录成功
  {
    BEEP_Time(50);
  }
  
   /* if()//如果收到^PPPCFG
    {
    //2.
      if()//收到+POC:00000000
      {
        //3.打开echat应用
      }
    }
    if()//+POC:9000 不需要更新
    {
      //等待登录
      if()//登录成功
      {
        if()//进入群组
        {
          //退出Task_Start标志位，进入下一标志位
        }
      }
    }
    else//90001或90002
    {
      //进入Task_Start标志位
    }
    */
 
}

void TASK_WriteFreq(void)
{

}