
#include "stm32f10x_gpio.h"
#include "drv_sleep.h"
#include "stm32f10x_exti.h"
#include "misc.h"

void BUTTON_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	// 1、开启外设的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE| RCC_APB2Periph_AFIO, ENABLE);//使能GPIOE和复用时钟AFIO

	// 2、配置IO的工作模式
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;//pin4引脚
	GPIO_InitStruct.GPIO_Mode =GPIO_Mode_IPU ;//上拉//GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOE, &GPIO_InitStruct);//初始化GPIO

	// 3、配置外部引脚的映射
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource4);//将GPIOE pin4映射到外部中断线4上
	
	// 4、配置EXTI控制器相应的工作参数
	EXTI_InitStruct.EXTI_Line = EXTI_Line4;//外部中断线4
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt; //设置为中断
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;//设置为上升沿触发
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;//使能
	EXTI_Init(&EXTI_InitStruct);

	// 5、配置相应的中断优先级
	NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQn;//
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	
	
}

uint8_t BUTTON_IsButtonPress(void)
{
	uint8_t ret = 0;

	if (RESET == GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4))
	{
		// 延时消抖
		sleep_ms(10);
		if (RESET == GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4))
		{
			ret = 1;
			// 等待按键释放
			//while (RESET == GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0));
		}
	}

	return ret;
}

