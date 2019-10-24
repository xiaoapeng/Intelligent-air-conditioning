#include  "drv_joypad.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

//初始化手柄接口.	 
#define RCC_APBXPeriph_GPIOX RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOC  //要使能的时钟

#define CAT_GPIOX GPIOC                                                 //锁存GPIO   com3_tx
#define CAT_PINX  GPIO_Pin_9																						//锁存pin引脚

#define CLK_GPIOX GPIOD																									//时钟线
#define CLK_PINX  GPIO_Pin_3				

#define DAT_GPIOX	GPIOC	  																							//数据线     com3_rx
#define DAT_PINX  GPIO_Pin_8	



//初始化手柄接口.	 
void JOYPAD_Init(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APBXPeriph_GPIOX, ENABLE);	 //使能PB,PD端口时钟
	
 	GPIO_InitStructure.GPIO_Pin = CAT_PINX;				 //锁存
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(CAT_GPIOX, &GPIO_InitStructure); //初始化GPIO
 	GPIO_SetBits(CAT_GPIOX,CAT_PINX);	//上拉

 	GPIO_InitStructure.GPIO_Pin = CLK_PINX;				 //时钟线 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(CLK_GPIOX, &GPIO_InitStructure); //初始化GPIO
 	GPIO_SetBits(CLK_GPIOX,CLK_PINX);	//上拉
	
	
 	GPIO_InitStructure.GPIO_Pin  = DAT_PINX;      //数据线 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
 	GPIO_Init(DAT_GPIOX, &GPIO_InitStructure); //初始化GPIO
	GPIO_SetBits(DAT_GPIOX,DAT_PINX);	//上拉

}

//手柄延迟函数
//t:要延迟的时间
void JOYPAD_Delay(u16 t)
{
	while(t--);
}
//读取手柄按键值.
//FC手柄数据输出格式:
//每给一个脉冲,输出一位数据,输出顺序:
//A->B->SELECT->START->UP->DOWN->LEFT->RIGHT.
//总共8位,对于有C按钮的手柄,按下C其实就等于A+B同时按下.
//按下是1,松开是0.
//返回值:
//[7]:右
//[6]:左
//[5]:下
//[4]:上
//[3]:Start
//[2]:Select
//[1]:B
//[0]:A
u8 JOYPAD_Read(void)
{
	vu8 temp=0;
 	u8 t;
	GPIO_SetBits(CAT_GPIOX,CAT_PINX); //锁存
 	JOYPAD_Delay(80);
	GPIO_ResetBits(CAT_GPIOX,CAT_PINX);
	for(t=0;t<8;t++)
	{
		temp>>=1;	 
		if(GPIO_ReadInputDataBit(DAT_GPIOX,DAT_PINX)==0)temp|=0x80;//LOAD之后，就得到第一个数据
		
		GPIO_SetBits(CLK_GPIOX,CLK_PINX);			   	//每给一次脉冲，收到一个数据
		JOYPAD_Delay(80);
		GPIO_ResetBits(CLK_GPIOX,CLK_PINX);
		JOYPAD_Delay(80); 
	}
	return temp;
}


