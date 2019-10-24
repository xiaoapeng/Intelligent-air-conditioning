#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include "stm32f10x.h"

#define UART_IRQ_FUN USART3_IRQHandler



#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);

int put_c(int ch);
void Usart1_Config_Init(void);
void Usart3_Config_Init(void);
void Usart2_Config_Init(void);
void send_usart3(char *a,u16 size);
extern int fputc(int ch, FILE *p);
extern u8 getData(void);



#endif







