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
    if(GetPlayState()==1)
    {
      Set_GreenLed(LED_ON);
    }
    else
    {
    LED_Conut++;
    if(LED_Conut==20)
    {
      Set_GreenLed(LED_ON);
    }
    if(LED_Conut==23)
    {
      Set_GreenLed(LED_OFF);
      LED_Conut=0;
    }
    }
  }
}