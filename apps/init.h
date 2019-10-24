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
	
	u32  time;  		  //显示的时间
	u8   always_flag; 
	u16   x,y;        //显示的坐标
	char text[20];    //显示的字符
	u8   size;        //显示的大小 
}st_ui;
#define PRESENCE    0x01 //存在位
#define ALWAYS      0x02 //永久显示位
#define MODE_1      0x04 //叠加模式



#define PRESENCE_(a) ((a)&(~PRESENCE))
#define ALWAYS_(a)   ((a)&(~ALWAYS))
#define MODE_1_(a)   ((a)&(~MODE_1))

typedef struct b_
{
	u16 magic;     //协议魔数 0x0724
	u8 state;			 //空调状态
	u8 temper;     //空调温度
	u8 room_temper;//房间温度
	u8 room_humi;  //房间湿度
	u8 ill;				 //光照强度
	u8 mode;       //智能模式标志
	u8 efficacy;   //效验位  //前面相加之和
}agreement;
#define AGRR_SIZE  sizeof(agreement)  //协议大小

extern agreement pack; 
extern u8 temp,humi;
extern u8 LED1_flag;
extern u8 LED2_flag;
extern st_ui ui[UI_SIZE];
extern u8 zhineng_flag;   //智能模式标志位  
extern u8 Illon_intensity; //光敏值
extern u8 WIFI_intt_flag;  //WIFI 初始化标志位 
extern u8 WIFI_on_off;     //WIFI 已连接标志位
extern u8 CON_m2m;         //服务器在线标志位 
extern u8 WIFI_sici;       //wifi信号的强度   
extern u8 CON_app;         //连接到APP标志  

extern u8 Game_Mode;       //游戏模式 


extern void Ctrl_Kt(void);
extern u8 add_ui(u32  time,u8  always_flag,u16 x,u16 y,const char* str,u8 size);
extern int wifi_flag;
extern void record(void);
extern void show(void);
extern void dispose_remote(void);
extern void init(void);
extern void  Square_whereabouts(u16 x1,u16 y1,u16 x2,u16 y2,u16 lim);


#endif
