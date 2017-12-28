#include "AllHead.h"

void ADC_Init(void)
{
  ADC2_Init(ADC2_CONVERSIONMODE_CONTINUOUS , ADC2_CHANNEL_2, ADC2_PRESSEL_FCPU_D18,\
    ADC2_EXTTRIG_TIM, DISABLE, ADC2_ALIGN_RIGHT, ADC2_SCHMITTTRIG_CHANNEL10,DISABLE);
  ADC2_Cmd(ENABLE);
}

//ADC2单通道选择读取AD值
 u16 OneChannelGetADValue(ADC2_Channel_TypeDef ADC2_Channel,\
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