#include "Gluttonous_Snake_key.h"
#include "Gluttonous_Snake.h"
#include "drv_lcd.h"
#include "stdio.h"
#include "stdlib.h"
#include "drv_basetim6.h"
#include "init.h"
static void GameModeRun(void);

#define DRAW(x,y) LCD_LCD_DrawSxRectangle((x),(y),(x)+RECT_L,(y)+RECT_L) //通过物理坐标来绘制

#define DRAW_RECT_(x,y)  DRAW((x)+X1_RECT,(y)+Y1_REXT)                    //通过相对坐标来绘制
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
static u8 GAME_ZB;        //第一次进行初始化
static u8 Game_Mode;
static u8 quanhua_flag;   //当这个标志位置位时,整条蛇都要画  
static u8 SHI_HEAD_X;     //蛇头坐标
static u8 SHI_HEAD_Y;			//
static u8 SHI_END_X;     	//蛇尾坐标
static u8 SHI_END_Y;			//
static u8 SHI_SHIWU_X;    //食物X
static u8 SHI_SHIWU_Y;    //食物Y
static int FANGXIANG;     //头方向位 
static int END_FANGXIANG; //尾巴方向位 
static u8  JESUOJIANGLI;  //结束标志或者是奖励标志
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
	if(Queue_full()) return 0xff; //满
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




//切进游戏模式 
void Tab_RunGame(void)
{
	
	//画游戏模式下的矩形框 
	POINT_COLOR=BLACK;
	LCD_DrawRectangle(X1_RECT-1,Y1_REXT-1,X2_RECT+1,y2_RECT+1);
	switch(Game_Mode)
	{
		case GAME_MODE_NOSTART : //游戏未开始
			Game_Mode=GAME_MODE_START;
			GAME_ZB=1;
		break;
		case GAME_MODE_STOP: 		 //游戏停止态
			quanhua_flag=1;
			Game_Mode=GAME_MODE_RUN; //设置为游戏运行态 
		break;
		default :
			break;
	}
}




//切出游戏
extern void Tab_StopGmae(void)
{
		switch(Game_Mode)
	{
		case GAME_MODE_RUN:      //运行运行态
			Game_Mode=GAME_MODE_STOP;
		break;
		case GAME_MODE_over:     //游戏结束
			Game_Mode=GAME_MODE_NOSTART;
		break;
		case GAME_MODE_START:    //显示游戏开始界面
			Game_Mode=GAME_MODE_NOSTART;
		break;
		default :
			break;
	}
}	






