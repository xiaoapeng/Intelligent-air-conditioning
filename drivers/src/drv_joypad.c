#include  "drv_joypad.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

//��ʼ���ֱ��ӿ�.	 
#define RCC_APBXPeriph_GPIOX RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOC  //Ҫʹ�ܵ�ʱ��

#define CAT_GPIOX GPIOC                                                 //����GPIO   com3_tx
#define CAT_PINX  GPIO_Pin_9																						//����pin����

#define CLK_GPIOX GPIOD																									//ʱ����
#define CLK_PINX  GPIO_Pin_3				

#define DAT_GPIOX	GPIOC	  																							//������     com3_rx
#define DAT_PINX  GPIO_Pin_8	



//��ʼ���ֱ��ӿ�.	 
void JOYPAD_Init(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APBXPeriph_GPIOX, ENABLE);	 //ʹ��PB,PD�˿�ʱ��
	
 	GPIO_InitStructure.GPIO_Pin = CAT_PINX;				 //����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(CAT_GPIOX, &GPIO_InitStructure); //��ʼ��GPIO
 	GPIO_SetBits(CAT_GPIOX,CAT_PINX);	//����

 	GPIO_InitStructure.GPIO_Pin = CLK_PINX;				 //ʱ���� 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(CLK_GPIOX, &GPIO_InitStructure); //��ʼ��GPIO
 	GPIO_SetBits(CLK_GPIOX,CLK_PINX);	//����
	
	
 	GPIO_InitStructure.GPIO_Pin  = DAT_PINX;      //������ 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
 	GPIO_Init(DAT_GPIOX, &GPIO_InitStructure); //��ʼ��GPIO
	GPIO_SetBits(DAT_GPIOX,DAT_PINX);	//����

}

//�ֱ��ӳٺ���
//t:Ҫ�ӳٵ�ʱ��
void JOYPAD_Delay(u16 t)
{
	while(t--);
}
//��ȡ�ֱ�����ֵ.
//FC�ֱ����������ʽ:
//ÿ��һ������,���һλ����,���˳��:
//A->B->SELECT->START->UP->DOWN->LEFT->RIGHT.
//�ܹ�8λ,������C��ť���ֱ�,����C��ʵ�͵���A+Bͬʱ����.
//������1,�ɿ���0.
//����ֵ:
//[7]:��
//[6]:��
//[5]:��
//[4]:��
//[3]:Start
//[2]:Select
//[1]:B
//[0]:A
u8 JOYPAD_Read(void)
{
	vu8 temp=0;
 	u8 t;
	GPIO_SetBits(CAT_GPIOX,CAT_PINX); //����
 	JOYPAD_Delay(80);
	GPIO_ResetBits(CAT_GPIOX,CAT_PINX);
	for(t=0;t<8;t++)
	{
		temp>>=1;	 
		if(GPIO_ReadInputDataBit(DAT_GPIOX,DAT_PINX)==0)temp|=0x80;//LOAD֮�󣬾͵õ���һ������
		
		GPIO_SetBits(CLK_GPIOX,CLK_PINX);			   	//ÿ��һ�����壬�յ�һ������
		JOYPAD_Delay(80);
		GPIO_ResetBits(CLK_GPIOX,CLK_PINX);
		JOYPAD_Delay(80); 
	}
	return temp;
}


