#ifndef __RED_H
#define __RED_H 
#include "stm32f10x.h"   
#define RDATA 	PBin(9)	 	//�������������
#define REMOTE_ID 0      		   
extern u8 RmtCnt;			//�������µĴ���
void Remote_Init(void);    	//���⴫��������ͷ���ų�ʼ��
u8 Remote_Scan(void);	    
#endif















