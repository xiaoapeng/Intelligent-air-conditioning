#include "Gluttonous_Snake_key.h"
#include "string.h"
#include "drv_joypad.h"
#include "drv_basetim6.h"
#include "stdio.h"

#define Time_ms Time_ms


static u16 EFFECTIVE_MS;  //��Ч���������ʱ��
static u8 Key_queue[KEY_QUEUE_SIZE];//�洢����ʱ��Ķ��� 
static u8 Key_queue_head;  //��ͷ
static u8 Key_queue_end;   //��β
static st_KeyState st_Key; //�洢�����ĵ�ǰ״̬ 
static en_KEY KEY_BIAO[]={KEY_YOU,KEY_ZUO,KEY_XIA,KEY_SHANG,KEY_START,KEY_XUANZHE,KEY_B,KEY_A};


inline static u8 queue_null(void);
inline static u8 queue_full(void);
static void Queue_Init(void);
static void ru_Queue(u8 key);
static u8 chu_Queue(void);
//��ʼ�������¼�������
void Game_KEY_Init(u16 EFFECTIVE_MS_,u8 sx)
{
	JOYPAD_Init();     //��ʼ����������
	Queue_Init();      //��ʼ����Ϣ�ж�
	EFFECTIVE_MS=EFFECTIVE_MS_;
	st_Key.key_cdan=sx;
}
//��ն���
void clean_queue(void)
{
	Key_queue_head=Key_queue_end=0;
}
//�ж��¼������Ƿ�Ϊ��
inline static u8 queue_null(void)
{
	return Key_queue_head==Key_queue_end ? 1 :0;
}

//�ж��¼������Ƿ���
inline static u8 queue_full(void)
{
	return Key_queue_head==((Key_queue_end+1)%KEY_QUEUE_SIZE) ? 1 :0;
}
//��ʼ������
static void Queue_Init(void)
{
	u8 i;
	for(i=0;i<KEY_QUEUE_SIZE;i++)
		Key_queue[i]=0;
		Key_queue_head=Key_queue_end=0;

}
//���
static void ru_Queue(u8 key)
{
	if(queue_full()) return ; //������ 
	Key_queue[Key_queue_end]=key;
	Key_queue_end++;
	Key_queue_end=(Key_queue_end%KEY_QUEUE_SIZE);
}

//����
static u8 chu_Queue(void)
{
	u16 key;
	if(queue_null()) return 0xff;
	key=Key_queue[Key_queue_head];
	Key_queue_head++;
	Key_queue_head=(Key_queue_head%KEY_QUEUE_SIZE);
	return key;
}

//�������¼�
void scanfkey(void)
{
	u8 i;
	static u8 maiden=1;
	u8 key_cmp;
	if(maiden)//�������״ν��� Ҫ����һϵ�г�ʼ��
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
			if(key_cmp&(0X80>>i)) //����һ��״̬
			{
				if(st_Key.key&(0x80>>i))  //��һ����״̬
				{
					st_Key.time[i]+=Time_ms-st_Key.ms;
					if(st_Key.time[i]>=EFFECTIVE_MS&&((st_Key.key_cdan&(0x80>>i))==0))//�¼������Ѿ����
					{
						//��������д���
							ru_Queue(KEY_BIAO[i]);
						//Ȼ�����״̬
							st_Key.key&=(~(0x80>>i));
							st_Key.time[i]=0;
					}
				}
				else                      //��һ����״̬
				{
					st_Key.key=st_Key.key|(0x80>>i); //����Ӧλ��λ
					st_Key.time[i]=0;
				}
			}
			else												//״̬����
			{
				//��֮ǰ��״̬���г��� ��δ�γɴ���¼�������
				if(st_Key.key&(0x80>>i))
				{
					if(st_Key.time[i]<EFFECTIVE_MS)
					{ //����ʱ��̫�̣����������������Ƿ������� ����Ϊ����!!
						st_Key.key&=(~(0x80>>i));
						st_Key.time[i]=0;
					}
					else
					{
						//�����Ƕ���ʱ��ģ�����˵ֻ�з��������˵������ǻ��ǽ����ж�
						if(st_Key.key_cdan&(0x80>>i))
						{
							//�����ɹ�̧�� 
							//��������д���
							ru_Queue(KEY_BIAO[i]);
							//����״̬
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
//�����¼����� ���Ժ���
void chuli_key(void)
{
	u8 key;
	key=chu_Queue();
	while(key!=0xff)
	{
		switch(key)
		{
			case KEY_YOU:
				printf("key:�Ҽ�");
			break;
			case KEY_ZUO:
				printf("key:���");
			break;
			case KEY_SHANG:
				printf("key:�ϼ�");
			break;
			case KEY_XIA:
				printf("key:�¼�");
			break;
			case KEY_A:
				printf("key:A��");
			break;
			case KEY_B:
				printf("key:B��");
			break;
			case KEY_START:
				printf("key:��ʼ��");
			break;
			case KEY_XUANZHE:
				printf("key:ѡ���");
			break;
			default:
				
				break;
		}
		key=chu_Queue();
	}
}
//��ȡ�����¼� 
u8 get_key(void)
{
	return chu_Queue();
}


