#include "AllHead.h"

#if 1
u8 ApiMenu_GpsInfo_Flag=0;
#endif


void MenuDisplay(MenuDisplayType id)
{
  switch(id)
  {
  case Menu0:
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
    api_lcd_pwr_on_hint5("5/8");
     api_lcd_pwr_on_hint("GPS信息         ");
     break;
  case Menu4:
    api_lcd_pwr_on_hint3("菜单            ");
    api_lcd_pwr_on_hint5("3/8");
     api_lcd_pwr_on_hint("背光灯设置      ");
     break;
  case Menu5:
    api_lcd_pwr_on_hint3("菜单            ");
    api_lcd_pwr_on_hint5("6/8");
     api_lcd_pwr_on_hint("网络模式        ");
     break;
  case Menu6:
    api_lcd_pwr_on_hint3("菜单            ");
    api_lcd_pwr_on_hint5("7/8");
     api_lcd_pwr_on_hint("本机信息        ");
    break;
  case Menu7:
    api_lcd_pwr_on_hint3("菜单            ");
    api_lcd_pwr_on_hint5("4/8");
     api_lcd_pwr_on_hint("北斗/写频切换   ");
     break;
/*  case Menu8:
    api_lcd_pwr_on_hint3("菜单            ");
    api_lcd_pwr_on_hint5("8/8");
     api_lcd_pwr_on_hint("语音播报        ");
    break;*/
  case Menu_Locking_NoOperation:
    api_lcd_pwr_on_hint("                ");//清屏
    MenuDisplay(Menu_RefreshAllIco);
    api_lcd_pwr_on_hint(HexToChar_MainGroupId());//显示当前群组ID
    api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//显示当前群组昵称
    break;
  case Menu_unLocking:
    MenuDisplay(Menu_RefreshAllIco);
    api_lcd_pwr_on_hint("                ");//清屏
     api_lcd_pwr_on_hint(HexToChar_MainGroupId());//显示当前群组ID
     api_lcd_pwr_on_hint4(UnicodeForGbk_MainWorkName());//显示当前群组昵称
    break;
  case Menu_RefreshAllIco:
    api_lcd_pwr_on_hint3("                ");//清屏
    api_disp_icoid_output( eICO_IDRXFULL, TRUE, TRUE);//GPRS三格信号图标
    if(NetworkType_2Gor3G_Flag==3)
      api_disp_icoid_output( eICO_IDEmergency, TRUE, TRUE);//3G图标
    else
      api_disp_icoid_output( eICO_IDPOWERL, TRUE, TRUE);//图标：2G
    if(VoiceType_FreehandOrHandset_Flag==0)
      api_disp_icoid_output( eICO_IDTemper, TRUE, TRUE);//免提模式
    else
      api_disp_icoid_output( eICO_IDMONITER, TRUE, TRUE);//听筒模式图标
    if(LockingState_Flag==FALSE)
      api_disp_icoid_output( eICO_IDBANDWIDTHN, TRUE, TRUE);//无锁屏空图标
    else
      api_disp_icoid_output( eICO_IDBANDWIDTHW, TRUE, TRUE);//锁屏图标
    if(PersonCallIco_Flag==0)
      api_disp_icoid_output( eICO_IDPOWERM, TRUE, TRUE);//显示组呼图标
    else
      api_disp_icoid_output( eICO_IDPOWERH, TRUE, TRUE);//显示个呼图标
    api_disp_icoid_output( BatteryLevel, TRUE, TRUE);//电池电量图标
    break;
  case Menu_UnlockStep1_Ok:
    //MenuDisplay(Menu_RefreshAllIco);
    api_lcd_pwr_on_hint("                ");//清屏
    api_lcd_pwr_on_hint("再按*键         ");
    break;
  default:
    break;
  }
}

void SubmenuMenuDisplay(SubmenuMenuDisplayType id)
{
  u8 Buf1[16];//={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  u8 Buf2[16];//={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  switch(id)
  {
  case GpsInfoMenu:
     api_lcd_pwr_on_hint("                ");//清屏
    api_lcd_pwr_on_hint3("                ");//清屏
    if(PositionInfoSendToATPORT_InfoDisplay_Flag==FALSE)//未定位显示000
    {
      api_lcd_pwr_on_hint3("经度:00.000000  ");//清屏
       api_lcd_pwr_on_hint("纬度:000.000000 ");//清屏
    }
    else
    {
    //Longitude经度Data_Latitude_Minute()*1000000+Data_Latitude_Second();
    //Latitudew纬度Data_Longitude_Minute()*1000000+Data_Longitude_Second();
    //换算并显示经度
    Buf1[0]=0xbe;
    Buf1[1]=0xad;
    Buf1[2]=0xb6;
    Buf1[3]=0xc8;
    Buf1[4]=0x3a;
    COML_DecToAsc(Data_Latitude_Minute(), Buf1+5);
    COML_StringReverse(3,Buf1+5);
    Buf1[8]=0x2e;
    COML_DecToAsc(Data_Latitude_Second(), Buf1+9);
    COML_StringReverse(6,Buf1+9);
    api_lcd_pwr_on_hint3(Buf1);
    //换算并显示纬度
    Buf2[0]=0xce;
    Buf2[1]=0xb3;
    Buf2[2]=0xb6;
    Buf2[3]=0xc8;
    Buf2[4]=0x3a;
    COML_DecToAsc(Data_Longitude_Minute(), Buf2+5);
    COML_StringReverse(2,Buf2+5);
    Buf2[7]=0x2e;
    COML_DecToAsc(Data_Longitude_Second(), Buf2+8);
    COML_StringReverse(6,Buf2+8);
    api_lcd_pwr_on_hint(Buf2);
    }
    break;
  }
}
