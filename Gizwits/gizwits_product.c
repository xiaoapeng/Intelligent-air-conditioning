/**
************************************************************
* @file         gizwits_product.c
* @brief        Gizwits control protocol processing, and platform-related hardware initialization 
* @author       Gizwits
* @date         2017-07-19
* @version      V03030000
* @copyright    Gizwits
*
* @note         Gizwits is only for smart hardware
*               Gizwits Smart Cloud for Smart Products
*               Links | Value Added | Open | Neutral | Safety | Own | Free | Ecology
*               www.gizwits.com
*
***********************************************************/
#include <stdio.h>
#include <string.h>
#include "gizwits_product.h"
#include "init.h"
#include "drv_usart.h"
#include "drv_basetim6.h"
#include "drv_led.h"
#include "init.h"
#include "drv_time3.h"
#include "drv_ht11.h"

static uint32_t timerMsCount;

/** Current datapoint */
dataPoint_t currentDataPoint;

/**@} */
/**@name Gizwits User Interface
* @{
*/

/**
* @brief Event handling interface

* Description:

* 1. Users can customize the changes in WiFi module status

* 2. Users can add data points in the function of event processing logic, such as calling the relevant hardware peripherals operating interface

* @param [in] info: event queue
* @param [in] data: protocol data
* @param [in] len: protocol data length
* @return NULL
* @ref gizwits_protocol.h
*/
int8_t gizwitsEventProcess(eventInfo_t *info, uint8_t *gizdata, uint32_t len)
{
  uint8_t i = 0;
  dataPoint_t *dataPointPtr = (dataPoint_t *)gizdata;
  moduleStatusInfo_t *wifiData = (moduleStatusInfo_t *)gizdata;
  protocolTime_t *ptime = (protocolTime_t *)gizdata;
  
#if MODULE_TYPE
  gprsInfo_t *gprsInfoData = (gprsInfo_t *)gizdata;
#else
  moduleInfo_t *ptModuleInfo = (moduleInfo_t *)gizdata;
#endif

  if((NULL == info) || (NULL == gizdata))
  {
    return -1;
  }

  for(i=0; i<info->num; i++)
  {
    switch(info->event[i])
    {
      case EVENT_LED2:
        currentDataPoint.valueLED2 = dataPointPtr->valueLED2;
        GIZWITS_LOG("Evt: EVENT_LED2 %d \n", currentDataPoint.valueLED2);
        if(0x01 == currentDataPoint.valueLED2)
        {
          SET_LED2();
		  LED2_flag=1;
				if(WIFI_intt_flag==0) WIFI_intt_flag=1;
        }
        else
        {
          RESET_LED2();
					LED2_flag=0;  
					if(WIFI_intt_flag==0) WIFI_intt_flag=1;
        }
        break;
      case EVENT_State_Conditioning:
        currentDataPoint.valueState_Conditioning = dataPointPtr->valueState_Conditioning;
        GIZWITS_LOG("Evt: EVENT_State_Conditioning %d \n", currentDataPoint.valueState_Conditioning);
        if(0x01 == currentDataPoint.valueState_Conditioning)
        {
          SET_DT();
        }
        else
        {
          RESET_DT();
        }
        break;
      case EVENT_Intelligent_model:
        currentDataPoint.valueIntelligent_model = dataPointPtr->valueIntelligent_model;
        GIZWITS_LOG("Evt: EVENT_Intelligent_model %d \n", currentDataPoint.valueIntelligent_model);
        if(0x01 == currentDataPoint.valueIntelligent_model)
        {
          //user handle
					zhineng_flag=0x01;
        }
        else
        {
          //user handle  
					zhineng_flag=0x00;					
        }
        break;
      case EVENT_Target_Temperature:
        currentDataPoint.valueTarget_Temperature = dataPointPtr->valueTarget_Temperature;
        GIZWITS_LOG("Evt:EVENT_Target_Temperature %d\n",currentDataPoint.valueTarget_Temperature);
        //user handle
			if(on_off)
					set_wd(currentDataPoint.valueTarget_Temperature);
			  Wd=currentDataPoint.valueTarget_Temperature;
        break;


      case WIFI_SOFTAP:
        break;
      case WIFI_AIRLINK:
        break;
      case WIFI_STATION:
        break;
      case WIFI_CON_ROUTER:
				//只用WIFI模块重新上电 才会进入 
				//连上wifi标志
				RESET_LED2();
				LED2_flag=0;
				if(WIFI_intt_flag==0)
					WIFI_intt_flag=1;//初始化完成
				WIFI_on_off=1;//WIFI状态可能会中途变化 
        break;
      case WIFI_DISCON_ROUTER:
				//wifi断开标志
				WIFI_on_off=0;
        break;
			
			
      case WIFI_CON_M2M:
			//手机在线状态 
			CON_m2m=1;
        break;
      case WIFI_DISCON_M2M:
			//手机断开标志
			CON_m2m=0;
        break;
      case WIFI_RSSI:
        GIZWITS_LOG("RSSI %d\n", wifiData->rssi);
			//wifi强度 
			WIFI_sici=wifiData->rssi;
        break;
      case TRANSPARENT_DATA:
        GIZWITS_LOG("TRANSPARENT_DATA \n");
        //user handle , Fetch data from [data] , size is [len]
        break;
      case WIFI_NTP:
        GIZWITS_LOG("WIFI_NTP : [%d-%d-%d %02d:%02d:%02d][%d] \n",ptime->year,ptime->month,ptime->day,ptime->hour,ptime->minute,ptime->second,ptime->ntp);
        break;
      case MODULE_INFO:
            GIZWITS_LOG("MODULE INFO ...\n");
      #if MODULE_TYPE
            GIZWITS_LOG("GPRS MODULE ...\n");
            //Format By gprsInfo_t
      #else
            GIZWITS_LOG("WIF MODULE ...\n");
            //Format By moduleInfo_t
            GIZWITS_LOG("moduleType : [%d] \n",ptModuleInfo->moduleType);
      #endif
				break;
			case WIFI_CON_APP:
				CON_app=1;
				break;
			case WIFI_DISCON_APP:
				CON_app=0;
				break;
      default:
        break;
    }
  }

  return 0;
}

