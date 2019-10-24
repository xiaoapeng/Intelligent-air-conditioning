#include "drv_iic.h"
#include "stm32f10x.h"
#include "stdio.h"
#include "drv_sleep.h"
#define AT24CXX_ADDR					0xA0
#define WR_FLAG_R						0x01

static void at24cxxErrorHandler(uint8_t stage)
{
	printf("������%d\n", stage);
}

void AT24CXX_WriteByte(uint8_t addr, uint8_t data)
{
	IIC_Start();              						//������ʼ�ź� 
	IIC_SendByte(AT24CXX_ADDR);						//�����豸��ַ
	if (EN_ACK_FLAG != IIC_WaitforAck())	//�ȴ�Ӧ���ź� 
	{				
		at24cxxErrorHandler(1);							//������
		goto ERR_HANDLER;
	}
	IIC_SendByte(addr);										//���Ͷ�д��ַ 
	if (EN_ACK_FLAG != IIC_WaitforAck())
	{
		at24cxxErrorHandler(2);
		goto ERR_HANDLER;
	}
	IIC_SendByte(data);										//��������
	if (EN_ACK_FLAG != IIC_WaitforAck())
	{
		at24cxxErrorHandler(3);
		goto ERR_HANDLER;
	}
	IIC_Stop();														//����ֹͣ�ź� 
	
ERR_HANDLER:
	return ;
}

uint8_t AT24CXX_ReadByte(uint8_t addr)  //��
{
	uint8_t rcvData = 0;
	
	IIC_Start();													//���Ϳ�ʼ�ź�
	IIC_SendByte(AT24CXX_ADDR);						//�����豸��ַ
	if (EN_ACK_FLAG != IIC_WaitforAck())	//�ȴ�Ӧ��
	{
		at24cxxErrorHandler(1);
		goto ERR_HANDLER;
	}
	
	IIC_SendByte(addr);										//���Ͷ�д��ַ 
	if (EN_ACK_FLAG != IIC_WaitforAck())
	{
		at24cxxErrorHandler(1);
		goto ERR_HANDLER;
	}


	IIC_Start();													 //���Ϳ�ʼ�ź� --Э��涨
	IIC_SendByte(AT24CXX_ADDR | WR_FLAG_R);//�ٴη����豸��ַ ȷ��������
	if (EN_ACK_FLAG != IIC_WaitforAck())   //�ȴ�Ӧ�� 
	{
		at24cxxErrorHandler(1);
		goto ERR_HANDLER;
	}
	rcvData = IIC_ReceiveByte(EN_NOACK_FLAG);//�������� 
	IIC_Stop();																//����ֹͣ�ź�

	return rcvData;														//��������
	
ERR_HANDLER:
	return 0xff;															//������뷵�� 
}




static u8  write_page(u8 addr,u8 *write_u8_buf,u8 *write_u8_buf_end)
{
	u8 i=0;
	u8 addr_offset;
	addr_offset=addr+8;
	addr_offset=addr_offset&0xF8;
	
	addr_offset=addr_offset-addr; //������д��ƫ����
	
	if(write_u8_buf+addr_offset>write_u8_buf_end)
	{
		addr_offset=write_u8_buf_end-write_u8_buf;
	}
	
	for(i=0;i<addr_offset;i++)
	{
		printf("д��%x\n",*write_u8_buf);
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
		 printf("���󷵻�ֵΪ��:\n");
		 return 0;
	 }
	op+=p;
	 sleep_ms(10);
	 while(op<write_u8_buf_size)
	 {
		 if((p=AT24CXX_Writepage_(addr+op,write_u8_buf+op,write_u8_buf+write_u8_buf_size))==0)
		 {
			 printf("���󷵻�ֵΪ��:\n");
			 return 0;
		 }
		 op+=p;
		 sleep_ms(10);
	 }
	 return 1;
}



uint8_t AT24CXX_Readpage(uint8_t addr,u8*buf,u8 size)  //��
{
	int i=0;
	uint8_t rcvData = 0;
	
	IIC_Start();													//���Ϳ�ʼ�ź�
	IIC_SendByte(AT24CXX_ADDR);						//�����豸��ַ
	if (EN_ACK_FLAG != IIC_WaitforAck())	//�ȴ�Ӧ��
	{
		at24cxxErrorHandler(1);
		goto ERR_HANDLER;
	}
	
	IIC_SendByte(addr);										//���Ͷ�д��ַ 
	if (EN_ACK_FLAG != IIC_WaitforAck())
	{
		at24cxxErrorHandler(1);
		goto ERR_HANDLER;
	}


	IIC_Start();													 //���Ϳ�ʼ�ź� --Э��涨
	IIC_SendByte(AT24CXX_ADDR | WR_FLAG_R);//�ٴη����豸��ַ ȷ��������
	if (EN_ACK_FLAG != IIC_WaitforAck())   //�ȴ�Ӧ�� 
	{
		at24cxxErrorHandler(1);
		goto ERR_HANDLER;
	}
	for(i=0;i<size;i++)
	{
		
		if(i<size-1)
			buf[i] = IIC_ReceiveByte(EN_ACK_FLAG);//��������
		else
			buf[i] = IIC_ReceiveByte(EN_NOACK_FLAG);//�������� 
		sleep_us(10);
	}
	IIC_Stop();																//����ֹͣ�ź�

	return rcvData;														//��������
	
ERR_HANDLER:
	return 0xff;															//������뷵�� 
}


