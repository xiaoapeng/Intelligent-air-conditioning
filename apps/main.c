#include <string.h>
#include "gizwits_product.h"
#include "common.h"
#include "init.h"
#include "drv_key.h"
#include "drv_led.h"
#include "drv_lcd.h"
#include "drv_ht11.h"
#include "drv_basetim6.h"
#include "Gluttonous_Snake_key.h"
/**
* @brief main

* In this function, the user - associated initialization and the main logical loop are completed

* @param none
* @return none
*/
int main(void)
{  
	
	  u8 key;
    userInit(); //用户初始化 主要初始化驱动

		LED2_flag=0;
	
	
    gizwitsInit();//gizwits 协议初始化接口。

    while(1)
    {
			key=KEY_Scan(1);
    
			userHandle();//数据上行传输 
      gizwitsHandle((dataPoint_t *)&currentDataPoint);  //该函数中完成了相应协议数据的处理及数据上报的等相关操作。
			if(key==KEY0_PRES)
		 {
			 printf("短按KEY0,Wifi Reset\n");
			 gizwitsSetMode(WIFI_RESET_MODE);
		 }
		 if(key==KEY1_PRES_long)
		 {
			 printf("长按KEY1,AP mode\n");
			 gizwitsSetMode(WIFI_SOFTAP_MODE);
		 }
		  if(key==KEY1_PRES)
		 {
			 printf("短按KEY1,AirLink mode\n");
			 gizwitsSetMode(WIFI_AIRLINK_MODE);
		 }
		 
		 Ctrl_Kt();
    }
		
}


