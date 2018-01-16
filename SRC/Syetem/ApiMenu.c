#include "AllHead.h"

void MenuDisplay(MenuDisplayType id)
{
  switch(id)
  {
  case Menu0:
    api_disp_icoid_output( eICO_IDRXFULL, TRUE, TRUE);//GPRS三格信号图标
    api_disp_icoid_output( eICO_IDEmergency, TRUE, TRUE);//3G图标
    break;
  case Menu1:
    api_lcd_pwr_on_hint3("菜单            ");
    api_lcd_pwr_on_hint5("1/3");
     api_lcd_pwr_on_hint("背光灯时间      ");
    break;
  case Menu2:
    api_lcd_pwr_on_hint3("菜单            ");
    api_lcd_pwr_on_hint5("2/3");
     api_lcd_pwr_on_hint("键盘锁          ");
    break;
  case Menu3:
    api_lcd_pwr_on_hint3("菜单            ");
    api_lcd_pwr_on_hint5("3/3");
     api_lcd_pwr_on_hint("GPS信息         ");
    break;
  case Menu_Lock:
    api_lcd_pwr_on_hint3("菜单            ");
    api_lcd_pwr_on_hint5("3/3");
     api_lcd_pwr_on_hint("GPS信息         ");
    break;
  default:
    break;
  }
}