#define SYSABLE
#include "AllHead.h"
u8 *ucPocOpenConfig     = "0000000101";
u8 *ucSetParamConfig    = "01000069703D302E302E302E303B69643D31393830303330373437343B7077643D3131313131313B00";
u8 *ucStartPTT          = "0B0000";
u8 *ucEndPTT            = "0C0000";
u8 *ucSwitch            = "10000002";
u8 *ucKeyUp                = "10000003";
u8 *ucKeyDown              = "10000004";
u8 *ucGroupListInfo     = "0D0000";

void main_app(void)
{
  bool r=FALSE;
  u8 Key_Flag=0;
  disableInterrupts();
  SystemClock_Init(HSE_Clock);
  ITC_SetSoftwarePriority(ITC_IRQ_UART1_RX,ITC_PRIORITYLEVEL_3);
  ITC_SetSoftwarePriority(ITC_IRQ_UART1_TX,ITC_PRIORITYLEVEL_3);
  ITC_SetSoftwarePriority(ITC_IRQ_TIM3_OVF,ITC_PRIORITYLEVEL_2);
  LED_Init();
  //定时初始化
  DEL_PowerOnInitial();
  //通讯模块初始化
  DrvGD83_Init();
  Uart1_Init();
  //音频初始化
  AF_Mute_Init();
  Noise_Mute_Init();
  MIC_GPIO_Init();
  Key_Init();
  Set_RedLed(LED_OFF);
  Set_GreenLed(LED_OFF);
  //enableInterrupts();
  
  GD83_ON();
  AUDIO_IOAFMUT(ON);
  AUDIO_IOAFPOW(ON);
  MIC_IOMUT(OFF); 
  r=ApiPocCmd_WritCommand(PocComm_OpenPOC,ucPocOpenConfig,strlen((char const *)ucPocOpenConfig));
    //DEL_SetTimer(1,100);
  while(1)
  {
    Keyboard_Test();
   /* DEL_Renew();
    ListenState();//接听亮绿灯
    if(ReadInput_KEY_PTT==0)
    {
      Set_RedLed(LED_ON);
      
      r=ApiPocCmd_WritCommand(PocComm_StartPTT,ucStartPTT,strlen((char const *)ucStartPTT));
      while(ReadInput_KEY_PTT==0);
      r=ApiPocCmd_WritCommand(PocComm_EndPTT,ucEndPTT,strlen((char const *)ucEndPTT));
    }
    else
    {
      if(ReadInput_KEY_4==0)
      {
        r=ApiPocCmd_WritCommand(PocComm_Key,ucSwitch,strlen((char const *)ucSwitch));
        Key_Flag=1;
      }
      else
      {
      if(ReadInput_KEY_2==0)
      {
        r=ApiPocCmd_WritCommand(PocComm_Key,ucKeyUp,strlen((char const *)ucKeyUp));
        Key_Flag=1;
      }
      if(ReadInput_KEY_3==0)
      {
        r=ApiPocCmd_WritCommand(PocComm_Key,ucKeyDown,strlen((char const *)ucKeyDown));
        Key_Flag=1;
      }
      }
      if(Key_Flag==1)
      {
        DEL_SetTimer(0,200);
        while(1)
        {
          Key_Flag=0;
          if(DEL_GetTimer(0) == TRUE) {break;}
        }
      }
      Set_RedLed(LED_OFF);
      Set_GreenLed(LED_OFF);
    }*/
  }
}