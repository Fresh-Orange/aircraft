/******************** (C) COPYRIGHT 2015 FTC ***************************
 * 作者		 ：FTC
 * 文件名  ：FTC_Drv_Nrf24l01.cpp
 * 描述    ：Nrf24l01
**********************************************************************************/
#include "FTC_Drv_Nrf24l01.h"

FTC_NRF nrf;

//***************************************NRF24L01寄存器指令*******************************************************
#define NRF_READ_REG        0x00  	// 读寄存器指令
#define NRF_WRITE_REG       0x20 	// 写寄存器指令
#define R_RX_PL_WID   	0x60
#define RD_RX_PLOAD     0x61  	// 读取接收数据指令
#define WR_TX_PLOAD     0xA0  	// 写待发数据指令
#define FLUSH_TX        0xE1 	// 冲洗发送 FIFO指令
#define FLUSH_RX        0xE2  	// 冲洗接收 FIFO指令
#define REUSE_TX_PL     0xE3  	// 定义重复装载数据指令
#define NOP             0xFF  	// 保留
//*************************************SPI(nRF24L01)寄存器地址****************************************************
#define CONFIG          0x00  // 配置收发状态，CRC校验模式以及收发状态响应方式
#define EN_AA           0x01  // 自动应答功能设置
#define EN_RXADDR       0x02  // 可用信道设置
#define SETUP_AW        0x03  // 收发地址宽度设置
#define SETUP_RETR      0x04  // 自动重发功能设置
#define RF_CH           0x05  // 工作频率设置
#define RF_SETUP        0x06  // 发射速率、功耗功能设置
#define NRFRegSTATUS    0x07  // 状态寄存器
#define OBSERVE_TX      0x08  // 发送监测功能
#define CD              0x09  // 地址检测           
#define RX_ADDR_P0      0x0A  // 频道0接收数据地址
#define RX_ADDR_P1      0x0B  // 频道1接收数据地址
#define RX_ADDR_P2      0x0C  // 频道2接收数据地址
#define RX_ADDR_P3      0x0D  // 频道3接收数据地址
#define RX_ADDR_P4      0x0E  // 频道4接收数据地址
#define RX_ADDR_P5      0x0F  // 频道5接收数据地址
#define TX_ADDR         0x10  // 发送地址寄存器
#define RX_PW_P0        0x11  // 接收频道0接收数据长度
#define RX_PW_P1        0x12  // 接收频道1接收数据长度
#define RX_PW_P2        0x13  // 接收频道2接收数据长度
#define RX_PW_P3        0x14  // 接收频道3接收数据长度
#define RX_PW_P4        0x15  // 接收频道4接收数据长度
#define RX_PW_P5        0x16  // 接收频道5接收数据长度
#define FIFO_STATUS     0x17  // FIFO栈入栈出状态寄存器设置
//**************************************************************************************
//*********************************************NRF24L01*************************************
#define RX_DR				6		//中断标志
#define TX_DS				5
#define MAX_RT			4

u8	TX_ADDRESS[TX_ADR_WIDTH] = {0x02,0x9B,0xCC,0x00,0x01};	//本地地址
u8	RX_ADDRESS[RX_ADR_WIDTH] = {0x02,0x9B,0xCC,0x00,0x01};	//接收地址	
	
	
/*
*****************************************************************
* 写寄存器
*****************************************************************
*/
uint8_t FTC_NRF::Write_Reg(uint8_t reg, uint8_t value)
{
	uint8_t status;
	CSN_L();					  /* 选通器件 */
	status = RW(reg);  /* 写寄存器地址 */
	RW(value);		  /* 写数据 */
	CSN_H();					  /* 禁止该器件 */
  return 	status;
}
/*
*****************************************************************
* 读寄存器
*****************************************************************
*/
uint8_t FTC_NRF::Read_Reg(uint8_t reg)
{
	uint8_t reg_val;
	CSN_L();					  /* 选通器件 */
	RW(reg);			  /* 写寄存器地址 */
	reg_val = RW(0);	  /* 读取该寄存器返回数据 */
	CSN_H();					  /* 禁止该器件 */
   return 	reg_val;
}
	
/*
*****************************************************************
*
* 写缓冲区
*
*****************************************************************
*/
uint8_t FTC_NRF::Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uchars)
{
	uint8_t i;
	uint8_t status;
	CSN_L();				        /* 选通器件 */
	status = RW(reg);	/* 写寄存器地址 */
	for(i=0; i<uchars; i++)
	{
		RW(pBuf[i]);		/* 写数据 */
	}
	CSN_H();						/* 禁止该器件 */
    return 	status;	
}
/*
*****************************************************************
* 读缓冲区
*****************************************************************
*/
uint8_t FTC_NRF::Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uchars)
{
	uint8_t i;
	uint8_t status;
	CSN_L();						/* 选通器件 */
	status = RW(reg);	/* 写寄存器地址 */
	for(i=0; i<uchars; i++)
	{
		pBuf[i] = RW(0); /* 读取返回数据 */ 	
	}
	CSN_H();						/* 禁止该器件 */
    return 	status;
}


