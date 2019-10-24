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
#include "string.h"
#include "drv_remote.h"
#include "drv_time3.h"
#include "drv_ht11.h"
#include "drv_lsens.h"
#include  "drv_joypad.h"
#include "Gluttonous_Snake_key.h"
#include "Gluttonous_Snake.h"

st_ui ui[25]={0};
int wifi_flag=0;
static u8 chu_queue(void);
u8 temp,humi;			    //温度湿度
u8 zhineng_flag;      //智能模式标志位
u8 LED1_flag;      
u8 LED2_flag;			    //LED2灯亮标志 
u8 Illon_intensity;   //光敏值
u8 WIFI_intt_flag;    //wifi初始化成功标志（这玩意怎么说）
u8 WIFI_on_off;       //wifi连接标志位
u8 CON_m2m;						//手机连接标志位
u8 WIFI_sici;            //wifi信号的强度 
u8 CON_app;         //连接到APP标志    
u8 Game_Mode;        //游戏模式标志 
agreement pack;     //要发送的包 
void Ctrl_Kt(void)
{
	static u8 flag=0;
	agreement pack_;
	u32 i=0;
	static u16 z=0;
	static u8 wd=0,wd_flag;
	static u32 ms=0,ms2=0,ms3=0,ms4=0;
	
	
	//对遥控事件进行处理 
	dispose_remote();
	
	//游戏模式
	if(Game_Mode)
	{
		Run_Game();
	}
	
	
	//读取各个传感器的内容
	if((Time_ms-ms)>3000)
	{
			if(DHT11_Read_Data(&temp,&humi)==1) printf("温度传感器出现问题");
		  temp-=15;
		  Illon_intensity=Lsens_Get_Val();
		  printf("temp=%d",temp);
		  ms=Time_ms;
	}
	//风页带不动解决方案
	{
		if(on_off&&(wd!=Wd))
		{
			wd_flag=1;
			ms3=Time_ms;
			set_wd(16);//强制设置为16度;
			wd=Wd;
		}
		if(wd_flag==1&&(Time_ms-ms3>500))
		{
			wd_flag=0;
			set_wd(Wd);
		}
	}
	
	
	
	//调用智能模式管理函数 进行屏幕显示 对从机发包 都是在这里处理的 
	if((Time_ms-ms2)>500)
	{
		  Illon_intensity=Lsens_Get_Val();
		  ms2=Time_ms;
		//调用智能模式管理函数 
			if(zhineng_flag&&on_off)
		{
			
			if(temp<30)
			{
				Wd=26;
				set_wd(Wd);
			}
			else if(temp>=30&&temp<35)
			{
				Wd=24;
				set_wd(Wd);
			}
			else if(temp>=35&&temp<38)
			{
				Wd=20;
				set_wd(Wd);
			}
			else if(temp>=38)
			{
				Wd=16;
				set_wd(Wd);
			}
			
			if(Illon_intensity<4)
			{
				add_Wd();
				add_Wd();
			}
		}
			//进行屏幕显示 在游戏模式下 是不会显示空调状态的
		if(Game_Mode==0)
		{
					if(WIFI_intt_flag==0)
					{
						if(z<6) z=6;
						POINT_COLOR=GREEN;
						LCD_LCD_DrawSxRectangle(6,111,z,119);
						if(z<230)
						{
							z+=15;
						}
						else if(flag==1)
							WIFI_intt_flag=1;
					}
				
				if((Time_ms-ms4)>1000)
				{
					if(WIFI_intt_flag==1)
					{
						//Show_Str(30,60,200,16,(u8*)"WIFI",16,0);
						if(z<230)
						{
							flag=1;
							WIFI_intt_flag=0;
							if(z<200) z=200;
						}
						else
						{
							LCD_Clear(WHITE);
							WIFI_intt_flag=2;
						}
					}
					if(WIFI_intt_flag==2)
					{
						LCD_Clear(WHITE);
						POINT_COLOR=BROWN;
						LCD_DrawRectangle(0,20,399,21);
						LCD_DrawRectangle(0,20,399,22);
						LCD_DrawRectangle(0,20,399,23);
						LCD_DrawRectangle(0,20,399,24);
						LCD_DrawRectangle(0,20,399,25);
						POINT_COLOR=BLACK;
						LCD_DrawLine(0,315,399,315);
						LCD_DrawLine(0,316,399,316);
						LCD_DrawLine(0,317,399,317);
						LCD_DrawLine(0,318,399,318);
						i=25;
						Show_Str(30,((i+i+48)/2)-8,200,16,"state           :",16,0);
						i+=48;LCD_DrawLine(0,i,399,i);
						Show_Str(30,((i+i+48)/2)-8,200,16,"noopsyche       :",16,0);
						i+=48;LCD_DrawLine(0,i,399,i);
						Show_Str(30,((i+i+48)/2)-8,200,16,"temperature     :",16,0);
						i+=48;LCD_DrawLine(0,i,399,i);
						Show_Str(30,((i+i+48)/2)-8,200,16,"room temperature:",16,0);
						i+=48;LCD_DrawLine(0,i,399,i);
						Show_Str(30,((i+i+48)/2)-8,200,16,"room humidity   :",16,0);
						i+=48;LCD_DrawLine(0,i,399,i);
						Show_Str(30,((i+i+48)/2)-8,200,16,"illumination    :",16,0);
						
						WIFI_intt_flag=3;
					}
					ms4=Time_ms;
				}
				if(WIFI_intt_flag==3) //数据活动变化
				{
					
					POINT_COLOR=WIFI_on_off? GREEN:RED;     //控制wifi信号的颜色变化 来表示wifi是否正常连接 
					Show_Str(5,0,200,16,(u8*)"WIFI",16,0);
					
					POINT_COLOR=CON_m2m? GREEN:RED;         //控制手机的颜色变化 来表示手机是否在线是否正常连接
					Show_Str(55,0,200,16,(u8*)"SERVER",16,0);
					
					POINT_COLOR=CON_app? GREEN:RED;
					Show_Str(150,0,200,16,(u8*)"APP",16,0);
					
					
					POINT_COLOR=WIFI_on_off? GREEN:RED;			//显示wifi信号强度 
					LCD_ShowNum(210,0,WIFI_sici,2,16);
					
					
					
					
					POINT_COLOR=BLACK;
					i=25;
					if(on_off)
						Show_Str(180,((i+i+48)/2)-8,200,16,"on ",16,0);
					else
						Show_Str(180,((i+i+48)/2)-8,200,16,"off",16,0);
					
					i+=48;
					if(zhineng_flag)
						Show_Str(180,((i+i+48)/2)-8,200,16,"yes",16,0);
					else
						Show_Str(180,((i+i+48)/2)-8,200,16,"no ",16,0);
					
					i+=48;
						LCD_ShowNum(180,((i+i+48)/2)-8, Wd,2,16);
					i+=48;
						LCD_ShowNum(180,((i+i+48)/2)-8, temp,2,16);
					
					i+=48;
					LCD_ShowNum(180,((i+i+48)/2)-8, humi,2,16);
					
					i+=48;
					LCD_ShowNum(180,((i+i+48)/2)-8, Illon_intensity,2,16);
				}
				
		}
			//对从机进行发包处理
		{
			pack_.magic=0x0724;
			pack_.ill=Illon_intensity;
			pack_.room_humi=humi;
			pack_.room_temper=temp;
			pack_.temper=Wd;
			pack_.state=on_off;
			pack_.mode=zhineng_flag;
			pack_.efficacy=pack_.magic+pack_.ill+pack_.room_humi+pack_.room_temper+pack_.state+pack_.temper+pack.mode;
			if(memcmp(&pack,&pack_,AGRR_SIZE)!=0)
			{
				send_usart3((char*)&pack_,AGRR_SIZE);
				pack=pack_;
			}
		}
			//
	}
}
void dispose_remote(void)
{
 u8 key=0;
 key=chu_queue();
	if(key==0)
		return ;
	
	switch(key)
	{
		case 162: //开关键
			if(on_off==0)
			{
				SET_DT();
				on_off=1;
			}
			else
			{
				RESET_DT();
				on_off=0;
			}
				break;
		case 98:  //上键
			if(LED1_flag==1)
			{
				RESET_LED1();
				LED1_flag=0;
			}
			else
			{
				SET_LED1();
				LED1_flag=1;
			}
				break;
		case 194: //右键
			zhineng_flag=0;
			add_Wd();
				break;
		case 168: //下键
				if(LED2_flag==1)
			{
				RESET_LED2();
				LED2_flag=0;
			}
			else
			{
				SET_LED2();
				LED2_flag=1;
			}
				break;
		case 34: //左键
			zhineng_flag=0;
			sub_Wd();
				break;
		case 226: //右上角键
			
		
			if(zhineng_flag==1)
				zhineng_flag=0;
			else
				zhineng_flag=1;
				break;
		case 224: //VOL-
			if(Game_Mode==0)
			{
				Game_Mode=1;
				LCD_Clear(WHITE); //清屏
				Tab_RunGame();
			}
			else
			{
				Game_Mode=0;
				LCD_Clear(WHITE); //清屏
				WIFI_intt_flag=1;
				Tab_StopGmae();
			}
				break;
		case 144: //VOL+
				break;
		case  2:  //中键
			WIFI_intt_flag=1;
				break;
		
		case 104: //1键
				break;
		case 152: //2键
				break;
		case 126: //3键
				break;
		case 48:  //4键
				break;
		case 24:  //5键
				break;
		case 122: // 6键
				break;
		case 16: //7键
				break;
		case 56: //8键
				break;
		case 90: //9键
				break;
		case 66: //0键
				break;
		case 82: //删除
				break;
		default :
			  break;
	}
}

