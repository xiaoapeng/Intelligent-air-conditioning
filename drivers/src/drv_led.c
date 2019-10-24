
#include "drv_led.h"



static void init_gpio_pin_ET(GPIO_TypeDef* GPIOX,uint16_t pinX,en_set set_or_reset)
{

	 GPIO_InitTypeDef  GPIO_InitStructure;
	 GPIO_InitStructure.GPIO_Pin = pinX;			
 	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
 	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	 GPIO_Init(GPIOX, &GPIO_InitStructure);	
	 if(set_or_reset)
	 	{
	 		 GPIO_SetBits(GPIOX,pinX);
	 	}
	 else
	 	{	
	 		GPIO_ResetBits(GPIOX,pinX);
	 	}
}


void LED_Init(void)
{
 
 RCC_APB2PeriphClockCmd(lED1_GPIO|LED2_GPIO, ENABLE);


 
 init_gpio_pin_ET(LED2_GPIO_TYPEDEF,LED2_PIN_X,LED_SET);	
 
}
 
void SET_LED1(void)
{
	GPIO_ResetBits(LED1_GPIO_TYPEDEF,LED1_PIN_X);	
}
void SET_LED2(void)
{
	GPIO_ResetBits(LED2_GPIO_TYPEDEF,LED2_PIN_X);	
}
	

	void RESET_LED1(void)
{
	GPIO_SetBits(LED1_GPIO_TYPEDEF,LED1_PIN_X);					
}
void RESET_LED2(void)
{
	GPIO_SetBits(LED2_GPIO_TYPEDEF,LED2_PIN_X); 					
}

