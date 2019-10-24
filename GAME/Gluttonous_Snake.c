#include "Gluttonous_Snake_key.h"
#include "Gluttonous_Snake.h"
#include "drv_lcd.h"
#include "stdio.h"
#include "stdlib.h"
#include "drv_basetim6.h"
#include "init.h"
static void GameModeRun(void);

#define DRAW(x,y) LCD_LCD_DrawSxRectangle((x),(y),(x)+RECT_L,(y)+RECT_L) //ͨ����������������

#define DRAW_RECT_(x,y)  DRAW((x)+X1_RECT,(y)+Y1_REXT)                    //ͨ���������������
#define ZZD_SIZE 64
#define DRAW_RECT(x,y) \
if(((x)<MAP_X)&&((y)<MAP_Y))\
{\
	DRAW_RECT_(5*(x),5*(y));\
}
ZZD Zzd_Queue[ZZD_SIZE];
u8 HEAD_queue,END_queue;

u8 shiwu_flag;
u8 map[MAP_Y][MAP_X]={0};
static u32 GAME_fs=0;
static u8 GAME_ZB;        //��һ�ν��г�ʼ��
static u8 Game_Mode;
static u8 quanhua_flag;   //�������־λ��λʱ,�����߶�Ҫ��  
static u8 SHI_HEAD_X;     //��ͷ����
static u8 SHI_HEAD_Y;			//
static u8 SHI_END_X;     	//��β����
static u8 SHI_END_Y;			//
static u8 SHI_SHIWU_X;    //ʳ��X
static u8 SHI_SHIWU_Y;    //ʳ��Y
static int FANGXIANG;     //ͷ����λ 
static int END_FANGXIANG; //β�ͷ���λ 
static u8  JESUOJIANGLI;  //������־�����ǽ�����־
void pzcl(u8 x,u8 y);
void new_she_end(u8 x,u8 y);
static u8 Queue_null(void)
{
	return HEAD_queue==END_queue?1:0;
}
static u8 Queue_full(void)
{
	return HEAD_queue==((END_queue+1)%ZZD_SIZE)?1:0;
}
static ZZD Queue_chu(void)
{
	ZZD a;
	a.fx=0;
	if(Queue_null()) return a;
	a=Zzd_Queue[HEAD_queue];
	HEAD_queue=(HEAD_queue+1)%ZZD_SIZE;
	return a;
}
static u8 Queue_ru(ZZD a)
{
	if(Queue_full()) return 0xff; //��
	Zzd_Queue[END_queue]=a;
	END_queue=(END_queue+1)%ZZD_SIZE;
	return 0;
}

static ZZD Queue_chakan(void)
{
	ZZD a;
	a.fx=0;
	if(Queue_null()) return a;
	a=Zzd_Queue[HEAD_queue];
	return a;
}




//�н���Ϸģʽ 
void Tab_RunGame(void)
{
	
	//����Ϸģʽ�µľ��ο� 
	POINT_COLOR=BLACK;
	LCD_DrawRectangle(X1_RECT-1,Y1_REXT-1,X2_RECT+1,y2_RECT+1);
	switch(Game_Mode)
	{
		case GAME_MODE_NOSTART : //��Ϸδ��ʼ
			Game_Mode=GAME_MODE_START;
			GAME_ZB=1;
		break;
		case GAME_MODE_STOP: 		 //��Ϸֹ̬ͣ
			quanhua_flag=1;
			Game_Mode=GAME_MODE_RUN; //����Ϊ��Ϸ����̬ 
		break;
		default :
			break;
	}
}




//�г���Ϸ
extern void Tab_StopGmae(void)
{
		switch(Game_Mode)
	{
		case GAME_MODE_RUN:      //��������̬
			Game_Mode=GAME_MODE_STOP;
		break;
		case GAME_MODE_over:     //��Ϸ����
			Game_Mode=GAME_MODE_NOSTART;
		break;
		case GAME_MODE_START:    //��ʾ��Ϸ��ʼ����
			Game_Mode=GAME_MODE_NOSTART;
		break;
		default :
			break;
	}
}	






//��Ϸ��ʼ��
void Game_Init(void)
{
	u8 x,y;
	//��ʼ�������¼������� ��Ч������̰���Ϊ50ms ѡ����Ϳ�ʼ������Ϊ̧����Ч ��������������
	Game_KEY_Init(5,DEF_XUANZHE|DEF_START);
	Game_Mode=GAME_MODE_NOSTART; //����Ϊ��Ϸδ��ʼ״̬ 
	shiwu_flag=0;                //����Ϊû��ʳ��
	srand(Time_ms);              //
	x=rand()%(MAP_X-8);
	y=rand()%(MAP_Y-8);
	x+=4;y+=4;
	SHI_END_Y=y;
	SHI_END_X=x+3;
	
	SHI_HEAD_Y=y;
	SHI_HEAD_X=x;
	
	map[y][x]=MAP_SE_HEAD;  //��ͷ
	map[y][x+1]=MAP_SE_SHENG;
	map[y][x+2]=MAP_SE_SHENG;
	map[y][x+3]=MAP_SE_END;  //Ҫȥ���Ĳ���
	FANGXIANG=ZUO;
	END_FANGXIANG=ZUO;
	HEAD_queue=0;
	END_queue=0;
}


