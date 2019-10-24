#include "Gluttonous_Snake_key.h"
#include "string.h"
#include "drv_joypad.h"
#include "drv_basetim6.h"
#include "stdio.h"

#define Time_ms Time_ms


static u16 EFFECTIVE_MS;  //有效按键的最短时间
static u8 Key_queue[KEY_QUEUE_SIZE];//存储按键时间的队列 
static u8 Key_queue_head;  //队头
static u8 Key_queue_end;   //队尾
static st_KeyState st_Key; //存储按键的当前状态 
static en_KEY KEY_BIAO[]={KEY_YOU,KEY_ZUO,KEY_XIA,KEY_SHANG,KEY_START,KEY_XUANZHE,KEY_B,KEY_A};


inline static u8 queue_null(void);
inline static u8 queue_full(void);
static void Queue_Init(void);
static void ru_Queue(u8 key);
static u8 chu_Queue(void);
//初始化按键事件接收器
void Game_KEY_Init(u16 EFFECTIVE_MS_,u8 sx)
{
	JOYPAD_Init();     //初始化驱动程序
	Queue_Init();      //初始化消息列队
	EFFECTIVE_MS=EFFECTIVE_MS_;
	st_Key.key_cdan=sx;
}
//清空队列
void clean_queue(void)
{
	Key_queue_head=Key_queue_end=0;
}
//判断事件队列是否为空
inline static u8 queue_null(void)
{
	return Key_queue_head==Key_queue_end ? 1 :0;
}

//判断事件队列是否满
inline static u8 queue_full(void)
{
	return Key_queue_head==((Key_queue_end+1)%KEY_QUEUE_SIZE) ? 1 :0;
}
//初始化队列
static void Queue_Init(void)
{
	u8 i;
	for(i=0;i<KEY_QUEUE_SIZE;i++)
		Key_queue[i]=0;
		Key_queue_head=Key_queue_end=0;

}
//入队
static void ru_Queue(u8 key)
{
	if(queue_full()) return ; //队列满 
	Key_queue[Key_queue_end]=key;
	Key_queue_end++;
	Key_queue_end=(Key_queue_end%KEY_QUEUE_SIZE);
}

//出队
static u8 chu_Queue(void)
{
	u16 key;
	if(queue_null()) return 0xff;
	key=Key_queue[Key_queue_head];
	Key_queue_head++;
	Key_queue_head=(Key_queue_head%KEY_QUEUE_SIZE);
	return key;
}

//读键盘事件
void scanfkey(void)
{
	u8 i;
	static u8 maiden=1;
	u8 key_cmp;
	if(maiden)//代表是首次进入 要进行一系列初始化
	{
		st_Key.key=JOYPAD_Read();
		for(i=0;i<KEY_len;i++)
		{
			st_Key.time[i]=0;
		}
		st_Key.ms=Time_ms;
		maiden=0;
		return ;
	}
	 key_cmp=JOYPAD_Read();
		for(i=0;i<KEY_len;i++)
		{
			if(key_cmp&(0X80>>i)) //出现一个状态
			{
				if(st_Key.key&(0x80>>i))  //是一个旧状态
				{
					st_Key.time[i]+=Time_ms-st_Key.ms;
					if(st_Key.time[i]>=EFFECTIVE_MS&&((st_Key.key_cdan&(0x80>>i))==0))//事件条件已经达成
					{
						//进行入队列处理
							ru_Queue(KEY_BIAO[i]);
						//然后清除状态
							st_Key.key&=(~(0x80>>i));
							st_Key.time[i]=0;
					}
				}
				else                      //是一个新状态
				{
					st_Key.key=st_Key.key|(0x80>>i); //将相应位置位
					st_Key.time[i]=0;
				}
			}
			else												//状态撤销
			{
				//对之前的状态进行撤销 并未形成达成事件的条件
				if(st_Key.key&(0x80>>i))
				{
					if(st_Key.time[i]<EFFECTIVE_MS)
					{ //按键时间太短，无论是连续按还是非连续按 都视为抖动!!
						st_Key.key&=(~(0x80>>i));
						st_Key.time[i]=0;
					}
					else
					{
						//超过非抖动时间的，按理说只有非连续按了但是我们还是进行判断
						if(st_Key.key_cdan&(0x80>>i))
						{
							//按键成功抬起 
							//进行入队列处理
							ru_Queue(KEY_BIAO[i]);
							//消除状态
							st_Key.key&=(~(0x80>>i));
							st_Key.time[i]=0;
						}
					}
				}
			}
		}
		if(st_Key.ms!=Time_ms)
		{
			st_Key.ms=Time_ms;
		}
}
//键盘事件处理 测试函数
void chuli_key(void)
{
	u8 key;
	key=chu_Queue();
	while(key!=0xff)
	{
		switch(key)
		{
			case KEY_YOU:
				printf("key:右键");
			break;
			case KEY_ZUO:
				printf("key:左键");
			break;
			case KEY_SHANG:
				printf("key:上键");
			break;
			case KEY_XIA:
				printf("key:下键");
			break;
			case KEY_A:
				printf("key:A键");
			break;
			case KEY_B:
				printf("key:B键");
			break;
			case KEY_START:
				printf("key:开始键");
			break;
			case KEY_XUANZHE:
				printf("key:选择键");
			break;
			default:
				
				break;
		}
		key=chu_Queue();
	}
}
//获取键盘事件 
u8 get_key(void)
{
	return chu_Queue();
}


