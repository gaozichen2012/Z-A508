#include "AllHead.h"

void LED_Test(void)
{
  if(ReadInput_PIN_RSSI==0)
  {
    Set_GreenLed(LED_OFF);
  }
  else
  {
     Set_GreenLed(LED_ON);
  }
}