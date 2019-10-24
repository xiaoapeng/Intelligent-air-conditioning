#ifndef _DRV_BASETIM_H_
#define _DRV_BASETIM_H_
#include "stm32f10x.h"

#define TIMER_IRQ_FUN TIM6_IRQHandler
extern volatile u32 Time_ms;
extern volatile u32 Time_15ms;


extern void BaseTim7_Config(void);
extern void BaseTim6_Config(void);
#endif


















