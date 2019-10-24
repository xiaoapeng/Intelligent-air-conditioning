#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include "stm32f10x.h"

#define UART_IRQ_FUN USART3_IRQHandler



#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	
//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound);

int put_c(int ch);
void Usart1_Config_Init(void);
void Usart3_Config_Init(void);
void Usart2_Config_Init(void);
void send_usart3(char *a,u16 size);
extern int fputc(int ch, FILE *p);
extern u8 getData(void);



#endif







