#include "drv_ht11.h"
#include "drv_sleep.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stdio.h"

void DHT11_Rst(void)	   
{                 
	DHT11_IO_OUT(); 	//SET OUTPUT
  GPIO_ResetBits(GPIOB,GPIO_Pin_13); 	//拉低DQ
  SYSTICK_DelayNms(20);    	//拉低至少18ms
  GPIO_SetBits(GPIOB,GPIO_Pin_13); 	//DQ=1 
	SYSTICK_DelayNus(30);     	//主机拉高20~40us
}
u8 DHT11_Check(void) 	   
{   
	u8 retry=0;
	DHT11_IO_IN();//SET INPUT	 
  while (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)&&retry<100)//DHT11会拉低40~80us
	{
		retry++;
		SYSTICK_DelayNus(1);
	};	 
	if(retry>=100)return 1;
	else retry=0;
    while (!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)&&retry<100)//DHT11拉低后会再次拉高40~80us
	{
		retry++;
		SYSTICK_DelayNus(1);
	};
	if(retry>=100)return 1;	    
	return 0;
}
u8 DHT11_Read_Bit(void) 			 
{
 	u8 retry=0;
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)&&retry<100)
	{
		retry++;
		SYSTICK_DelayNus(1);
	}
	retry=0;
	while(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)&&retry<100)
	{
		retry++;
		SYSTICK_DelayNus(1);
	}
	SYSTICK_DelayNus(40);
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13))return 1;
	else return 0;		   
}

u8 DHT11_Read_Byte(void)    
{        
    u8 i,dat;
    dat=0;
	for (i=0;i<8;i++) 
	{
   		dat<<=1; 
	    dat|=DHT11_Read_Bit();
    }						    
    return dat;
}



u8 DHT11_Read_Data(u8 *temp,u8 *humi)    
{
 	u8 buf[5];
	u8 i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//读取40位数据
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0];
			*temp=buf[2];
		}
	}else return 1;
	return 0;	    
}
u8 DHT11_Init(void)
{	 
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
	
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);				
 	GPIO_SetBits(GPIOB,GPIO_Pin_13);						
			    
	DHT11_Rst();  
	return DHT11_Check();
}



