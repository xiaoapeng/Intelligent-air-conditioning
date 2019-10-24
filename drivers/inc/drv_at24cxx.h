#ifndef __DRV_AT24CXX_H__
#define __DRV_AT24CXX_H__
#include "stm32f10x.h"

int AT24CXX_Writepage(u8 addr,u8 *write_u8_buf,u8 write_u8_buf_size);
void AT24CXX_WriteByte(uint8_t addr, uint8_t data);
uint8_t AT24CXX_ReadByte(uint8_t addr);
uint8_t AT24CXX_Readpage(uint8_t addr,u8*buf,u8 size);
#endif // __DRV_AT24CXX_H__

