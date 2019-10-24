#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "drv_time3.h"
#include "stdio.h"
#include "drv_sleep.h"

//函数执行开关标志 在Tim3_config函数中被赋值，标志着初始化的完成，
//只有完成初始化，才能让TM3_CH2_change函数成功调用
static int fun_ONorOFF_flag=0;
u8 Wd;
u8 on_off;
void Tim3_config(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;

	//使能TIM3和GPIOB和复用时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE); 

	//设置重映射与GPIO的工作模式 （CH2到PB5） 使用部分重映射
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); 
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;                       //TIM_CH2 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;        			//复用推挽输出 
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB,&GPIO_InitStruct);

	//初始化TIM3      该LED灯会在低电平时亮起来
	TIM_TimeBaseStructure.TIM_Period = PERIOD;                     	//设置在自动重装载周期值 
	TIM_TimeBaseStructure.TIM_Prescaler =72-1;                  	//设置预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;					//设置时钟分割:TDTS = Tck_tim //开始的时候它是不会亮的 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//TIM 向上计数模式 
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);      			//③初始化 TIMx 


	//初始化 TIM3 CH2 PWM 模式
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2;          		//选择 PWM 模式 2 
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; 	//比较输出使能 
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;    	//输出极性低 

	TIM_OC2Init(TIM3,&TIM_OCInitStruct);

	//使能预装载值寄存器和TM3
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);    			//使能预装载寄存器 
	on_off=0;
	TIM_Cmd(TIM3, ENABLE);                         			//使能 TIM3 
	
	fun_ONorOFF_flag=1;         //标志着初始化的完成;
	Wd=32;
	RESET_DT();

}




void TM3_CH2_change(void)
{
	static u16 ch2_bj=0;
	static u8  flag=1;
	if(fun_ONorOFF_flag)
	{
		if(flag)
		{
			if(ch2_bj>=PERIOD-1)
			{
				flag=0;
			}
			ch2_bj++;
		}else
		{
			if(ch2_bj<=1)
			{
				flag=1;
			}
			ch2_bj--;
		}
		TIM_SetCompare2(TIM3,ch2_bj);
	}
}
void set_wd(u8 wd)
{
	
	TIM_SetCompare2(TIM3,0);
	int a=0;
	printf("温度：%d\n",Wd);
	if(wd>=16&&wd<=32)
	{
		int b;
		wd-=16;
		b=wd*20;
		
		TIM_SetCompare2(TIM3,b);
	}
	return ;
}
void add_Wd(void)
{
	if(on_off==0)
		return ;
	Wd++;
	if(Wd>32) Wd=32;
	set_wd(Wd);
}
void sub_Wd(void)
{
	if(on_off==0)
	 return ;
	Wd--;
	if(Wd<16) Wd=16;
	set_wd(Wd);
}
u8 Get_Wd(void)
	
{
	return Wd;
}


