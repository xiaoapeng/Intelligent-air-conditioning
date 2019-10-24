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
	// �ж��ж�Դ
	if (SET == USART_GetITStatus(USART1, USART_IT_RXNE))
	{
		// ���жϱ�־
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		//�����ж� 
		
		
		//js(1);
	}

	if (SET == USART_GetITStatus(USART1, USART_IT_IDLE))
	{
		USART_ClearITPendingBit(USART1, USART_IT_IDLE);
		//�����ж� 
		temp = USART1->SR;
		temp = USART1->DR;
		temp++;
//		js(0);

	}
	
}
void USART2_IRQHandler(void)
{
  	 uint32_t temp;
	// �ж��ж�Դ
	if (SET == USART_GetITStatus(USART2, USART_IT_RXNE))
	{
		// ���жϱ�־
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		//�����ж� 
		
		
		//js(1);
	}

	if (SET == USART_GetITStatus(USART2, USART_IT_IDLE))
	{
		USART_ClearITPendingBit(USART2, USART_IT_IDLE);
		//�����ж� 
		temp = USART2->SR;
		temp = USART2->DR;
		temp++;
		
		
		
		
		
//		js(0);

	}
	
}





//KEY0���������ж�
void EXTI4_IRQHandler(void)
{
	if (SET == EXTI_GetITStatus(EXTI_Line4))
	{
		EXTI_ClearITPendingBit(EXTI_Line4);
		fs_();
	}
}

#if 0
void TIM6_IRQHandler(void) //��ʱ���ж� 
{
//	 unsigned char i;          //��������
//	 unsigned char Send_Count; //������Ҫ���͵����ݸ���
	if(SET == TIM_GetITStatus( TIM6,TIM_FLAG_Update) )
	{
		TIM_ClearITPendingBit (TIM6,TIM_FLAG_Update);
		//���붨ʱ��*************************************************
		Time_ms++;
		
		
		
		
		
		
		
		
		
//		if(!(Time_ms%2))
//		TM3_CH2_change();
//		if(!(Time_ms%50))
//			
//		{            
//			 { 		    		    				 
//					DataScope_Get_Channel_Data( (float)Get_Adc(1) , 1 ); //������ 1.0  д��ͨ�� 1
//		 
//					Send_Count = DataScope_Data_Generate(1); //����10��ͨ���� ��ʽ��֡���ݣ�����֡���ݳ���
//				
//					for( i = 0 ; i < Send_Count; i++)  //ѭ������,ֱ���������   
//					{
//						while((USART1->SR&0X40)==0){;}  
//						USART1->DR = DataScope_OutPut_Buffer[i]; //�Ӵ��ڶ�һ���ֽ����ݳ�ȥ      
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
		
		//printf("DMA���ж��Ѿ����");
		//for(i=0;i<10;i++)
			//printf("%d\n",daming[i]);
	}
	
	
}
const u8*JOYPAD_SYMBOL_TBL[8]=
{"Right","Left","Down","Up","Start","Select","A","B"};//�ֱ��������Ŷ���
void TIM7_IRQHandler(void)
{
	u8 key;
	u16 i;
	if (SET == TIM_GetITStatus(TIM7, TIM_FLAG_Update))
	{
		TIM_ClearITPendingBit(TIM7, TIM_FLAG_Update);
		//���붨ʱ��*************************************************
		Time_15ms++;

		
		
	}
	
	
	
}

//uint8_t i = 0;

//uint8_t rcvData[20] = {0};
//void USART1_IRQHandler(void)
//{
//	uint32_t temp;
//	
//	// �ж��ж�Դ
//	if (SET == USART_GetITStatus(USART1, USART_IT_RXNE))
//	{
//		// ���жϱ�־
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
