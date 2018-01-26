#include "AllHead.h"
u8 LED_Conut=0;
void LED_IntOutputRenew(void)
{

  if( GetTaskId()==Task_Start)
  {
    LED_Conut++;
    if(LED_Conut==5)
    {
      Set_GreenLed(LED_ON);
    }
    if(LED_Conut==8)
    {
      Set_GreenLed(LED_OFF);
      LED_Conut=0;
    }
  }
  else
  {
    if(POC_ReceivedVoice_Flag==TRUE)
    {
      Set_GreenLed(LED_ON);
    }
    else
    {
      LED_Conut++;
      if(LED_Conut>=29)
      {
        if(PositionInfoSendToATPORT_RedLed_Flag==TRUE)
        {
          Set_RedLed(LED_ON);
        }
        else
        {
          Set_GreenLed(LED_ON);
        }
      }
      if(LED_Conut>=30)
      {
        PositionInfoSendToATPORT_RedLed_Flag=FALSE;
        Set_GreenLed(LED_OFF);
        Set_RedLed(LED_OFF);
        LED_Conut=0;
      }
    }
  }
}