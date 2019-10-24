#ifndef __RED_H
#define __RED_H 
#include "stm32f10x.h"   
#define RDATA 	PBin(9)	 	//红外数据输入脚
#define REMOTE_ID 0      		   
extern u8 RmtCnt;			//按键按下的次数
void Remote_Init(void);    	//红外传感器接收头引脚初始化
u8 Remote_Scan(void);	    
#endif















