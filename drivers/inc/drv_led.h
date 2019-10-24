#ifndef __LED_H
#define __LED_H	 
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"


#define lED1_GPIO RCC_APB2Periph_GPIOB
#define LED2_GPIO RCC_APB2Periph_GPIOE

#define LED1_GPIO_TYPEDEF  GPIOB
#define LED2_GPIO_TYPEDEF  GPIOE

#define LED1_PIN_X  GPIO_Pin_5
#define LED2_PIN_X  GPIO_Pin_5

typedef enum en_set_
{

	LED_SET=1,LED_RESET

}en_set;

#define ZT(LED,BOOL) BOOL##_##LED()

//***************************


extern void LED_Init(void);//≥ı ºªØ
extern void SET_LED1(void);
extern void SET_LED2(void);
extern void RESET_LED1(void);
extern void RESET_LED2(void);
		 				    
#endif
