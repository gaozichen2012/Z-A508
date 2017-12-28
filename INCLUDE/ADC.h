#ifndef __ADC_H
#define __ADC_H

void ADC_Init(void);
u16 OneChannelGetADValue(ADC2_Channel_TypeDef ADC2_Channel,\
  ADC2_SchmittTrigg_TypeDef ADC2_SchmittTriggerChannel);

#endif