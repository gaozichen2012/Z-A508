#include "AllHead.h"

u8 BatteryLevel=0;
bool LowVoltageDetection_Flag;

static u16 OneChannelGetADValue(ADC2_Channel_TypeDef ADC2_Channel,\
  ADC2_SchmittTrigg_TypeDef ADC2_SchmittTriggerChannel);
void ADC_Init(void)
{
  ADC2_Init(ADC2_CONVERSIONMODE_CONTINUOUS , ADC2_CHANNEL_2, ADC2_PRESSEL_FCPU_D18,\
    ADC2_EXTTRIG_TIM, DISABLE, ADC2_ALIGN_RIGHT, ADC2_SCHMITTTRIG_CHANNEL10,DISABLE);
  ADC2_Cmd(ENABLE);
}

//ADC2单通道选择读取AD值
static u16 OneChannelGetADValue(ADC2_Channel_TypeDef ADC2_Channel,\
  ADC2_SchmittTrigg_TypeDef ADC2_SchmittTriggerChannel)
{
  uint16_t ADConversion_Value;
  /**< 连续转换模式 */
  /**< 使能通道 */
  /**< ADC时钟：fADC2 = fcpu/18 */
  /**< 这里设置了从TIM TRGO 启动转换，但实际是没有用到的*/
  /**  不使能 ADC2_ExtTriggerState**/
  /**< 转换数据右对齐 */
  /**< 不使能通道10的斯密特触发器 */
  /**  不使能通道10的斯密特触发器状态 */
  ADC2_Init(ADC2_CONVERSIONMODE_CONTINUOUS , ADC2_Channel, ADC2_PRESSEL_FCPU_D18,\
    ADC2_EXTTRIG_TIM, DISABLE, ADC2_ALIGN_RIGHT, ADC2_SchmittTriggerChannel,DISABLE);
  ADC2_Cmd(ENABLE);
  ADC2_StartConversion();
  ADConversion_Value = ADC2_GetConversionValue();
  return ADConversion_Value;
}
void LowVoltageDetection(void)
{
  u16 ADValue;
  ADValue=OneChannelGetADValue(ADC2_CHANNEL_2,ADC2_SCHMITTTRIG_CHANNEL2);
  
  if(GetTaskId()==Task_Start)
  {}
  else
  {
    if(ADValue<=340&&ADValue>=300)
    {
      api_disp_icoid_output( eICO_IDBATT, TRUE, TRUE);//电池电量3级
      SetTaskId(TASK_LOBATT);
      
      LowVoltageDetection_Flag=TRUE;
    }
    else
    {
      if(ADValue<=350&&ADValue>=300)
      {
        api_disp_icoid_output( eICO_IDBATT , TRUE, TRUE);
        BatteryLevel=0;
      }//电池电量0级
      else if(ADValue<=360&&ADValue>350)
      {
        api_disp_icoid_output( eICO_IDBATT1, TRUE, TRUE);
        BatteryLevel=1;
      }//电池电量1级
      else if(ADValue<=375&&ADValue>365)
      {
        api_disp_icoid_output( eICO_IDBATT2, TRUE, TRUE);
        BatteryLevel=2;
      }//电池电量2级
      else if(ADValue<=390&&ADValue>380)
      {
        api_disp_icoid_output( eICO_IDBATT3, TRUE, TRUE);
        BatteryLevel=3;
      }//电池电量3级
      else if(ADValue<=405&&ADValue>395)
      {
        api_disp_icoid_output( eICO_IDBATT4, TRUE, TRUE);
        BatteryLevel=4;
      }//电池电量4级
      else if(ADValue<=500&&ADValue>410)
      {
        api_disp_icoid_output( eICO_IDBATT5, TRUE, TRUE);
        BatteryLevel=5;
      }//电池电量5级
      else{}
      if(LowVoltageDetection_Flag==1)//识别从低电量到高电量的状态
      {
        ApiPocCmd_WritCommand(PocComm_Cancel,(u8 *)ucQuitPersonalCalling,strlen((char const *)ucQuitPersonalCalling));
        LowVoltageDetection_Flag=FALSE;
      }
      SetTaskId(Task_NormalOperation);
    }
  }
}

void KeyBatteryReport(void)
{
  switch(BatteryLevel)
  {
  case 0:
    VOICE_SetOutput(ATVOICE_FreePlay,"3575cf917e7606524b4e3500",24);//百分之五
    break;
  case 1:
    VOICE_SetOutput(ATVOICE_FreePlay,"3575cf917e7606524b4e32004153",28);//百分之20
    break;
  case 2:
    VOICE_SetOutput(ATVOICE_FreePlay,"3575cf917e7606524b4e34004153",28);//百分之40
    break;
  case 3:
    VOICE_SetOutput(ATVOICE_FreePlay,"3575cf917e7606524b4e36004153",28);//百分之60
    break;
  case 4:
    VOICE_SetOutput(ATVOICE_FreePlay,"3575cf917e7606524b4e38004153",28);//百分80
    break;
  case 5:
    VOICE_SetOutput(ATVOICE_FreePlay,"3575cf917e7606527e76",20);//百分百
    break;
  default:
    break;
  }

}