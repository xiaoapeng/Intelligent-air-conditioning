#ifndef __DRV_IIC_H__
#define __DRV_IIC_H__

#include "stm32f10x.h"

typedef enum en_ack_flag{
	EN_ACK_FLAG = 0,
	EN_NOACK_FLAG,

	EN_ACK_MAX
} en_ack_flag_t;
extern void IIC_SendnoAck(void);
extern void IIC_SendAck(void);
extern void IIC_Config(void);
extern void IIC_Start(void);
extern void IIC_Stop(void);
extern uint8_t IIC_WaitforAck(void);
extern void IIC_SendByte(uint8_t data);
extern uint8_t IIC_ReceiveByte(en_ack_flag_t ackFlag);

#endif // __DRV_IIC_H__

