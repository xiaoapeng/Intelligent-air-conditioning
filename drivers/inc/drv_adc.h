#ifndef __DRV_ADC_H__
#define __DRV_ADC_H__
#include "stm32f10x.h"
void  Adc3_Init(void);
u16 Get_Adc3(u8 ch) ;
void  Adc_Init(void);
u16 Get_Adc_Average(u8 ch,u8 times);
u16 Get_Adc(u8 ch);
#endif // __DRV_ADC_H__

