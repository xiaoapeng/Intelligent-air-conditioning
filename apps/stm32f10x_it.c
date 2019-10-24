/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "stm32f10x_usart.h"
#include "stdio.h"
#include "string.h"
#include "drv_cmd.h"
#include "drv_usart.h"
#include "drv_basetim6.h"
#include "drv_sleep.h"
#include "drv_time3.h"
#include "drv_dma.h"
#include "stm32f10x_dma.h"
#include "DataScope_DP.h"
#include "drv_adc.h"
#include  "drv_joypad.h"

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup GPIO_IOToggle
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/


void USART1_IRQHandler(void)
{
	 uint32_t temp;
	// 判断中断源
	if (SET == USART_GetITStatus(USART1, USART_IT_RXNE))
	{
		// 清中断标志
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		//接收中断 
		
		
		//js(1);
	}

	if (SET == USART_GetITStatus(USART1, USART_IT_IDLE))
	{
		USART_ClearITPendingBit(USART1, USART_IT_IDLE);
		//空闲中断 
		temp = USART1->SR;
		temp = USART1->DR;
		temp++;
//		js(0);

	}
	
}
void USART2_IRQHandler(void)
{
  	 uint32_t temp;
	// 判断中断源
	if (SET == USART_GetITStatus(USART2, USART_IT_RXNE))
	{
		// 清中断标志
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		//接收中断 
		
		
		//js(1);
	}

	if (SET == USART_GetITStatus(USART2, USART_IT_IDLE))
	{
		USART_ClearITPendingBit(USART2, USART_IT_IDLE);
		//空闲中断 
		temp = USART2->SR;
		temp = USART2->DR;
		temp++;
		
		
		
		
		
//		js(0);

	}
	
}





//KEY0的上升沿中断
void EXTI4_IRQHandler(void)
{
	if (SET == EXTI_GetITStatus(EXTI_Line4))
	{
		EXTI_ClearITPendingBit(EXTI_Line4);
		fs_();
	}
}

#if 0
void TIM6_IRQHandler(void) //定时器中断 
{
//	 unsigned char i;          //计数变量
//	 unsigned char Send_Count; //串口需要发送的数据个数
	if(SET == TIM_GetITStatus( TIM6,TIM_FLAG_Update) )
	{
		TIM_ClearITPendingBit (TIM6,TIM_FLAG_Update);
		//毫秒定时器*************************************************
		Time_ms++;
		
		
		
		
		
		
		
		
		
//		if(!(Time_ms%2))
//		TM3_CH2_change();
//		if(!(Time_ms%50))
//			
//		{            
//			 { 		    		    				 
//					DataScope_Get_Channel_Data( (float)Get_Adc(1) , 1 ); //将数据 1.0  写入通道 1
//		 
//					Send_Count = DataScope_Data_Generate(1); //生成10个通道的 格式化帧数据，返回帧数据长度
//				
//					for( i = 0 ; i < Send_Count; i++)  //循环发送,直到发送完毕   
//					{
//						while((USART1->SR&0X40)==0){;}  
//						USART1->DR = DataScope_OutPut_Buffer[i]; //从串口丢一个字节数据出去      
//					}
//			 }
//		}
	}
}
#endif

void DMA1_Channel1_IRQHandler(void)
{
//	int i=0;
	if(SET==DMA_GetITStatus(DMA1_IT_TC1))
	{
		DMA_ClearITPendingBit(DMA1_IT_TC1);
		
		//printf("DMA的中断已经完成");
		//for(i=0;i<10;i++)
			//printf("%d\n",daming[i]);
	}
	
	
}
const u8*JOYPAD_SYMBOL_TBL[8]=
{"Right","Left","Down","Up","Start","Select","A","B"};//手柄按键符号定义
void TIM7_IRQHandler(void)
{
	u8 key;
	u16 i;
	if (SET == TIM_GetITStatus(TIM7, TIM_FLAG_Update))
	{
		TIM_ClearITPendingBit(TIM7, TIM_FLAG_Update);
		//毫秒定时器*************************************************
		Time_15ms++;

		
		
	}
	
	
	
}

//uint8_t i = 0;

//uint8_t rcvData[20] = {0};
//void USART1_IRQHandler(void)
//{
//	uint32_t temp;
//	
//	// 判断中断源
//	if (SET == USART_GetITStatus(USART1, USART_IT_RXNE))
//	{
//		// 清中断标志
//		USART_ClearITPendingBit(USART1, USART_IT_RXNE);

//		rcvData[i] = USART_ReceiveData(USART1);
//		rcvData[i+1] = '\0';
//		printf("%c",rcvData[i]);
//		i++;
//	}

//	if (SET == USART_GetITStatus(USART1, USART_IT_IDLE))
//	{
//		USART_ClearITPendingBit(USART1, USART_IT_IDLE);
//		temp = USART1->SR;
//		temp = USART1->DR;
//		
//		printf("%s",rcvData);
//		
//		i = 0;
//	}
//	
//}

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
