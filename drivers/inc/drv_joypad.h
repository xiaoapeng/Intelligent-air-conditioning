#ifndef __JOYPAD_H
#define __JOYPAD_H	 
#include "stm32f10x.h"


void JOYPAD_Init(void);			//��ʼ��	
void JOYPAD_Delay(u16 t);		//��ʱ
u8 JOYPAD_Read(void);			//��ȡ��ֵ	 				    
#endif
