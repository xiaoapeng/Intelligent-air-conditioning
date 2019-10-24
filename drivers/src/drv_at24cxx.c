#include "drv_iic.h"
#include "stm32f10x.h"
#include "stdio.h"
#include "drv_sleep.h"
#define AT24CXX_ADDR					0xA0
#define WR_FLAG_R						0x01

static void at24cxxErrorHandler(uint8_t stage)
{
	printf("错误在%d\n", stage);
}

void AT24CXX_WriteByte(uint8_t addr, uint8_t data)
{
	IIC_Start();              						//产生开始信号 
	IIC_SendByte(AT24CXX_ADDR);						//发送设备地址
	if (EN_ACK_FLAG != IIC_WaitforAck())	//等待应答信号 
	{				
		at24cxxErrorHandler(1);							//错误处理
		goto ERR_HANDLER;
	}
	IIC_SendByte(addr);										//发送读写地址 
	if (EN_ACK_FLAG != IIC_WaitforAck())
	{
		at24cxxErrorHandler(2);
		goto ERR_HANDLER;
	}
	IIC_SendByte(data);										//发送数据
	if (EN_ACK_FLAG != IIC_WaitforAck())
	{
		at24cxxErrorHandler(3);
		goto ERR_HANDLER;
	}
	IIC_Stop();														//发送停止信号 
	
ERR_HANDLER:
	return ;
}

uint8_t AT24CXX_ReadByte(uint8_t addr)  //读
{
	uint8_t rcvData = 0;
	
	IIC_Start();													//发送开始信号
	IIC_SendByte(AT24CXX_ADDR);						//发送设备地址
	if (EN_ACK_FLAG != IIC_WaitforAck())	//等待应答
	{
		at24cxxErrorHandler(1);
		goto ERR_HANDLER;
	}
	
	IIC_SendByte(addr);										//发送读写地址 
	if (EN_ACK_FLAG != IIC_WaitforAck())
	{
		at24cxxErrorHandler(1);
		goto ERR_HANDLER;
	}


	IIC_Start();													 //发送开始信号 --协议规定
	IIC_SendByte(AT24CXX_ADDR | WR_FLAG_R);//再次发送设备地址 确定读属性
	if (EN_ACK_FLAG != IIC_WaitforAck())   //等待应答 
	{
		at24cxxErrorHandler(1);
		goto ERR_HANDLER;
	}
	rcvData = IIC_ReceiveByte(EN_NOACK_FLAG);//读数据线 
	IIC_Stop();																//发送停止信号

	return rcvData;														//返回数据
	
ERR_HANDLER:
	return 0xff;															//错误代码返回 
}




static u8  write_page(u8 addr,u8 *write_u8_buf,u8 *write_u8_buf_end)
{
	u8 i=0;
	u8 addr_offset;
	addr_offset=addr+8;
	addr_offset=addr_offset&0xF8;
	
	addr_offset=addr_offset-addr; //本次能写的偏移量
	
	if(write_u8_buf+addr_offset>write_u8_buf_end)
	{
		addr_offset=write_u8_buf_end-write_u8_buf;
	}
	
	for(i=0;i<addr_offset;i++)
	{
		printf("写：%x\n",*write_u8_buf);
		IIC_SendByte(*write_u8_buf);
		write_u8_buf++;
		
		
		if (EN_ACK_FLAG != IIC_WaitforAck())
		{
			at24cxxErrorHandler(3+i);
			goto ERR_HANDLER;
		}
	}
	return addr_offset;
	ERR_HANDLER:
	return 0;
	
	
	
	
}
u8 AT24CXX_Writepage_(u8 addr,u8 *write_u8_buf,u8 *write_u8_buf_end)
{
	static int debug=0;
	u8 addr_;
	debug++;
	IIC_Start();
	IIC_SendByte(AT24CXX_ADDR);
	if (EN_ACK_FLAG != IIC_WaitforAck())
	{
		at24cxxErrorHandler(debug+50);
		//at24cxxErrorHandler(1);
		goto ERR_HANDLER;
	}
	IIC_SendByte(addr);
	if (EN_ACK_FLAG != IIC_WaitforAck())
	{
		at24cxxErrorHandler(2);
		goto ERR_HANDLER;
	}
	
	
	addr_=write_page(addr,write_u8_buf,write_u8_buf_end);
	IIC_Stop();
	return addr_;
	ERR_HANDLER:
	return 0;
}
int AT24CXX_Writepage(u8 addr,u8 *write_u8_buf,u8 write_u8_buf_size)
{
	u8 p;
	u8 op=0;
	 if((p=AT24CXX_Writepage_(addr+op,write_u8_buf+op,write_u8_buf+write_u8_buf_size))==0)
	 {
		 printf("错误返回值为零:\n");
		 return 0;
	 }
	op+=p;
	 sleep_ms(10);
	 while(op<write_u8_buf_size)
	 {
		 if((p=AT24CXX_Writepage_(addr+op,write_u8_buf+op,write_u8_buf+write_u8_buf_size))==0)
		 {
			 printf("错误返回值为零:\n");
			 return 0;
		 }
		 op+=p;
		 sleep_ms(10);
	 }
	 return 1;
}



uint8_t AT24CXX_Readpage(uint8_t addr,u8*buf,u8 size)  //读
{
	int i=0;
	uint8_t rcvData = 0;
	
	IIC_Start();													//发送开始信号
	IIC_SendByte(AT24CXX_ADDR);						//发送设备地址
	if (EN_ACK_FLAG != IIC_WaitforAck())	//等待应答
	{
		at24cxxErrorHandler(1);
		goto ERR_HANDLER;
	}
	
	IIC_SendByte(addr);										//发送读写地址 
	if (EN_ACK_FLAG != IIC_WaitforAck())
	{
		at24cxxErrorHandler(1);
		goto ERR_HANDLER;
	}


	IIC_Start();													 //发送开始信号 --协议规定
	IIC_SendByte(AT24CXX_ADDR | WR_FLAG_R);//再次发送设备地址 确定读属性
	if (EN_ACK_FLAG != IIC_WaitforAck())   //等待应答 
	{
		at24cxxErrorHandler(1);
		goto ERR_HANDLER;
	}
	for(i=0;i<size;i++)
	{
		
		if(i<size-1)
			buf[i] = IIC_ReceiveByte(EN_ACK_FLAG);//读数据线
		else
			buf[i] = IIC_ReceiveByte(EN_NOACK_FLAG);//读数据线 
		sleep_us(10);
	}
	IIC_Stop();																//发送停止信号

	return rcvData;														//返回数据
	
ERR_HANDLER:
	return 0xff;															//错误代码返回 
}


