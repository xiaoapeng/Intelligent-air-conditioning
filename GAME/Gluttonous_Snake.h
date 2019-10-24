#ifndef _GLUTTONOUS_SNAKE_H_
#define _GLUTTONOUS_SNAKE_H_
#include "drv_lcd.h"

#define MAP_DIMIAN     0 //����
#define MAP_SE_HEAD    1 //Ҫ���Ĳ���     ͷ
#define MAP_SE_END     2 //Ҫȥ���Ĳ���   β��
#define MAP_SE_SHENG   3 //��Ҫ�ı�Ĳ��� ����
#define MAP_SHIWU      4 //ʳ��

#define X1_RECT  5    //���α߽����Ͻ�x
#define Y1_REXT  5		//���α߽����Ͻ�y
#define X2_RECT  235	//���α߽����½�x
#define y2_RECT  315  //���α߽����½�y

#define RECT_L   5             //������С���Ӵ�С 
#define HEAD_CLOCK  RED        //��ͷ��ɫ
#define BODY_CLOCK  BRRED      //��������ɫ
#define BG_CLOCK    WHITE      //����
#define SHIWU_CLOCK LIGHTGREEN //ʳ�����ɫ


#define GAME_MODE_NOSTART  0 //��Ϸδ��ʼ״̬
#define GAME_MODE_RUN   	 1 //������Ϸ��״̬
#define GAME_MODE_STOP  	 2 //��Ϸ��ͣ״̬
#define GAME_MODE_over  	 3 //��Ϸ���������˳�
#define GAME_MODE_START 	 4 //��Ϸ��ʼ���� 


#define MAP_X   ((X2_RECT-X1_RECT)/RECT_L)
#define MAP_Y   ((y2_RECT-Y1_REXT)/RECT_L)

#define SHANG (1)     //��
#define XIA   (-1)			//��
#define ZUO   (2)		//��
#define YOU   (-2)			//��

#define YUEJIE  1    //Խ��
#define YAOZJ   2		 //ҧ���Լ�
#define JIANGLI 3		 //�Ե�����




typedef struct zhuanzhedian
{
	u8 x,y;
	int fx;
}ZZD;




extern void Tab_RunGame(void);    //�н���Ϸ
extern void Tab_StopGmae(void);		//�г���Ϸ 

extern u8 map[MAP_Y][MAP_X];
extern void Game_Init(void);
extern  void Run_Game(void);



#endif
