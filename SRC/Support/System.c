#define SYSABLE
#include "AllHead.h"

#define SYS_IDLE	0x00
#define SYS_RUN		0x01

typedef struct {	
	struct{
		union{
			struct{
				u8 WorkMode	: 2;
				u8 UnlineArm	: 1;
				u8				: 5;
			}Bits;
			u8 Byte;
		}Cfg;
		u8 UnlineArmCountdown;
		u8 Res[3];
	}Par;
	union {
		struct {
			u16 bLoad		: 1;
			u16 bRenew		: 1;
			u16 bTxRxRun	: 1;
			u16 bDebug		: 3;
			u16 bRxNullCh	: 1;
			u16 bTxNullCh	: 1;
			u16 bTalk		: 1;
			u16 bSq		: 1;
			u16 bSqMome	: 1;
			u16 bMoni		: 1;
			u16 bMoniMome	: 1;
			u16 bTot		: 1;
			u16 bUnlineArm	: 1;
			u16 			: 1;
		}Bit;
		u16 Byte;
	}STA;
	union {
		struct {
			u16 TxError		: 1;
			u16 			: 15;
		}Bit;
		u16 Byte;
	}Error;
	u16 cChannel;
	u8 KeyCode;
	POW_MANTYPE   PowMan;
	IO_ONOFF	  PowOnOff;
	TASK_CODE 	  NewId;
	TASK_CODE  	  OldId;
	MSG_STATUS	  Msg;
	SW_CODE 	  Sw;
	u16 UnlineArmTimer;
}TASK_DRV;

static TASK_DRV	TaskDrvObj;

u8 *ucSwitch            = "10000002";
u8 *ucGroupListInfo     = "0D0000";
u8 *ucGD83Reset         = "at^reset";



