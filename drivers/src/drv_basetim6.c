#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "misc.h"
#include "drv_basetim6.h"
volatile u32 Time_ms=0;
volatile u32 Time_15ms=0;


void BaseTim6_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	//使能外设
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);

	//2、初始化基本定时器(定1ms)
	TIM_TimeBaseInitStruct.TIM_Period = 10;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7200-1;
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStruct);

	//3、使能定中段源 （更新中断）
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);

	//4、配置中断优先级
	NVIC_InitStruct.NVIC_IRQChannel=TIM6_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStruct);

	// 3、使能基本定时器
	TIM_Cmd(TIM6, ENABLE);

}



void BaseTim7_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	//使能外设
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);

	//2、初始化基本定时器 (15毫秒一次中断)
	TIM_TimeBaseInitStruct.TIM_Period = 10000;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7200-1;
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStruct);

	//3、使能定中段源 （更新中断）
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);

	//4、配置中断优先级
	NVIC_InitStruct.NVIC_IRQChannel=TIM7_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStruct);

	// 3、使能基本定时器
	TIM_Cmd(TIM7, ENABLE);

}




