void FTC_NRF::TxPacket(uint8_t * tx_buf, uint8_t len)
{	
	CE_L();		 //StandBy I模式	
	
	Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // 装载接收端地址
	Write_Buf(WR_TX_PLOAD, tx_buf, len); 			 // 装载数据	
	CE_H();		 //置高CE，激发数据发送
}

void FTC_NRF::TxPacket_AP(uint8_t * tx_buf, uint8_t len)
{	
	CE_L();		 //StandBy I模式	
	Write_Buf(0xa8, tx_buf, len); 			 // 装载数据
	CE_H();		 //置高CE
}



void FTC_NRF::Init(u8 model, u8 ch)
{

	CE_L();
	Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH);	//写RX节点地址 
	Write_Buf(NRF_WRITE_REG+TX_ADDR,TX_ADDRESS,TX_ADR_WIDTH); 		//写TX节点地址  
	Write_Reg(NRF_WRITE_REG+EN_AA,0x01); 													//使能通道0的自动应答 
	Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);											//使能通道0的接收地址 
	Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);											//设置自动重发间隔时间:500us;最大自动重发次数:10次 2M波特率下
	Write_Reg(NRF_WRITE_REG+RF_CH,ch);														//设置RF通道为CHANAL
	Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f); 												//设置TX发射参数,0db增益,2Mbps,低噪声增益开启
	//Write_Reg(NRF_WRITE_REG+RF_SETUP,0x07); 												//设置TX发射参数,0db增益,1Mbps,低噪声增益开启
/////////////////////////////////////////////////////////
	if(model==1)				//RX
	{
		Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);								//选择通道0的有效数据宽度 
		Write_Reg(NRF_WRITE_REG + CONFIG, 0x0f);   		 // IRQ收发完成中断开启,16位CRC,主接收
	}
	else if(model==2)		//TX
	{
		Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);								//选择通道0的有效数据宽度 
		Write_Reg(NRF_WRITE_REG + CONFIG, 0x0e);   		 // IRQ收发完成中断开启,16位CRC,主发送
	}
	else if(model==3)		//RX2
	{
		Write_Reg(FLUSH_TX,0xff);
		Write_Reg(FLUSH_RX,0xff);
		Write_Reg(NRF_WRITE_REG + CONFIG, 0x0f);   		 // IRQ收发完成中断开启,16位CRC,主接收
		
		RW(0x50);
		RW(0x73);
		Write_Reg(NRF_WRITE_REG+0x1c,0x01);
		Write_Reg(NRF_WRITE_REG+0x1d,0x06);
	}
	else								//TX2
	{
		Write_Reg(NRF_WRITE_REG + CONFIG, 0x0e);   		 // IRQ收发完成中断开启,16位CRC,主发送
		Write_Reg(FLUSH_TX,0xff);
		Write_Reg(FLUSH_RX,0xff);
		
		RW(0x50);
		RW(0x73);
		Write_Reg(NRF_WRITE_REG+0x1c,0x01);
		Write_Reg(NRF_WRITE_REG+0x1d,0x06);
	}
	CE_H();
}

bool FTC_NRF::Check(void)
{ 
	u8 buf1[5]; 
	u8 i; 
	/*写入5个字节的地址. */ 
	Write_Buf(NRF_WRITE_REG+TX_ADDR,TX_ADDRESS,5); 
	/*读出写入的地址 */ 
	Read_Buf(TX_ADDR,buf1,5); 
	/*比较*/ 
	for(i=0;i<5;i++) 
	{ 
		if(buf1[i]!=TX_ADDRESS[i]) 
			break; 
	} 
	if(i==5)
		return true; //MCU与NRF成功连接 
	else
		return false; //MCU与NRF不正常连接 
}

void FTC_NRF::Check_Event(void)
{
	u8 sta = Read_Reg(NRF_READ_REG + NRFRegSTATUS);
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	if(sta & (1<<RX_DR))//接收中断
	{
		u8 rx_len = Read_Reg(R_RX_PL_WID);
		Read_Buf(RD_RX_PLOAD,NRF24L01_2_RXDATA,rx_len);
		Usart1_Send_Buf(NRF24L01_2_RXDATA,rx_len);
	}
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	if(sta & (1<<TX_DS))
	{
		
	}
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	if(sta & (1<<MAX_RT))
	{
		if(sta & 0x01)	//TX FIFO FULL
		{
			Write_Reg(FLUSH_TX,0xff);
		}
	}
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	Write_Reg(NRF_WRITE_REG + NRFRegSTATUS, sta);//??nrf??????
	sta = Read_Reg(NRF_READ_REG + NRFRegSTATUS);
}

/******************* (C) COPYRIGHT 2015 FTC *****END OF FILE************/
