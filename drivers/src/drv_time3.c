#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "drv_time3.h"
#include "stdio.h"
#include "drv_sleep.h"

//����ִ�п��ر�־ ��Tim3_config�����б���ֵ����־�ų�ʼ������ɣ�
//ֻ����ɳ�ʼ����������TM3_CH2_change�����ɹ�����
static int fun_ONorOFF_flag=0;
u8 Wd;
u8 on_off;
void Tim3_config(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;

	//ʹ��TIM3��GPIOB�͸���ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE); 

	//������ӳ����GPIO�Ĺ���ģʽ ��CH2��PB5�� ʹ�ò�����ӳ��
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); 
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;                       //TIM_CH2 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;        			//����������� 
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB,&GPIO_InitStruct);

	//��ʼ��TIM3      ��LED�ƻ��ڵ͵�ƽʱ������
	TIM_TimeBaseStructure.TIM_Period = PERIOD;                     	//�������Զ���װ������ֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =72-1;                  	//����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;					//����ʱ�ӷָ�:TDTS = Tck_tim //��ʼ��ʱ�����ǲ������� 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//TIM ���ϼ���ģʽ 
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);      			//�۳�ʼ�� TIMx 


	//��ʼ�� TIM3 CH2 PWM ģʽ
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2;          		//ѡ�� PWM ģʽ 2 
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; 	//�Ƚ����ʹ�� 
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;    	//������Ե� 

	TIM_OC2Init(TIM3,&TIM_OCInitStruct);

	//ʹ��Ԥװ��ֵ�Ĵ�����TM3
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);    			//ʹ��Ԥװ�ؼĴ��� 
	on_off=0;
	TIM_Cmd(TIM3, ENABLE);                         			//ʹ�� TIM3 
	
	fun_ONorOFF_flag=1;         //��־�ų�ʼ�������;
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
	printf("�¶ȣ�%d\n",Wd);
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


