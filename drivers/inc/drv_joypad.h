#ifndef __JOYPAD_H
#define __JOYPAD_H	 
#include "stm32f10x.h"


void JOYPAD_Init(void);			//初始化	
void JOYPAD_Delay(u16 t);		//延时
u8 JOYPAD_Read(void);			//读取键值	 				    
#endif
