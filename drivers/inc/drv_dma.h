#ifndef _DRV_DMA_H_
#define _DRV_DMA_H_

#include "stm32f10x.h"

void DMA1_Config(u32 *src,u32 *aim,u32 src_size);
void Enable_DMA1(void);
extern u32 daming[10];


#endif