u8   key_queue[100];
u8   key_head=0;
u8   key_end=0;


u8 queue_null(void)
{
	return key_head==key_end;
}

u8 queue_full(void)
{
	return ((key_end+1)%100) == key_head ;
}


u8 ru_queue(u8 data)
{
	if(queue_full())
	{
		printf("队列满\n");
		return 1;
	}
	key_queue[key_end]=data;
	key_end=(key_end+1)%100;
	return 0;
}

u8 chu_queue(void)
{
	u8 data;
	if(queue_null())
	{
		return 0;
	}
	data=key_queue[key_head];
	key_head=(key_head+1)%100;
	return data;
}

void record(void)//记录有效遥控按键事件
{
	static u8 last=0;
	
	if((last>0&&last<6)||!(RmtCnt>0&&RmtCnt<6))
	{
		last = RmtCnt;
		return ;
	}
	ru_queue(Remote_Scan());
	last = RmtCnt;
}

void init(void)
{
	 CON_m2m=0;
	 WIFI_sici=0;
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2 ); //中断分组情况
	 LED_Init(); //初始化LED
	
   SET_LED2();
	 LED2_flag=1;
	
	
	 KEY_Init();
	 SYSTICK_Config();//初始化延时 
	 Usart1_Config_Init();//初始化串口1
	 Usart2_Config_Init(); //串口2初始化 
	 Usart3_Config_Init();//初始化串口3
	 Adc_Init();   //初始化ADC1 1通道 
	 IIC_Config(); //IIC初始化 软件模拟IIC
	 LCD_Init();   //
	 Remote_Init();
	
	 Tim3_config(); //通用定时器 用于PWM输出
	 DHT11_Init();  //温湿度传感器初始化
	 Lsens_Init();  //初始化光敏传感器
	 
	 zhineng_flag=0; //系统默认下 智能模式是不开的 
	 WIFI_intt_flag=0; //wifi初始成功标志复位
	 Game_Mode=0;      //游戏模式标志 
	
	
	BaseTim6_Config();//初始化定时器
	//BaseTim7_Config();//初始化定时器7 用于监视
	Game_Init();
	while(font_init()) 		//检查字库
	{	    
		LCD_ShowString(30,50,200,16,16,"Font Error!");
		sleep_ms(200);				  
		LCD_Fill(30,50,240,66,WHITE);//清除显示	     
		sleep_ms(200);				  
	}
