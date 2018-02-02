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
    api_lcd_pwr_on_hint5("1/8");
     api_lcd_pwr_on_hint("群组选择        ");
    break;
  case Menu2:
    api_lcd_pwr_on_hint3("菜单            ");
    api_lcd_pwr_on_hint5("2/8");
     api_lcd_pwr_on_hint("成员选择        ");
    break;
  case Menu3:
    api_lcd_pwr_on_hint3("菜单            ");
    api_lcd_pwr_on_hint5("3/8");
     api_lcd_pwr_on_hint("背光灯设置      ");
     break;
  case Menu4:
    api_lcd_pwr_on_hint3("菜单            ");
    api_lcd_pwr_on_hint5("4/8");
     api_lcd_pwr_on_hint("北斗/写频切换   ");
     break;
  case Menu5:
    api_lcd_pwr_on_hint3("菜单            ");
    api_lcd_pwr_on_hint5("5/8");
     api_lcd_pwr_on_hint("GPS信息         ");
     break;
  case Menu6:
    api_lcd_pwr_on_hint3("菜单            ");
    api_lcd_pwr_on_hint5("6/8");
     api_lcd_pwr_on_hint("网络模式        ");
     break;
  case Menu7:
    api_lcd_pwr_on_hint3("菜单            ");
    api_lcd_pwr_on_hint5("7/8");
     api_lcd_pwr_on_hint("语言设置        ");
    break;
  case Menu8:
    api_lcd_pwr_on_hint3("菜单            ");
    api_lcd_pwr_on_hint5("8/8");
     api_lcd_pwr_on_hint("语音播报        ");
    break;
  case Menu_Locking_NoOperation:
    api_lcd_pwr_on_hint3("        ");//清屏，防止显示“个呼短号”
     api_lcd_pwr_on_hint("                ");
     api_disp_icoid_output( eICO_IDRXFULL, TRUE, TRUE);//GPRS三格信号图标
     if(NetworkType_2Gor3G_Flag==3)
       api_disp_icoid_output( eICO_IDEmergency, TRUE, TRUE);//3G图标
     else
       api_disp_icoid_output( eICO_IDPOWERL, TRUE, TRUE);//图标：2G
    api_lcd_pwr_on_hint(HexToChar_MainGroupId());//显示当前群组ID
    api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//显示当前群组昵称
    
    break;
  case Menu_UnlockStep1_Ok:
    //api_lcd_pwr_on_hint3("        ");//清屏，防止显示“个呼短号”
    api_disp_icoid_output( eICO_IDRXFULL, TRUE, TRUE);//GPRS三格信号图标
    if(NetworkType_2Gor3G_Flag==3)
      api_disp_icoid_output( eICO_IDEmergency, TRUE, TRUE);//3G图标
    else
      api_disp_icoid_output( eICO_IDPOWERL, TRUE, TRUE);//图标：2G
     api_lcd_pwr_on_hint("再按*键         ");
    break;
  default:
    break;
  }
}