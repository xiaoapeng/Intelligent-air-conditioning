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

	// 1、开启外设的时钟
	IIC_APBPERIPH_CLK_FUNC(IIC_APBPERIPH_PORT, ENABLE);

	// 2、配置GPIO端口
	GPIO_InitStruct.GPIO_Pin = IIC_PIN_SCL | IIC_PIN_SDA;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IIC_PORT, &GPIO_InitStruct);
	GPIO_SetBits(GPIOB,IIC_PIN_SCL|IIC_PIN_SDA); 	//PB6,PB7 输出高
}

void IIC_Start(void)   //发送开始信号
{
	SCL_LOW();					 //时钟线拉高
	iicSdaOut();         //数据线设置为输出

	SDA_HIGH();					 //拉高数据线
	SCL_HIGH();					 //拉高时钟线
	SYSTICK_DelayNus(4); 
	SDA_LOW();					 //拉低数据线（在数据有效时）
	SYSTICK_DelayNus(4); 
	SCL_LOW();		// 钳住IIC总线，利用线与特性 拉低时钟线 霸占IIC总线 
}

void IIC_Stop(void)     //发送停止信号
{
	SCL_LOW();						//拉高时钟线
	iicSdaOut();					//数据线设置为输出

	SDA_LOW();						//拉低数据线
	SCL_HIGH();						//拉高时钟线
	SYSTICK_DelayNus(4);
	SDA_HIGH();						//拉高数据线 
}

uint8_t IIC_WaitforAck(void)  //等待应答信号
{
	uint8_t counter = 0;

	SCL_LOW();									//拉低时钟线
	iicSdaIn();									//设置数据线为读
	// 在读SDA之前一定记得将SCL拉高让SDA线上的数据有效
	
	
	SDA_HIGH();                 //拉高时钟线
	SYSTICK_DelayNus(1);				//等待发送数据
	SCL_HIGH();									//拉高时钟线 
	SYSTICK_DelayNus(1);				//等待发送数据
	

	while (SDA_READ())					//读数据线
	{
		if (counter > 250)				//如果读了250次还没有应答信号那么就发送停止信号
		{
			IIC_Stop();

			return EN_NOACK_FLAG;		//没有等到应答信号 返回应答失败 
		}
		counter++;
	}

	SCL_LOW();									//成功等到应答信号

	return EN_ACK_FLAG;					//返回应答成功
}

void IIC_SendByte(uint8_t data) //发送数据操作
{
	uint8_t i;
	
	SCL_LOW();                    //时钟线拉低
	iicSdaOut();   								//数据线设置为输出

	for (i=0; i<8; i++)						
	{
		if (data & 0x80)
			SDA_HIGH();						    //数据传输 
		else
			SDA_LOW();								//数据传输
		data <<= 1;
		SCL_HIGH();									//拉高时钟线 
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
uint8_t IIC_ReceiveByte(en_ack_flag_t ackFlag) //读操作
{
	uint8_t i;
	uint8_t rcvData = 0;

	SCL_LOW(); 							//拉低时钟线
	iicSdaIn();							//数据线设置为输入

	for (i=0; i<8; i++)
	{
		SCL_HIGH();						//拉高时钟线
		SYSTICK_DelayNus(4);	//延时等待
		rcvData <<= 1; 				//腾位
		if (SDA_READ())				//读数据线 
			rcvData |= 0x01;		
		SCL_LOW();						//拉低时钟线
		SYSTICK_DelayNus(4);  //延时   
	}

	if (EN_ACK_FLAG == ackFlag)
		iicSendAck();
	else
		iicSendNoack();

	return rcvData;
}

