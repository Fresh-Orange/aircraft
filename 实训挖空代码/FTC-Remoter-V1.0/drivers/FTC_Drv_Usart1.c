#include "board.h"
#include "FTC_Drv_Usart1.h"

#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

typedef union {unsigned char byte[4];float num;}t_floattobyte;
t_floattobyte floattobyte;

void Uart1_Init(u32 br_num)
{
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //开启USART1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	
	
	//串口
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	

	//配置PA9作为USART1　Tx
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA , &GPIO_InitStructure);
	//配置PA10作为USART1　Rx
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA , &GPIO_InitStructure);
	
	//配置USART1
	//中断被屏蔽了
	USART_InitStructure.USART_BaudRate = br_num;       //波特率可以通过地面站配置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //8位数据
	USART_InitStructure.USART_StopBits = USART_StopBits_1;   //在帧结尾传输1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;    //禁用奇偶校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //硬件流控制失能
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //发送、接收使能
	//配置USART1时钟
	USART_ClockInitStruct.USART_Clock = USART_Clock_Disable;  //时钟低电平活动
	USART_ClockInitStruct.USART_CPOL = USART_CPOL_Low;  //SLCK引脚上时钟输出的极性->低电平
	USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge;  //时钟第二个边沿进行数据捕获
	USART_ClockInitStruct.USART_LastBit = USART_LastBit_Disable; //最后一位数据的时钟脉冲不从SCLK输出
	
	USART_Init(USART1, &USART_InitStructure);
	USART_ClockInit(USART1, &USART_ClockInitStruct);

	//使能USART1接收中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	//使能USART1
	USART_Cmd(USART1, ENABLE); 
}

static u8 TxBuffer[256];
static u8 TxCounter=0;
static u8 count=0; 
u8 Rx_Buf[2][32];	//两个32字节的串口接收缓存
static u8 Rx_Act=0;		//正在使用的buf号
static u8 Rx_Adr=0;		//正在接收第几字节
u8 Rx_Ok0 = 0;
u8 Rx_Ok1 = 0;

void Uart1_IRQ(void)
{
	if(USART1->SR & USART_SR_ORE)//ORE中断
	{
		u8 com_data = USART1->DR;
	}
	//发送中断
	if((USART1->SR & (1<<7))&&(USART1->CR1 & USART_CR1_TXEIE))
	{
		USART1->DR = TxBuffer[TxCounter++]; //写DR清除中断标志          
		if(TxCounter == count)
		{
			USART1->CR1 &= ~USART_CR1_TXEIE;		//关闭TXE中断
		}
	}
	//接收中断 (接收寄存器非空) 
	if(USART1->SR & (1<<5))  
	{
		u8 com_data = USART1->DR;
		
			if(Rx_Adr==0)		//寻找帧头0XAAAF
			{
				if(com_data==0xAA)	
				{
					Rx_Buf[Rx_Act][0] = com_data;
					Rx_Adr = 1;
				}
			}
			else if(Rx_Adr==1)
			{
				if(com_data==0xAF)	
				{
					Rx_Buf[Rx_Act][1] = com_data;
					Rx_Adr = 2;
				}
				else
					Rx_Adr = 0;
			}
			else if(Rx_Adr==2)		//FUN
			{
				Rx_Buf[Rx_Act][2] = com_data;
				Rx_Adr = 3;
			}
			else if(Rx_Adr==3)		//LEN
			{
				Rx_Buf[Rx_Act][3] = com_data;
				Rx_Adr = 4;
			}
			else
			{
				Rx_Buf[Rx_Act][Rx_Adr] = com_data;
				Rx_Adr ++;
			}
			if(Rx_Adr==Rx_Buf[Rx_Act][3]+5)		//数据接收完毕
			{
				Rx_Adr = 0;
				if(Rx_Act)	
				{ 
					Rx_Act = 0; 			//切换缓存
					Rx_Ok1 = 1;
				}
				else 				
				{
					Rx_Act = 1;
					Rx_Ok0 = 1;
				}
			}
	}
}
/**************************实现函数********************************************
*******************************************************************************/
uint8_t Uart1_Put_Char(unsigned char DataToSend)
{
	TxBuffer[count++] = DataToSend;
  if(!(USART1->CR1 & USART_CR1_TXEIE))
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE); 
	return DataToSend;
}
uint8_t Uart1_Put_Int16(uint16_t DataToSend)
{
	uint8_t sum = 0;
	TxBuffer[count++] = BYTE1(DataToSend);
	TxBuffer[count++] = BYTE0(DataToSend);
	if(!(USART1->CR1 & USART_CR1_TXEIE))
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	sum += BYTE1(DataToSend);
	sum += BYTE0(DataToSend);
	return sum;
}
uint8_t Uart1_Put_Float(float DataToSend)
{
	uint8_t sum = 0;
	floattobyte.num=DataToSend;
	TxBuffer[count++] = floattobyte.byte[3];  
	TxBuffer[count++] = floattobyte.byte[2];  
	TxBuffer[count++] = floattobyte.byte[1];  
	TxBuffer[count++] = floattobyte.byte[0];  
	if(!(USART1->CR1 & USART_CR1_TXEIE))
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	sum += BYTE3(DataToSend);
	sum += BYTE2(DataToSend);
	sum += BYTE1(DataToSend);
	sum += BYTE0(DataToSend);
	return sum;	
}
void Uart1_Put_String(unsigned char *Str)
{
	//判断Str指向的数据是否有效.
	while(*Str)
	{
	//是否是回车字符 如果是,则发送相应的回车 0x0d 0x0a
	if(*Str=='\r')Uart1_Put_Char(0x0d);
		else if(*Str=='\n')Uart1_Put_Char(0x0a);
			else Uart1_Put_Char(*Str);
	//指针++ 指向下一个字节.
	Str++;
	}
}
void Usart1_Send_Buf(unsigned char *DataToSend , u8 data_num)
{
	for(u8 i=0;i<data_num;i++)
		TxBuffer[count++] = *(DataToSend+i);
	if(!(USART1->CR1 & USART_CR1_TXEIE))
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE); 
}
