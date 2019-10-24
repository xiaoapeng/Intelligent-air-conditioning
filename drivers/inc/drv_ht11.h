#ifndef __DRV_HT11_H
#define __DRV_HT11_H 
#include "stm32f10x.h"   

#define DHT11_IO_IN()  {GPIOB->CRH&=0XFF0FFFFF;GPIOB->CRH|=0x00800000;}
#define DHT11_IO_OUT() {GPIOB->CRH&=0XFF0FFFFF;GPIOB->CRH|=0x00300000;}
										   
#define	DHT11_DQ_OUT PAout(8) //
#define	DHT11_DQ_IN  PAin(8)  //


u8 DHT11_Init(void);
u8 DHT11_Read_Data(u8 *temp,u8 *humi); //¶ÁÈ¡ÎÂÊª¶È
u8 DHT11_Read_Byte(void);
u8 DHT11_Read_Bit(void);
u8 DHT11_Check(void);
void DHT11_Rst(void);   
#endif















