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

u8 *ucPocOpenConfig     = "0000000101";
u8 *ucSetParamConfig    = "01000069703D302E302E302E303B69643D31393830303330373437343B7077643D3131313131313B00";
u8 *ucStartPTT          = "0B0000";
u8 *ucEndPTT            = "0C0000";
u8 *ucSwitch            = "10000002";

u8 *ucGroupListInfo     = "0D0000";

void main_app(void)
{
  bool r=FALSE;
u8 Key_Flag_0=0;
u8 t=0;
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
  
  GD83_ON();
  AUDIO_IOAFMUT(ON);
  AUDIO_IOAFPOW(ON);
  GPIO_Init(GPIOB,GPIO_PIN_6,GPIO_MODE_OUT_PP_LOW_FAST);//LOC MIC MUTE
  MIC_IOMUT(OFF); 
  api_lcd_pwr_on_hint("    ABELL    ");

  BEEP_Time(50);
    
/****打开POC应用**********/
  DEL_SetTimer(0,1500);
  while(1){if(DEL_GetTimer(0) == TRUE) {break;}}
  r=ApiPocCmd_WritCommand(PocComm_OpenPOC,ucPocOpenConfig,strlen((char const *)ucPocOpenConfig));
  DEL_SetTimer(0,100);
  DEL_SetTimer(1,100);
/*****************************/
  TaskDrvObj.NewId=Task_Start;
  while(1)
  {
    
    switch(TaskDrvObj.NewId)
    {
    case Task_Start:
      Task_RunStart();
      break;
    case TASK_WRITEFREQ:
      TASK_WriteFreq();
      break;
      
    }
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
  }
}