//***********
void Run_Game(void)
{
	u8 key;
	u8 x,y;
	switch(Game_Mode)
	{
		case GAME_MODE_RUN:      //��������̬
		//д��Ϸ���еĴ���
		scanfkey(); //�������¼� 
		if(Time_ms%50==0)
		{
			GameModeRun();
		}
		break;
		case GAME_MODE_over:     //��Ϸ����������һ�ѣ�
			printf("��Ϸ����\n");
			key=get_key();
		while(key!=0xff)
		{
			if(key==KEY_START)
			{
				Game_Mode=GAME_MODE_START;
				quanhua_flag=1;
			}
			key=get_key();
		}
		break;
		//*******************************************��ʼ����************************************************
		case GAME_MODE_START:    //��ʾ��Ϸ��ʼ����
		//�������¼�
		printf("��Ϸ��ʼ����\n");
		for(x=0;x<MAP_X;x++)
			for(y=0;y<MAP_Y;y++)
			{
				map[y][x]=0;
			}
		if(GAME_ZB)
		{
			GAME_ZB=0;
			//ֻ�е�һ�ν����Ž��г�ʼ�� 
			shiwu_flag=0;                //����Ϊû��ʳ��
			srand(Time_ms);              //
			x=rand()%(MAP_X-8);
			y=rand()%(MAP_Y-8);
			x+=4;y+=4;
			SHI_END_Y=y;
			SHI_END_X=x+3;

			SHI_HEAD_Y=y;
			SHI_HEAD_X=x;

			map[y][x]=MAP_SE_HEAD;  //��ͷ
			map[y][x+1]=MAP_SE_SHENG;
			map[y][x+2]=MAP_SE_SHENG;
			map[y][x+3]=MAP_SE_END;  //Ҫȥ���Ĳ���
			FANGXIANG=ZUO;
			END_FANGXIANG=ZUO;
			HEAD_queue=0;
			END_queue=0;
			Square_whereabouts(10+10,10,50+10,50,320);
			Square_whereabouts(50+10,10,90+10,50,280);
			Square_whereabouts(90+10,10,130+10,50,320);
			Square_whereabouts(90+40+10,10,130+40+10,50,280);
			Square_whereabouts(90+80+10,10,130+80+10,50,320);
			Square_whereabouts(90+10,10,130+10,50,280-40);
			Show_Str(25,50,200,24,"Gluttonous Snake",24,0);
		}
		
		scanfkey();
		key=get_key();
		while(key!=0xff)
		{
			if(key==KEY_START)
			{
				
				Game_Mode=GAME_MODE_RUN;
				quanhua_flag=1;
				LCD_Clear(WHITE);
				//Ӧ������
				POINT_COLOR=BLACK;
				LCD_DrawRectangle(X1_RECT-1,Y1_REXT-1,X2_RECT+1,y2_RECT+1);
			}
			key=get_key();
		}
		break;
		default :
			
			break;
	}
}
void GameModeRun(void)
{
	ZZD zzd;
	u8 x,y;
	int fx;
	u8 key;
	ZZD zhuanzd;
		printf("��Ϸ��������\n");
		
		
	while(shiwu_flag==0)
		{//����ʳ��
				SHI_SHIWU_X=x=rand()%MAP_X;
				SHI_SHIWU_Y=y=rand()%MAP_Y;
				if(map[y][x]==MAP_DIMIAN)
				{
					shiwu_flag=1;
					map[y][x]=MAP_SHIWU;
				}
		}
	//����������ͼ�Ļ���
	for(x=0;x<MAP_X;x++)
		for(y=0;y<MAP_Y;y++)
		{
			switch(map[y][x])
			{
				case MAP_DIMIAN: //���治�û��� 
					break;
				case MAP_SE_HEAD://��ͷ
					POINT_COLOR=HEAD_CLOCK;
					DRAW_RECT(x,y);
					break;
				case MAP_SE_END: //��һ����β
					if(JESUOJIANGLI==JIANGLI) 
					{
						//�Ե�ʳ��Ľ��� 
						break;
					}
					zzd=Queue_chakan();
					if(zzd.x==x&&zzd.y==y&&(zzd.fx!=0))
					{
						END_FANGXIANG=zzd.fx;
						Queue_chu();
					}
					POINT_COLOR=BG_CLOCK;
					DRAW_RECT(x,y);
				  map[y][x]=MAP_DIMIAN;
					
					break;
				case MAP_SE_SHENG://����
					if(!quanhua_flag) break;
					POINT_COLOR=BODY_CLOCK;
					DRAW_RECT(x,y);
					break;
				case MAP_SHIWU:    //ʳ��
					POINT_COLOR=SHIWU_CLOCK;
					DRAW_RECT(x,y);
					break;
			}
		}
		//����������µ���β
		if(JESUOJIANGLI!=JIANGLI)
			new_she_end(SHI_END_X,SHI_END_Y);
		//���ߵķ�����вٿ�
		//�ȱ���֮ǰ�ķ��� �����෴�ķ�����Ϊ����ȷ
		fx=FANGXIANG;
		key=get_key();
		while(key!=0xff)
		{
				switch(key)
				{
					case KEY_YOU:
						if((-YOU)==fx) break;
						FANGXIANG=YOU;
					break;
					case KEY_ZUO:
						if((-ZUO)==fx) break;
						FANGXIANG=ZUO;
					break;
					case KEY_SHANG:
						if((-SHANG)==fx) break;
						FANGXIANG=SHANG;
					break;
					case KEY_XIA:
						if((-XIA)==fx) break;
						FANGXIANG=XIA;
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
				key=get_key();
		}
		//�������߽����ƶ�
		//�Ƚ�����ͷ��Ϊ����
		map[SHI_HEAD_Y][SHI_HEAD_X]=MAP_SE_SHENG;
		//��ת�������½�ת�۵������
				if(fx!=FANGXIANG)
		{
			//˵�����Ѿ����˷��� Ҫ��ת�۵����
			zhuanzd.fx=FANGXIANG;
			zhuanzd.x=SHI_HEAD_X;
			zhuanzd.y=SHI_HEAD_Y;
			if(Queue_ru(zhuanzd))
			{
				printf("������\n");
				Game_Mode=GAME_MODE_over;
				return ;
			}
		}
		switch(FANGXIANG)
		{
			case SHANG:
				pzcl(SHI_HEAD_X,--SHI_HEAD_Y);
				break;
			case XIA:
				pzcl(SHI_HEAD_X,++SHI_HEAD_Y);
				break;
			case ZUO:
				pzcl(--SHI_HEAD_X,SHI_HEAD_Y);
				break;
			case YOU:
				pzcl(++SHI_HEAD_X,SHI_HEAD_Y);
				break;
		}
		map[SHI_HEAD_Y][SHI_HEAD_X]=MAP_SE_HEAD;
		//�ж��ߴ�ʱ��״̬ 
		//�����ж��ߵ�
		if(JESUOJIANGLI==YAOZJ||JESUOJIANGLI==YUEJIE)
		{
			Game_Mode=GAME_MODE_over;
			LCD_Clear(WHITE);
			POINT_COLOR=BLACK;
			Show_Str(25,50,200,24,"score:",24,0);
			LCD_ShowNum(90,50,GAME_fs,3,24);
			
			return ;
		}
		if(JESUOJIANGLI==JIANGLI)
		{
			//�����ڻ����д��� ���������β��
			shiwu_flag=0;
			map[SHI_SHIWU_Y][SHI_SHIWU_X]=MAP_SE_HEAD;
			GAME_fs++;
		}
}

//��ײ����
void pzcl(u8 x,u8 y)
{
	if(x>=MAP_X||y>=MAP_Y)
	{
		JESUOJIANGLI=YUEJIE;
		return ;
	}
	switch(map[y][x])
	{
		case MAP_DIMIAN: //����
			JESUOJIANGLI=0;
			break;
		case MAP_SE_HEAD://��ͷ
			break;
		case MAP_SE_END: //��һ����β
			JESUOJIANGLI=YAOZJ;
			break;
		case MAP_SHIWU:    //ʳ��
			JESUOJIANGLI=JIANGLI;
			break;
		case MAP_SE_SHENG://����
			JESUOJIANGLI=YAOZJ;
				break;
	}
}

//�����µ���β
void new_she_end(u8 x,u8 y)
{
		switch(END_FANGXIANG)
		{
			case SHANG:
				map[--y][x]=MAP_SE_END;
				break;
			case XIA:
				map[++y][x]=MAP_SE_END;
				break;
			case ZUO:
				map[y][--x]=MAP_SE_END;
				break;
			case YOU:
				map[y][++x]=MAP_SE_END;
				break;
			default:
				break;
		}
		SHI_END_X=x;
		SHI_END_Y=y;
}




