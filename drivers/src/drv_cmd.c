#include "drv_cmd.h"
#include "string.h"
#include "drv_usart.h"
#include "stdio.h"
#include "drv_basetim6.h"
B_data st_date[b_size];
u8 fun_arr_count=0;
ST_CMD_FUN fun_arr[fun_arr_size];
static FUN cmp_cmd(const char* s);


//检查发来的包是否按照约定 如果是那么找出该包所执行的操作
static FUN cmp_cmd(const char* s)
{
	int i=0;
	for(i=0;i<=fun_arr_count;i++)
	{
		if(!memcmp(s,fun_arr[i].cmd,CMDSIZE))
		{
			return fun_arr[i].fun; 
		}
	}
	return NULL;
}
void Cmd_config(void)
{
	
}
//遍历数组
void bianli(void)
{
	int i=0;
	printf("命令介绍：\n");
	for(i=0;i<=fun_arr_count;i++)
	{
		printf("%s\n",fun_arr[i].cmd);
		printf("%s\n",fun_arr[i].ms);
	}
	
}
//封包函数
void fb(u8 cmd_dev,u8 dz,char *t)
{
	t[0]=0xff;
	t[1]=0xff;
	t[2]=(char)cmd_dev;
	t[3]=0x00;
	t[4]=0x03;
	t[5]=(char)dz;
	t[6]=t[0]+t[1]+t[2]+t[3]+t[4]+t[5];
}




//不允许添加重复的命令，不允许命令超出数组大小
int Add_cmd(u8 cmd_dev,u8 dz,FUN fun,const char *str)
{
	char t[CMDSIZE];
	char *cmd=t;
	fb(cmd_dev,dz,t);
	
	
	if(!fun||cmp_cmd(cmd)!=NULL)
		return -1;
	if(fun_arr_count<fun_arr_size)
	{
		memcpy(fun_arr[fun_arr_count].cmd,cmd,CMDSIZE);
		fun_arr[fun_arr_count].fun=fun;
		strcpy(fun_arr[fun_arr_count].ms,str);
		fun_arr_count++;
		return 0;
	}
	return -1;
}

//收包处理函数 并执行包所代表的操作
//该函数在串口的中断中被调用 
void js(int flag)
{
	static int i=0;
	FUN fun;
	static char cmd[CMDSIZE];
	if(flag==1)
	{
		if(i>=CMDSIZE)
			return ;
  		//cmd[i]=DATE;
			cmd[i] = USART_ReceiveData(USART1);
		  i++;
	}
	if(flag==0)
	{
		
		//cmd[i]='\0';
		//printf("%s\n", cmd);
		i=0;
  	fun=cmp_cmd(cmd);
		if(fun==NULL)
		{
			printf("命令错误....傻子\n");
		}else
			fun();
	}
}
void fs_(void)
{
	int count;
	static int i=0;
	static int ms_=0;
	if(Time_ms-ms_>200)
		{
			for(count=0;count<CMDSIZE;count++)
				fputc(((char*)(&st_date[i]))[count],(FILE *)1);
			i++;
			if(i>=b_size)
				i=0;
		}
		ms_=Time_ms;
	
	
}




