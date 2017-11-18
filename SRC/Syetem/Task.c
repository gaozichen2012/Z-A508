#define TASKABLE
#include "AllHead.h"

void Task_RunStart(void)
{
  if(SIMST_Flag==1)//收到模块开机指令:SIMST:1
  {
    BEEP_Time(50);
    SIMST_Flag==0;
    //1.发送PPPCFG
   /* if()//如果收到^PPPCFG
    {
    //2.配置echat账号、IP
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
}

void TASK_WriteFreq(void)
{

}