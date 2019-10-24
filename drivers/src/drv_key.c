//PE4 上拉
#include "stm32f10x_gpio.h"
#include "drv_key.h"
#include "drv_sleep.h"
#include "drv_basetim6.h"
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);//使能PORTA,PORTE时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;//KEY0-KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE2,3,4

	//初始化 WK_UP-->GPIOA.0	  下拉输入
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0设置成输入，默认下拉	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.0	
}


//当mode 为0时，不进行抬起检测
//当mode 为1时，进行抬起检测，当按键真正的抬起时，才把函数返回
u8 KEY_Scan(u8 mode)
{	  
	u32 ms=0;
	
	if((KEY0==0||KEY1==0||KEY2==0||WK_UP==1))
	{
		ms=Time_ms;
		
		sleep_ms(10);//去抖动 
		if(KEY0==0)      
		{
			
			while(mode&&KEY0==0);
			if(Time_ms-ms>1000)
				return KEY0_PRES_long;
			return KEY0_PRES;
		}
		else if(KEY1==0) 
		{	
			while(mode&&KEY1==0);
			if(Time_ms-ms>1000)
				return KEY1_PRES_long;
			return KEY1_PRES;
		}
		else if(KEY2==0) 
		{
			while(mode&&KEY2==0);
			if(Time_ms-ms>1000)
				return KEY2_PRES_long;
			return KEY2_PRES;
		}
		else if(WK_UP==1)
		{
			while(mode&& WK_UP==1);
			if(Time_ms-ms>1000)
				return WKUP_PRES_long;
			return WKUP_PRES;
		}
	}
 	return 0;// 无按键按下
}

