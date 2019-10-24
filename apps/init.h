#ifndef _INIT_H_
#define _INIT_H_
#include "init.h"
#include "stm32f10x.h"
#include "drv_led.h"
#include "drv_sleep.h"
#include "drv_key.h"
#include "drv_usart.h"
#include "drv_cmd.h"
#include "string.h"
#include "drv_button.h"
#include "drv_basetim6.h"
#include "drv_time3.h"
#include "drv_dma.h"
#include "drv_adc.h"
#include "DataScope_DP.h"
#include "drv_iic.h"
#include "drv_at24cxx.h"
#include "gizwits_protocol.h"
#include "drv_lcd.h"
#include "fontupd.h"
#include "text.h"	
#define  UI_SIZE 25
typedef struct ui_
{
	
	u32  time;  		  //��ʾ��ʱ��
	u8   always_flag; 
	u16   x,y;        //��ʾ������
	char text[20];    //��ʾ���ַ�
	u8   size;        //��ʾ�Ĵ�С 
}st_ui;
#define PRESENCE    0x01 //����λ
#define ALWAYS      0x02 //������ʾλ
#define MODE_1      0x04 //����ģʽ



#define PRESENCE_(a) ((a)&(~PRESENCE))
#define ALWAYS_(a)   ((a)&(~ALWAYS))
#define MODE_1_(a)   ((a)&(~MODE_1))

typedef struct b_
{
	u16 magic;     //Э��ħ�� 0x0724
	u8 state;			 //�յ�״̬
	u8 temper;     //�յ��¶�
	u8 room_temper;//�����¶�
	u8 room_humi;  //����ʪ��
	u8 ill;				 //����ǿ��
	u8 mode;       //����ģʽ��־
	u8 efficacy;   //Ч��λ  //ǰ�����֮��
}agreement;
#define AGRR_SIZE  sizeof(agreement)  //Э���С

extern agreement pack; 
extern u8 temp,humi;
extern u8 LED1_flag;
extern u8 LED2_flag;
extern st_ui ui[UI_SIZE];
extern u8 zhineng_flag;   //����ģʽ��־λ  
extern u8 Illon_intensity; //����ֵ
extern u8 WIFI_intt_flag;  //WIFI ��ʼ����־λ 
extern u8 WIFI_on_off;     //WIFI �����ӱ�־λ
extern u8 CON_m2m;         //���������߱�־λ 
extern u8 WIFI_sici;       //wifi�źŵ�ǿ��   
extern u8 CON_app;         //���ӵ�APP��־  

extern u8 Game_Mode;       //��Ϸģʽ 


extern void Ctrl_Kt(void);
extern u8 add_ui(u32  time,u8  always_flag,u16 x,u16 y,const char* str,u8 size);
extern int wifi_flag;
extern void record(void);
extern void show(void);
extern void dispose_remote(void);
extern void init(void);
extern void  Square_whereabouts(u16 x1,u16 y1,u16 x2,u16 y2,u16 lim);


#endif
