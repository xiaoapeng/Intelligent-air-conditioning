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
    userInit(); //�û���ʼ�� ��Ҫ��ʼ������

		LED2_flag=0;
	
	
    gizwitsInit();//gizwits Э���ʼ���ӿڡ�

    while(1)
    {
			key=KEY_Scan(1);
    
			userHandle();//�������д��� 
      gizwitsHandle((dataPoint_t *)&currentDataPoint);  //�ú������������ӦЭ�����ݵĴ��������ϱ��ĵ���ز�����
			if(key==KEY0_PRES)
		 {
			 printf("�̰�KEY0,Wifi Reset\n");
			 gizwitsSetMode(WIFI_RESET_MODE);
		 }
		 if(key==KEY1_PRES_long)
		 {
			 printf("����KEY1,AP mode\n");
			 gizwitsSetMode(WIFI_SOFTAP_MODE);
		 }
		  if(key==KEY1_PRES)
		 {
			 printf("�̰�KEY1,AirLink mode\n");
			 gizwitsSetMode(WIFI_AIRLINK_MODE);
		 }
		 
		 Ctrl_Kt();
    }
		
}


