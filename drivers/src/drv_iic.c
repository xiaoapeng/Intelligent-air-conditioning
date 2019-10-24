#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x.h"
#include "drv_sleep.h"
#include "drv_iic.h"


#define IIC_APBPERIPH_CLK_FUNC				RCC_APB2PeriphClockCmd
#define IIC_APBPERIPH_PORT					RCC_APB2Periph_GPIOB
#define IIC_PORT							GPIOB
#define IIC_PIN_SCL							GPIO_Pin_6
#define IIC_PIN_SDA							GPIO_Pin_7

#define SDA_HIGH()							GPIO_SetBits(IIC_PORT, IIC_PIN_SDA)
#define SDA_LOW()							GPIO_ResetBits(IIC_PORT, IIC_PIN_SDA)
#define SDA_READ()							GPIO_ReadInputDataBit(IIC_PORT, IIC_PIN_SDA)

#define SCL_HIGH()							GPIO_SetBits(IIC_PORT, IIC_PIN_SCL)
#define SCL_LOW()							GPIO_ResetBits(IIC_PORT, IIC_PIN_SCL)

static void iicSdaOut(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = IIC_PIN_SDA;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IIC_PORT, &GPIO_InitStruct);
	
}

static void iicSdaIn(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = IIC_PIN_SDA;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(IIC_PORT, &GPIO_InitStruct);
}

static void iicSendAck(void)
{
	SCL_LOW();
	iicSdaOut();

	SDA_LOW();
	SYSTICK_DelayNus(2);
	SCL_HIGH();
	SYSTICK_DelayNus(2);
	SCL_LOW();
	SYSTICK_DelayNus(2);
	SDA_HIGH();
}

static void iicSendNoack(void)
{
	SCL_LOW();
	iicSdaOut();

	SDA_HIGH();
	SYSTICK_DelayNus(2);
	SCL_HIGH();
	SYSTICK_DelayNus(2);
	SCL_LOW();
}

void IIC_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	// 1�����������ʱ��
	IIC_APBPERIPH_CLK_FUNC(IIC_APBPERIPH_PORT, ENABLE);

	// 2������GPIO�˿�
	GPIO_InitStruct.GPIO_Pin = IIC_PIN_SCL | IIC_PIN_SDA;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IIC_PORT, &GPIO_InitStruct);
	GPIO_SetBits(GPIOB,IIC_PIN_SCL|IIC_PIN_SDA); 	//PB6,PB7 �����
}

void IIC_Start(void)   //���Ϳ�ʼ�ź�
{
	SCL_LOW();					 //ʱ��������
	iicSdaOut();         //����������Ϊ���

	SDA_HIGH();					 //����������
	SCL_HIGH();					 //����ʱ����
	SYSTICK_DelayNus(4); 
	SDA_LOW();					 //���������ߣ���������Чʱ��
	SYSTICK_DelayNus(4); 
	SCL_LOW();		// ǯסIIC���ߣ������������� ����ʱ���� ��ռIIC���� 
}

void IIC_Stop(void)     //����ֹͣ�ź�
{
	SCL_LOW();						//����ʱ����
	iicSdaOut();					//����������Ϊ���

	SDA_LOW();						//����������
	SCL_HIGH();						//����ʱ����
	SYSTICK_DelayNus(4);
	SDA_HIGH();						//���������� 
}

uint8_t IIC_WaitforAck(void)  //�ȴ�Ӧ���ź�
{
	uint8_t counter = 0;

	SCL_LOW();									//����ʱ����
	iicSdaIn();									//����������Ϊ��
	// �ڶ�SDA֮ǰһ���ǵý�SCL������SDA���ϵ�������Ч
	
	
	SDA_HIGH();                 //����ʱ����
	SYSTICK_DelayNus(1);				//�ȴ���������
	SCL_HIGH();									//����ʱ���� 
	SYSTICK_DelayNus(1);				//�ȴ���������
	

	while (SDA_READ())					//��������
	{
		if (counter > 250)				//�������250�λ�û��Ӧ���ź���ô�ͷ���ֹͣ�ź�
		{
			IIC_Stop();

			return EN_NOACK_FLAG;		//û�еȵ�Ӧ���ź� ����Ӧ��ʧ�� 
		}
		counter++;
	}

	SCL_LOW();									//�ɹ��ȵ�Ӧ���ź�

	return EN_ACK_FLAG;					//����Ӧ��ɹ�
}

void IIC_SendByte(uint8_t data) //�������ݲ���
{
	uint8_t i;
	
	SCL_LOW();                    //ʱ��������
	iicSdaOut();   								//����������Ϊ���

	for (i=0; i<8; i++)						
	{
		if (data & 0x80)
			SDA_HIGH();						    //���ݴ��� 
		else
			SDA_LOW();								//���ݴ���
		data <<= 1;
		SCL_HIGH();									//����ʱ���� 
		SYSTICK_DelayNus(4);				
		SCL_LOW();
		SYSTICK_DelayNus(4);
	}
}
void IIC_SendAck(void)
{
	iicSendAck();
}
void IIC_SendnoAck(void)
{
	iicSendNoack();
}
uint8_t IIC_ReceiveByte(en_ack_flag_t ackFlag) //������
{
	uint8_t i;
	uint8_t rcvData = 0;

	SCL_LOW(); 							//����ʱ����
	iicSdaIn();							//����������Ϊ����

	for (i=0; i<8; i++)
	{
		SCL_HIGH();						//����ʱ����
		SYSTICK_DelayNus(4);	//��ʱ�ȴ�
		rcvData <<= 1; 				//��λ
		if (SDA_READ())				//�������� 
			rcvData |= 0x01;		
		SCL_LOW();						//����ʱ����
		SYSTICK_DelayNus(4);  //��ʱ   
	}

	if (EN_ACK_FLAG == ackFlag)
		iicSendAck();
	else
		iicSendNoack();

	return rcvData;
}

