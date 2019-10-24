#ifndef _CMD_H_
#define _CMD_H_
#include "stm32f10x.h"
#include "drv_usart.h"
#define fun_arr_size 100
#define CMDSIZE 7
#define b_size 4
#define  DATE ((char)getData())
typedef void(*FUN)(void);

 


typedef struct fun_str
{
	char cmd[CMDSIZE];
	FUN  fun;
	char ms[2*CMDSIZE];
}ST_CMD_FUN;

typedef struct b_data{
	char data[CMDSIZE];
}B_data;


extern  B_data st_date[b_size];


extern int Add_cmd(u8 cmd_dev,u8 dz,FUN fun,const char *str);
extern void js(int flag);
extern void bianli(void);
extern void Cmd_config(void);
extern void fb(u8 cmd_dev,u8 dz,char *t);
extern void fs_(void);

#endif
