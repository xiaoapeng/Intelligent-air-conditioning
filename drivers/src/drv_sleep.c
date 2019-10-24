#include "stm32f10x.h"
#include "drv_delay.h"
#include "drv_sleep.h"

void SYSTICK_Config()
{
	delay_init();
}

void sleep_ms(uint16_t ms)
{
	uint16_t i=0;
	uint16_t count=ms/1000;
	for(i=0;i<count;i++)
	{
		delay_ms(1000);
	}
	if(ms%1000)
	{
		delay_ms(ms%1000);
	}
}


void sleep_us(uint32_t us)
{
	delay_us(us);
}