/**
* User data acquisition

* Here users need to achieve in addition to data points other than the collection of data collection, can be self-defined acquisition frequency and design data filtering algorithm

* @param none
* @return none
*/
void userHandle(void)
{
 /*
    currentDataPoint.valueRoom_Temperature = ;//Add Sensor Data Collection
    currentDataPoint.valueroom_humidity = ;//Add Sensor Data Collection
    currentDataPoint.valueillumination_intensity = ;//Add Sensor Data Collection
    currentDataPoint.valuebuzzing = ;//Add Sensor Data Collection

    */
	currentDataPoint.valueLED2=LED2_flag;                           //led
	currentDataPoint.valueroom_humidity=humi;												//湿度
	currentDataPoint.valueRoom_Temperature=temp;										//温度
	//printf("temp=%d\n",temp);																			//
	currentDataPoint.valueState_Conditioning=on_off;								//空调开关
	currentDataPoint.valueTarget_Temperature=Wd;										//空调温度
	currentDataPoint.valueillumination_intensity=Illon_intensity;		//光照强度
	if(temp>=70)
	{
		currentDataPoint.valuebuzzing=0x01;
	}
	currentDataPoint.valueIntelligent_model=zhineng_flag;						 //智能模式标识位
}

/**
* Data point initialization function

* In the function to complete the initial user-related data
* @param none
* @return none
* @note The developer can add a data point state initialization value within this function
*/
void userInit(void)
{
	init();
    memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));
}


void gizTimerMs(void)
{
    timerMsCount++;
}

/**
* @brief gizGetTimerCount

* Read system time, millisecond timer

* @param none
* @return System time millisecond
*/
uint32_t gizGetTimerCount(void)
{
    return timerMsCount;
}

/**
* @brief mcuRestart

* MCU Reset function

* @param none
* @return none
*/
void mcuRestart(void)
{

}



void TIMER_IRQ_FUN(void)
{
	if (SET == TIM_GetITStatus(TIM6, TIM_FLAG_Update))
	{
		TIM_ClearITPendingBit(TIM6, TIM_FLAG_Update);
		//毫秒定时器*************************************************
		gizTimerMs();
		Time_ms++;
		record();
	}
}
/**
* @brief UART_IRQ_FUN

* UART Serial interrupt function ，For Module communication

* Used to receive serial port protocol data between WiFi module

* @param none
* @return none
*/
void UART_IRQ_FUN(void)
{
	uint8_t value = 0;
	if (RESET != USART_GetITStatus(USART3, USART_IT_RXNE))
	{
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		value = USART_ReceiveData(USART3);//STM32 test demo
		gizPutData(&value, 1);
	}
}


/**
* @brief uartWrite

* Serial write operation, send data to the WiFi module

* @param buf      : Data address
* @param len       : Data length
*
* @return : Not 0,Serial send success;
*           -1，Input Param Illegal
*/
int32_t uartWrite(uint8_t *buf, uint32_t len)
{
    uint32_t i = 0;
    
    if(NULL == buf)
    {
        return -1;
    }
    
    #ifdef PROTOCOL_DEBUG
    GIZWITS_LOG("MCU2WiFi[%4d:%4d]: ", gizGetTimerCount(), len);
    for(i=0; i<len; i++)
    {
        GIZWITS_LOG("%02x ", buf[i]);
    }
    GIZWITS_LOG("\n");
    #endif
		

    for(i=0; i<len; i++)
    {
        USART_SendData(USART3, buf[i]);//STM32 test demo
		while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET); 
		if(i >=2 && buf[i] == 0xFF)
        {
			USART_SendData(USART3, 0x55);
			while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
		}
    }
    return len;
}

