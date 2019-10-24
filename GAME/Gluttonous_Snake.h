#ifndef _GLUTTONOUS_SNAKE_H_
#define _GLUTTONOUS_SNAKE_H_
#include "drv_lcd.h"

#define MAP_DIMIAN     0 //地面
#define MAP_SE_HEAD    1 //要画的部分     头
#define MAP_SE_END     2 //要去掉的部分   尾巴
#define MAP_SE_SHENG   3 //不要改变的部分 身体
#define MAP_SHIWU      4 //食物

#define X1_RECT  5    //矩形边界左上角x
#define Y1_REXT  5		//矩形边界左上角y
#define X2_RECT  235	//矩形边界右下角x
#define y2_RECT  315  //矩形边界右下角y

#define RECT_L   5             //正方形小格子大小 
#define HEAD_CLOCK  RED        //蛇头颜色
#define BODY_CLOCK  BRRED      //蛇身体颜色
#define BG_CLOCK    WHITE      //背景
#define SHIWU_CLOCK LIGHTGREEN //食物的颜色


#define GAME_MODE_NOSTART  0 //游戏未开始状态
#define GAME_MODE_RUN   	 1 //正在游戏的状态
#define GAME_MODE_STOP  	 2 //游戏暂停状态
#define GAME_MODE_over  	 3 //游戏结束正常退出
#define GAME_MODE_START 	 4 //游戏开始界面 


#define MAP_X   ((X2_RECT-X1_RECT)/RECT_L)
#define MAP_Y   ((y2_RECT-Y1_REXT)/RECT_L)

#define SHANG (1)     //上
#define XIA   (-1)			//下
#define ZUO   (2)		//左
#define YOU   (-2)			//右

#define YUEJIE  1    //越界
#define YAOZJ   2		 //咬到自己
#define JIANGLI 3		 //吃到奖励




typedef struct zhuanzhedian
{
	u8 x,y;
	int fx;
}ZZD;




extern void Tab_RunGame(void);    //切进游戏
extern void Tab_StopGmae(void);		//切出游戏 

extern u8 map[MAP_Y][MAP_X];
extern void Game_Init(void);
extern  void Run_Game(void);



#endif
