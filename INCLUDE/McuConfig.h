#ifndef __MCUCONFIG_H
#define __MCUCONFIG_H

#include "stm8s.h"
typedef	enum {
  OFF= 0x00,
  ON= 0x01
}IO_ONOFF;

/******************************************************************************
;--------1-LED hardware macro define
******************************************************************************/
#define GPIO_LED_Green          GPIOB
#define GPIO_LED_Red            GPIOD

#define GPIO_PIN_LED_Green      GPIO_PIN_3
#define GPIO_PIN_LED_Red        GPIO_PIN_2

#define LED_ON  0
#define LED_OFF 1

/******************************************************************************
;--------2-TIM3:DELAY hardware macro define
******************************************************************************/
#define HSE_CLK

/******************************************************************************
;--------3-UART1:GD83 hardware macro define
******************************************************************************/
#define GPIO_C_Reset            GPIOA 
#define GPIO_C_ONOFF            GPIOA 

#define GPIO_PIN_C_Reset        GPIO_PIN_3
#define GPIO_PIN_C_ONOFF        GPIO_PIN_6

/******************************************************************************
;--------4-Audio&MIC hardware macro define
******************************************************************************/
#define GPIO_AF_Mute            GPIOG
#define GPIO_MIC_Mute           GPIOD
#define GPIO_Noise_Mute         GPIOD

#define GPIO_PIN_AF_Mute        GPIO_PIN_0
#define GPIO_PIN_MIC_Mute       GPIO_PIN_4
#define GPIO_PIN_Noise_Mute     GPIO_PIN_7

/******************************************************************************
;--------5-KEY hardware macro define
******************************************************************************/
#define GPIO_Key_2              GPIOB
#define GPIO_Key_3              GPIOB
#define GPIO_Key_PTT            GPIOE

#define GPIO_PIN_Key_2          GPIO_PIN_6
#define GPIO_PIN_Key_3          GPIO_PIN_5
#define GPIO_PIN_Key_PTT        GPIO_PIN_5

#define ReadInput_KEY_2         GPIO_ReadInputPin(GPIO_Key_2, GPIO_PIN_Key_2)
#define ReadInput_KEY_3         GPIO_ReadInputPin(GPIO_Key_3, GPIO_PIN_Key_3)
#define ReadInput_KEY_PTT       GPIO_ReadInputPin(GPIO_Key_PTT, GPIO_PIN_Key_PTT)

/******************************************************************************
;--------1-LED hardware macro define
******************************************************************************/
void LED_Init(void);
void Set_GreenLed(u8 state);
void Set_RedLed(u8 state);

/******************************************************************************
;--------2-TIM3:DELAY hardware macro define
******************************************************************************/
void Tim3_Timer_Init(void);
/*void TimingDelay_Decrement(void);
void Delay_ms(u16 nms);*/

/******************************************************************************
;--------3-UART1:GD83 hardware macro define
******************************************************************************/
void DrvGD83_Init(void);
void GD83_ON(void);

void Uart1_Init(void);
u8 UART1_ReceiveByte(void);
/******************************************************************************
;--------4-Audio&MIC hardware macro define
******************************************************************************/
void MIC_GPIO_Init(void);
void AF_Mute_Init(void);
void Noise_Mute_Init(void);

void C_TEST_OUT_SET(void);

void MIC_IOMUT(IO_ONOFF OnOff);
void AUDIO_IOAFMUT(IO_ONOFF OnOff);
void AUDIO_IOAFPOW(IO_ONOFF OnOff);

/******************************************************************************
;--------5-KEY hardware macro define
******************************************************************************/
void Key_Init(void);
#endif