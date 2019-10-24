#ifndef _DRV_TIME3_H_
#define _DRV_TIME3_H_
#include "stm32f10x.h"

#define PERIOD 500
extern u8 Wd;
extern u8 on_off;

extern void add_Wd(void);
extern void sub_Wd(void);
extern u8 Get_Wd(void);
extern void set_wd(u8 wd);
extern void Tim3_config(void);
extern void TM3_CH2_change(void);
#define   SET_DT()    set_wd(Wd);on_off=1
#define   RESET_DT()  TIM_SetCompare2(TIM3,PERIOD);on_off=0



#endif 





