//游戏初始化
void Game_Init(void)
{
	u8 x,y;
	//初始化按键事件接收器 有效按键最短按程为50ms 选择键和开始键设置为抬起有效 其他键可以连按
	Game_KEY_Init(5,DEF_XUANZHE|DEF_START);
	Game_Mode=GAME_MODE_NOSTART; //设置为游戏未开始状态 
	shiwu_flag=0;                //设置为没有食物
	srand(Time_ms);              //
	x=rand()%(MAP_X-8);
	y=rand()%(MAP_Y-8);
	x+=4;y+=4;
	SHI_END_Y=y;
	SHI_END_X=x+3;
	
	SHI_HEAD_Y=y;
	SHI_HEAD_X=x;
	
	map[y][x]=MAP_SE_HEAD;  //蛇头
	map[y][x+1]=MAP_SE_SHENG;
	map[y][x+2]=MAP_SE_SHENG;
	map[y][x+3]=MAP_SE_END;  //要去掉的部分
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
		case GAME_MODE_RUN:      //运行运行态
		//写游戏运行的代码
		scanfkey(); //读按键事件 
		if(Time_ms%50==0)
		{
			GameModeRun();
		}
		break;
		case GAME_MODE_over:     //游戏结束（重来一把）
			printf("游戏结束\n");
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
		//*******************************************开始界面************************************************
		case GAME_MODE_START:    //显示游戏开始界面
		//读按键事件
		printf("游戏开始界面\n");
		for(x=0;x<MAP_X;x++)
			for(y=0;y<MAP_Y;y++)
			{
				map[y][x]=0;
			}
		if(GAME_ZB)
		{
			GAME_ZB=0;
			//只有第一次进来才进行初始化 
			shiwu_flag=0;                //设置为没有食物
			srand(Time_ms);              //
			x=rand()%(MAP_X-8);
			y=rand()%(MAP_Y-8);
			x+=4;y+=4;
			SHI_END_Y=y;
			SHI_END_X=x+3;

			SHI_HEAD_Y=y;
			SHI_HEAD_X=x;

			map[y][x]=MAP_SE_HEAD;  //蛇头
			map[y][x+1]=MAP_SE_SHENG;
			map[y][x+2]=MAP_SE_SHENG;
			map[y][x+3]=MAP_SE_END;  //要去掉的部分
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
				//应该清屏
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
		printf("游戏正在运行\n");
		
		
	while(shiwu_flag==0)
		{//产生食物
				SHI_SHIWU_X=x=rand()%MAP_X;
				SHI_SHIWU_Y=y=rand()%MAP_Y;
				if(map[y][x]==MAP_DIMIAN)
				{
					shiwu_flag=1;
					map[y][x]=MAP_SHIWU;
				}
		}
	//进行整个地图的绘制
	for(x=0;x<MAP_X;x++)
		for(y=0;y<MAP_Y;y++)
		{
			switch(map[y][x])
			{
				case MAP_DIMIAN: //地面不用绘制 
					break;
				case MAP_SE_HEAD://蛇头
					POINT_COLOR=HEAD_CLOCK;
					DRAW_RECT(x,y);
					break;
				case MAP_SE_END: //上一次蛇尾
					if(JESUOJIANGLI==JIANGLI) 
					{
						//吃到食物的奖励 
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
				case MAP_SE_SHENG://蛇身
					if(!quanhua_flag) break;
					POINT_COLOR=BODY_CLOCK;
					DRAW_RECT(x,y);
					break;
				case MAP_SHIWU:    //食物
					POINT_COLOR=SHIWU_CLOCK;
					DRAW_RECT(x,y);
					break;
			}
		}
		//在这里产生新的蛇尾
		if(JESUOJIANGLI!=JIANGLI)
			new_she_end(SHI_END_X,SHI_END_Y);
		//对蛇的方向进行操控
		//先保留之前的方向 将它相反的方向视为不正确
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
				key=get_key();
		}
		//对整个蛇进行移动
		//先将旧蛇头置为蛇身
		map[SHI_HEAD_Y][SHI_HEAD_X]=MAP_SE_SHENG;
		//在转弯的情况下将转折点入队列
				if(fx!=FANGXIANG)
		{
			//说明蛇已经换了方向 要将转折点入队
			zhuanzd.fx=FANGXIANG;
			zhuanzd.x=SHI_HEAD_X;
			zhuanzd.y=SHI_HEAD_Y;
			if(Queue_ru(zhuanzd))
			{
				printf("队列满\n");
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
		//判断蛇此时的状态 
		//首先判断蛇的
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
			//将会在绘制中处理 （不清除蛇尾）
			shiwu_flag=0;
			map[SHI_SHIWU_Y][SHI_SHIWU_X]=MAP_SE_HEAD;
			GAME_fs++;
		}
}

//碰撞处理
void pzcl(u8 x,u8 y)
{
	if(x>=MAP_X||y>=MAP_Y)
	{
		JESUOJIANGLI=YUEJIE;
		return ;
	}
	switch(map[y][x])
	{
		case MAP_DIMIAN: //地面
			JESUOJIANGLI=0;
			break;
		case MAP_SE_HEAD://蛇头
			break;
		case MAP_SE_END: //上一次蛇尾
			JESUOJIANGLI=YAOZJ;
			break;
		case MAP_SHIWU:    //食物
			JESUOJIANGLI=JIANGLI;
			break;
		case MAP_SE_SHENG://蛇身
			JESUOJIANGLI=YAOZJ;
				break;
	}
}

//产生新的蛇尾
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




