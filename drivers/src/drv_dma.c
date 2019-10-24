#include "drv_dma.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "misc.h"
#include "stm32f10x_dma.h"
u32 daming[10]={0};
void DMA1_Config(u32 *src,u32 *aim,u32 src_size)
{

 DMA_InitTypeDef DMA_InitStruct;
 NVIC_InitTypeDef NVIC_InitStruct;



 //使能时钟源
 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

 //配置DmA1的工作参数
 	DMA_InitStruct.DMA_BufferSize=src_size; //传输次数
 	DMA_InitStruct.DMA_DIR=DMA_DIR_PeripheralSRC ;//外设作为数据传输的来源 
	DMA_InitStruct.DMA_M2M=DMA_M2M_Enable;//DMA 通道 x 设置为内存到内存传输 
	DMA_InitStruct.DMA_MemoryBaseAddr=(u32)aim;//存储器为目标地址	
	DMA_InitStruct.DMA_MemoryDataSize=DMA_MemoryDataSize_Word;//32位
	DMA_InitStruct.DMA_MemoryInc=DMA_MemoryInc_Enable;//递增
	DMA_InitStruct.DMA_Mode=DMA_Mode_Normal;//
	DMA_InitStruct.DMA_PeripheralBaseAddr=(u32)src;//配置外设地址为源地址
	DMA_InitStruct.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Word;//32位
	DMA_InitStruct.DMA_PeripheralInc=DMA_PeripheralInc_Enable;//递增
	DMA_InitStruct.DMA_Priority=DMA_Priority_High;//配置优先级为高
	DMA_Init(DMA1_Channel1,&DMA_InitStruct);
//配置中断
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);
	
	NVIC_InitStruct.NVIC_IRQChannel=DMA1_Channel1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
	
	
	DMA_ClearITPendingBit(DMA1_IT_TC1);
	NVIC_Init(&NVIC_InitStruct);
	
}

void Enable_DMA1(void)
{

	DMA_Cmd(DMA1_Channel1,ENABLE);
}






























