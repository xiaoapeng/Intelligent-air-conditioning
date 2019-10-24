#ifndef _GLUTTONOUS_SNAKE_KEY_H_
#define _GLUTTONOUS_SNAKE_KEY_H_
#include "stm32f10x.h"

//��Ϊ�������ṩһ��ʱ��


#define KEY_QUEUE_SIZE 20   //��Ϣ���д�С
#define KEY_len 8           //������������ 

//�������̰��������� 
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
	u8 key_cdan; //�������������������Ƿ������� 
	u16 time[KEY_len];
	u32 ms;
}st_KeyState;
// EFFECTIVE_MS ��ʾ��������ʱ��Ч��ʱ���Ƕ��� 
//sx ��ʾ�������� 
extern void Game_KEY_Init(u16 EFFECTIVE_MS,u8 sx); //���а��������ĳ�ʼ�� �����ײ������������Ϣ����
extern void scanfkey(void);												 //��ȡ����״̬ ������Ϣ������ 
extern u8 get_key(void); 													 //��ȡ�����¼� ����0xffʱ��ʾ�������Ѿ����� 
extern void chuli_key(void);
extern void clean_queue(void);                     //��ն���

//extern void chuli_key(void);






#endif
