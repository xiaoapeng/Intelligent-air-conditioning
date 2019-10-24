#ifndef _GLUTTONOUS_SNAKE_KEY_H_
#define _GLUTTONOUS_SNAKE_KEY_H_
#include "stm32f10x.h"

//请为本程序提供一个时基


#define KEY_QUEUE_SIZE 20   //消息队列大小
#define KEY_len 8           //按键数量长度 

//按键长短按属性设置 
#define DEF_YOU   	0x80
#define DEF_ZUO   	0x40
#define DEF_XIA   	0x20
#define DEF_SHANG 	0x10
#define DEF_START 	0x08
#define DEF_XUANZHE 0x04
#define DEF_B       0x02
#define DEF_A       0x01


typedef enum type
{
	KEY_SHANG=1,
	KEY_XIA,
	KEY_ZUO,
	KEY_YOU,
	KEY_A,
	KEY_B,
	KEY_XUANZHE,
	KEY_START
}en_KEY;
typedef struct state
{
	u8 key;
	u8 key_cdan; //决定按键是连续按还是非连续按 
	u16 time[KEY_len];
	u32 ms;
}st_KeyState;
// EFFECTIVE_MS 表示按键触发时有效按时间是多少 
//sx 表示按键属性 
extern void Game_KEY_Init(u16 EFFECTIVE_MS,u8 sx); //进行按键操作的初始化 包括底层驱动程序和消息队列
extern void scanfkey(void);												 //读取按键状态 放在消息队列中 
extern u8 get_key(void); 													 //获取按键事件 返回0xff时表示按键表已经空了 
extern void chuli_key(void);
extern void clean_queue(void);                     //清空队列

//extern void chuli_key(void);






#endif