void main_app(void)
{
  bool r=FALSE;

u8 t=0;

  disableInterrupts();
  SystemClock_Init(HSE_Clock);
  ITC_SetSoftwarePriority(ITC_IRQ_UART1_RX,ITC_PRIORITYLEVEL_3);
  ITC_SetSoftwarePriority(ITC_IRQ_UART1_TX,ITC_PRIORITYLEVEL_3);
  ITC_SetSoftwarePriority(ITC_IRQ_UART3_RX,ITC_PRIORITYLEVEL_2);
  ITC_SetSoftwarePriority(ITC_IRQ_TIM3_OVF,ITC_PRIORITYLEVEL_1);
  LED_Init();
  //定时初始化
  DEL_PowerOnInitial();
  //通讯模块初始化
  DrvGD83_Init();
  Uart1_Init();//模块通讯使用
  Uart3_Init(); //串口写频使用
  //音频初始化
  AF_Mute_Init();
  Noise_Mute_Init();
  MIC_GPIO_Init();
  //BEEP
  BEEP_PowerOnInitial();
  TONE_PowerOnInitial();
  //显示屏
  drv_gt20_pwr_on_init();
  drv_htg_pwr_on_init();
  
  Key_Init();
  Set_RedLed(LED_OFF);
  Set_GreenLed(LED_OFF);
  enableInterrupts();
#if 1//EEPROM TEST
  
  /*ReadBuffer[1]=FLASH_ReadByte(0x4001);
  ReadBuffer[2]=FLASH_ReadByte(0x4081);
  ReadBuffer[3]=FLASH_ReadByte(0x4400);
  ReadBuffer[4]=FLASH_ReadByte(0x47ff);
  WriteEEPROMByte(0x4001, '2');
  WriteEEPROMByte(0x4081, '5');
  WriteEEPROMByte(0x4400, '8');
  WriteEEPROMByte(0x47ff, '0');*/
  

#endif //EEPROMTEST

  GD83_ON();
  AUDIO_IOAFMUT(ON);
  AUDIO_IOAFPOW(ON);
  GPIO_Init(GPIOB,GPIO_PIN_6,GPIO_MODE_OUT_PP_LOW_FAST);//LOC MIC MUTE
  MIC_IOMUT(OFF); 
  api_lcd_pwr_on_hint2("eChat");

  BEEP_Time(1);
  r=ApiAtCmd_WritCommand(ATCOMM3_GD83Reset,(u8 *)ucGD83Reset,strlen((char const *)ucGD83Reset));
  TaskDrvObj.NewId=Task_Start;
  
  
#if 1//测试模块关机
  Set_GreenLed(LED_OFF);
  Set_RedLed(LED_OFF);
  Delay_100ms(100);
  GPIO_WriteHigh(GPIO_C_Reset,GPIO_PIN_C_Reset);
  Set_GreenLed(LED_ON);
  Set_RedLed(LED_ON);
  Delay_100ms(100);
  Set_GreenLed(LED_OFF);
  Set_RedLed(LED_OFF);
  
  
  GPIO_WriteHigh(GPIO_C_ONOFF,GPIO_PIN_C_ONOFF);
  Set_GreenLed(LED_ON);
  Delay_100ms(50);
  GPIO_WriteLow(GPIO_C_ONOFF,GPIO_PIN_C_ONOFF);
  Set_GreenLed(LED_OFF);
  Set_RedLed(LED_ON);
  Delay_100ms(100);
  Set_RedLed(LED_OFF);

  GPIO_WriteHigh(GPIO_C_Reset,GPIO_PIN_C_Reset);
  Delay_100ms(10);
  GPIO_WriteLow(GPIO_C_Reset,GPIO_PIN_C_Reset);
  GPIO_WriteHigh(GPIO_C_ONOFF,GPIO_PIN_C_ONOFF);
  Set_GreenLed(LED_ON);
  Delay_100ms(50);
  GPIO_WriteLow(GPIO_C_ONOFF,GPIO_PIN_C_ONOFF);
  Set_GreenLed(LED_OFF);
  Set_RedLed(LED_ON);
  Delay_100ms(100);
  Set_RedLed(LED_OFF);

  GPIO_WriteHigh(GPIO_C_Reset,GPIO_PIN_C_Reset);
  Delay_100ms(10);
  GPIO_WriteLow(GPIO_C_Reset,GPIO_PIN_C_Reset);
  GPIO_WriteHigh(GPIO_C_ONOFF,GPIO_PIN_C_ONOFF);
  Set_GreenLed(LED_ON);
  Delay_100ms(50);
  GPIO_WriteLow(GPIO_C_ONOFF,GPIO_PIN_C_ONOFF);
  Set_GreenLed(LED_OFF);
  Set_RedLed(LED_ON);
  Delay_100ms(100);
  Set_RedLed(LED_OFF);
#endif

  
  
  
  
  
  
  while(1)
  {
#if 0//调试显示屏界面
    Delay_100ms(100);
    Set_GreenLed(LED_ON);
    Delay_100ms(1);
    Set_GreenLed(LED_OFF);
   /* if(ReadInput_KEY_PTT==0)
    {
      api_disp_icoid_output( eICO_IDRXFULL, TRUE, FALSE);
      api_disp_all_screen_refresh();
    }
    else
    {
        api_disp_icoid_output( eICO_IDRXNULL, TRUE, TRUE);
        api_disp_all_screen_refresh();
    }
    if(ReadInput_KEY_2==0)
    {
      drv_htg_clr_allscr();//清屏
    }
    else
    {}*/
#else

    DEL_Renew();
    switch(TaskDrvObj.NewId)
    {
    case Task_Start:
      Task_RunStart();
      if(ApiAtCmd_GetLoginState()==TRUE)//登录成功
      {
        api_lcd_pwr_on_hint("正在获取群组信息");
        TaskDrvObj.NewId=Task_NormalOperation;
      }
      break;
    case Task_NormalOperation:
      Task_RunNormalOperation();
      break;
    case TASK_WRITEFREQ:
      TASK_WriteFreq();
      break;
    default:
      break;
      
    }
#endif   
    
/*
#if 1//按键控制灯亮灭   
    Keyboard_Test();
#endif

    
#if 1//对讲&换组
    DEL_Renew();
    ListenState();//接听亮绿灯
    if(ReadInput_KEY_PTT==0)
    {
      Set_RedLed(LED_ON);
      
      r=ApiPocCmd_WritCommand(PocComm_StartPTT,ucStartPTT,strlen((char const *)ucStartPTT));
      AUDIO_IOAFPOW(OFF);
      while(ReadInput_KEY_PTT==0);
      r=ApiPocCmd_WritCommand(PocComm_EndPTT,ucEndPTT,strlen((char const *)ucEndPTT));
      AUDIO_IOAFPOW(ON);
    }
    else
    {
      if(ReadInput_KEY_4==0)
      {
        r=ApiPocCmd_WritCommand(PocComm_Key,ucSwitch,strlen((char const *)ucSwitch));
        Key_Flag_0=1;
      }
      else
      {
      if(ReadInput_KEY_2==0)
      {
        api_lcd_pwr_on_hint("欧标按键:K2 退出单呼     ");
        r=ApiPocCmd_WritCommand(PocComm_EndPTT,"0A0000ffffffff",strlen((char const *)"0A0000ffffffff"));
        Key_Flag_0=1;
      }
      if(ReadInput_KEY_3==0)
      {
#if 1//1:一号机烧录  0:三号机烧录
        api_lcd_pwr_on_hint("按键:K3 单呼       ");
        r=ApiPocCmd_WritCommand(PocComm_EndPTT,"0E000000000001",strlen((char const *)"0E000000000001"));
        Key_Flag_0=1;
        DEL_SetTimer(1,100);
        while(1)
        {
          if(DEL_GetTimer(1) == TRUE) {break;}
        }
        
        r=ApiPocCmd_WritCommand(PocComm_EndPTT,"0A00000000006401",strlen((char const *)"0A00000000006401"));
#else//
        api_lcd_pwr_on_hint("按键:K3 单呼一号   ");
        r=ApiPocCmd_WritCommand(PocComm_EndPTT,"0E000000000001",strlen((char const *)"0E000000000001"));
        Key_Flag_0=1;
        DEL_SetTimer(1,20);
        while(1)
        {
          if(DEL_GetTimer(1) == TRUE) {break;}
        }
        
        r=ApiPocCmd_WritCommand(PocComm_EndPTT,"0A00000000006401",strlen((char const *)"0A00000000006401"));
#endif
      }
      }
      if(Key_Flag_0==1)
      {
        DEL_SetTimer(0,200);
        while(1)
        {
          Key_Flag_0=0;
          if(DEL_GetTimer(0) == TRUE) {break;}
        }
      }
     
      Set_RedLed(LED_OFF);
      Set_GreenLed(LED_OFF);
    }

#endif
  }*/
}
}

void SYS_McuReset(void)
{
	POW_SetPowSwitch(OFF);
	DEL_Soft1ms(8000);

}

TASK_CODE GetTaskId(void)
{
  return TaskDrvObj.NewId;
}