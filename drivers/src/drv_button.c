
#include "stm32f10x_gpio.h"
#include "drv_sleep.h"
#include "stm32f10x_exti.h"
#include "misc.h"

void BUTTON_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	// 1�����������ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE| RCC_APB2Periph_AFIO, ENABLE);//ʹ��GPIOE�͸���ʱ��AFIO

	// 2������IO�Ĺ���ģʽ
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;//pin4����
	GPIO_InitStruct.GPIO_Mode =GPIO_Mode_IPU ;//����//GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOE, &GPIO_InitStruct);//��ʼ��GPIO

	// 3�������ⲿ���ŵ�ӳ��
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource4);//��GPIOE pin4ӳ�䵽�ⲿ�ж���4��
	
	// 4������EXTI��������Ӧ�Ĺ�������
	EXTI_InitStruct.EXTI_Line = EXTI_Line4;//�ⲿ�ж���4
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt; //����Ϊ�ж�
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;//����Ϊ�����ش���
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;//ʹ��
	EXTI_Init(&EXTI_InitStruct);

	// 5��������Ӧ���ж����ȼ�
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
		// ��ʱ����
		sleep_ms(10);
		if (RESET == GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4))
		{
			ret = 1;
			// �ȴ������ͷ�
			//while (RESET == GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0));
		}
	}

	return ret;
}