//	Square_whereabouts(10+10,10,50+10,50,320);
//	Square_whereabouts(50+10,10,90+10,50,280);
//	Square_whereabouts(90+10,10,130+10,50,320);
//	Square_whereabouts(90+40+10,10,130+40+10,50,280);
//	Square_whereabouts(90+80+10,10,130+80+10,50,320);
//	Square_whereabouts(90+10,10,130+10,50,280-40);
	
	LCD_DrawRectangle(5,110,235,120);//进度条框框绘制
	sleep_ms(1000);
	
}
void  Square_whereabouts(u16 x1,u16 y1,u16 x2,u16 y2,u16 lim)
{
	u16 i;
		for(i=0;(i+y2)<=lim;i++,i++,i++)
	{
		POINT_COLOR=WHITE;
		LCD_DrawLine(x1,y1+i-1,x2,y1+i-1);
		LCD_DrawLine(x1,y1+i-2,x2,y1+i-2);
		LCD_DrawLine(x1,y1+i-3,x2,y1+i-3);
		POINT_COLOR=BLACK;
		LCD_LCD_DrawSxRectangle(x1,y1+i,x2,y2+i);
	}
}
u8 add_ui(u32  time,u8  always_flag,u16 x,u16 y,const char* str,u8 size)
{
	int i=0;
	for(i=0;i<UI_SIZE;i++)
	{
		if(!PRESENCE_(ui[i].always_flag))
		{
			ui[i].always_flag=always_flag|PRESENCE;
			ui[i].size=size;
			ui[i].x=x;
			ui[i].y=y;
			ui[i].time=time;
			strcpy(ui[i].text,str);
			//printf("%s\n",ui[i].text);
			return 0;
		}
	}
	return 1;
}
void show(void)
{
	static u32 ms=0;
	int o=0;
	int i=0;
	if(Time_ms==ms)
	{
		return ;
	}
	
	for(i=0;i<UI_SIZE;i++)
	{
		if(PRESENCE_(ui[i].always_flag))
		{
			if(ui[i].time==0)
			{
				ui[i].always_flag&=(~PRESENCE);
				for(o=0;o<20&& ui[i].text[o]!='\0';o++) ui[i].text[o]=' ';
				Show_Str(ui[i].x,ui[i].y,200,16,(u8*)ui[i].text,ui[i].size,MODE_1_(ui[i].always_flag));	
			}
			else
			{
				Show_Str(ui[i].x,ui[i].y,200,16,(u8*)ui[i].text,ui[i].size,MODE_1_(ui[i].always_flag));
				if(! ALWAYS_(ui[i].always_flag))
				 ui[i].time--;
			}
		}
		else
		{
			//p不存在
		}
		ms=Time_ms;
	}
}

