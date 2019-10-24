#ifndef _SLEEP_H_
#define _SLEEP_H_
#include "stm32f10x.h"


#define SYSTICK_DelayNus(us) sleep_us(us)
#define SYSTICK_DelayNms(us) sleep_ms(us)

extern void SYSTICK_Config(void);
extern void sleep_us(uint32_t us);
extern void sleep_ms(uint16_t ms);



#endif
