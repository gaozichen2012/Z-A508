#ifndef __ADC_H
#define __ADC_H

extern u8 BatteryLevel;
extern void ADC_Init(void);
extern void LowVoltageDetection(void);
extern void KeyBatteryReport(void);
extern bool PrimaryLowPower_Flag;
#endif