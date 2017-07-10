/******************** (C) COPYRIGHT 2015 FTC ***************************
 * ����		 ��FTC
 * �ļ���  ��FTC_Drv_Nrf24l01.cpp
 * ����    ��Nrf24l01
**********************************************************************************/
#include "FTC_Drv_Nrf24l01.h"

FTC_NRF nrf;

//***************************************NRF24L01�Ĵ���ָ��*******************************************************
#define NRF_READ_REG        0x00  	// ���Ĵ���ָ��
#define NRF_WRITE_REG       0x20 	// д�Ĵ���ָ��
#define R_RX_PL_WID   	0x60
#define RD_RX_PLOAD     0x61  	// ��ȡ��������ָ��
#define WR_TX_PLOAD     0xA0  	// д��������ָ��
#define FLUSH_TX        0xE1 	// ��ϴ���� FIFOָ��
#define FLUSH_RX        0xE2  	// ��ϴ���� FIFOָ��
#define REUSE_TX_PL     0xE3  	// �����ظ�װ������ָ��
#define NOP             0xFF  	// ����
//*************************************SPI(nRF24L01)�Ĵ�����ַ****************************************************
#define CONFIG          0x00  // �����շ�״̬��CRCУ��ģʽ�Լ��շ�״̬��Ӧ��ʽ
#define EN_AA           0x01  // �Զ�Ӧ��������
#define EN_RXADDR       0x02  // �����ŵ�����
#define SETUP_AW        0x03  // �շ���ַ�������
#define SETUP_RETR      0x04  // �Զ��ط���������
#define RF_CH           0x05  // ����Ƶ������
#define RF_SETUP        0x06  // �������ʡ����Ĺ�������
#define NRFRegSTATUS    0x07  // ״̬�Ĵ���
#define OBSERVE_TX      0x08  // ���ͼ�⹦��
#define CD              0x09  // ��ַ���           
#define RX_ADDR_P0      0x0A  // Ƶ��0�������ݵ�ַ
#define RX_ADDR_P1      0x0B  // Ƶ��1�������ݵ�ַ
#define RX_ADDR_P2      0x0C  // Ƶ��2�������ݵ�ַ
#define RX_ADDR_P3      0x0D  // Ƶ��3�������ݵ�ַ
#define RX_ADDR_P4      0x0E  // Ƶ��4�������ݵ�ַ
#define RX_ADDR_P5      0x0F  // Ƶ��5�������ݵ�ַ
#define TX_ADDR         0x10  // ���͵�ַ�Ĵ���
#define RX_PW_P0        0x11  // ����Ƶ��0�������ݳ���
#define RX_PW_P1        0x12  // ����Ƶ��1�������ݳ���
#define RX_PW_P2        0x13  // ����Ƶ��2�������ݳ���
#define RX_PW_P3        0x14  // ����Ƶ��3�������ݳ���
#define RX_PW_P4        0x15  // ����Ƶ��4�������ݳ���
#define RX_PW_P5        0x16  // ����Ƶ��5�������ݳ���
#define FIFO_STATUS     0x17  // FIFOջ��ջ��״̬�Ĵ�������
//**************************************************************************************
//*********************************************NRF24L01*************************************
#define RX_DR				6		//�жϱ�־
#define TX_DS				5
#define MAX_RT			4

u8	TX_ADDRESS[TX_ADR_WIDTH] = {0x02,0x9B,0xCC,0x00,0x01};	//���ص�ַ
u8	RX_ADDRESS[RX_ADR_WIDTH] = {0x02,0x9B,0xCC,0x00,0x01};	//���յ�ַ	
	
	
/*
*****************************************************************
* д�Ĵ���
*****************************************************************
*/
uint8_t FTC_NRF::Write_Reg(uint8_t reg, uint8_t value)
{
	uint8_t status;
	CSN_L();					  /* ѡͨ���� */
	status = RW(reg);  /* д�Ĵ�����ַ */
	RW(value);		  /* д���� */
	CSN_H();					  /* ��ֹ������ */
  return 	status;
}
/*
*****************************************************************
* ���Ĵ���
*****************************************************************
*/
uint8_t FTC_NRF::Read_Reg(uint8_t reg)
{
	uint8_t reg_val;
	CSN_L();					  /* ѡͨ���� */
	RW(reg);			  /* д�Ĵ�����ַ */
	reg_val = RW(0);	  /* ��ȡ�üĴ����������� */
	CSN_H();					  /* ��ֹ������ */
   return 	reg_val;
}
	
/*
*****************************************************************
*
* д������
*
*****************************************************************
*/
uint8_t FTC_NRF::Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uchars)
{
	uint8_t i;
	uint8_t status;
	CSN_L();				        /* ѡͨ���� */
	status = RW(reg);	/* д�Ĵ�����ַ */
	for(i=0; i<uchars; i++)
	{
		RW(pBuf[i]);		/* д���� */
	}
	CSN_H();						/* ��ֹ������ */
    return 	status;	
}
/*
*****************************************************************
* ��������
*****************************************************************
*/
uint8_t FTC_NRF::Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uchars)
{
	uint8_t i;
	uint8_t status;
	CSN_L();						/* ѡͨ���� */
	status = RW(reg);	/* д�Ĵ�����ַ */
	for(i=0; i<uchars; i++)
	{
		pBuf[i] = RW(0); /* ��ȡ�������� */ 	
	}
	CSN_H();						/* ��ֹ������ */
    return 	status;
}


void FTC_NRF::TxPacket(uint8_t * tx_buf, uint8_t len)
{	
	CE_L();		 //StandBy Iģʽ	
	
	Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // װ�ؽ��ն˵�ַ
	Write_Buf(WR_TX_PLOAD, tx_buf, len); 			 // װ������	
	CE_H();		 //�ø�CE���������ݷ���
}

void FTC_NRF::TxPacket_AP(uint8_t * tx_buf, uint8_t len)
{	
	CE_L();		 //StandBy Iģʽ	
	Write_Buf(0xa8, tx_buf, len); 			 // װ������
	CE_H();		 //�ø�CE
}



void FTC_NRF::Init(u8 model, u8 ch)
{

	CE_L();
	Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH);	//дRX�ڵ��ַ 
	Write_Buf(NRF_WRITE_REG+TX_ADDR,TX_ADDRESS,TX_ADR_WIDTH); 		//дTX�ڵ��ַ  
	Write_Reg(NRF_WRITE_REG+EN_AA,0x01); 													//ʹ��ͨ��0���Զ�Ӧ�� 
	Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);											//ʹ��ͨ��0�Ľ��յ�ַ 
	Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);											//�����Զ��ط����ʱ��:500us;����Զ��ط�����:10�� 2M��������
	Write_Reg(NRF_WRITE_REG+RF_CH,ch);														//����RFͨ��ΪCHANAL
	Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f); 												//����TX�������,0db����,2Mbps,���������濪��
	//Write_Reg(NRF_WRITE_REG+RF_SETUP,0x07); 												//����TX�������,0db����,1Mbps,���������濪��
/////////////////////////////////////////////////////////
	if(model==1)				//RX
	{
		Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);								//ѡ��ͨ��0����Ч���ݿ�� 
		Write_Reg(NRF_WRITE_REG + CONFIG, 0x0f);   		 // IRQ�շ�����жϿ���,16λCRC,������
	}
	else if(model==2)		//TX
	{
		Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);								//ѡ��ͨ��0����Ч���ݿ�� 
		Write_Reg(NRF_WRITE_REG + CONFIG, 0x0e);   		 // IRQ�շ�����жϿ���,16λCRC,������
	}
	else if(model==3)		//RX2
	{
		Write_Reg(FLUSH_TX,0xff);
		Write_Reg(FLUSH_RX,0xff);
		Write_Reg(NRF_WRITE_REG + CONFIG, 0x0f);   		 // IRQ�շ�����жϿ���,16λCRC,������
		
		RW(0x50);
		RW(0x73);
		Write_Reg(NRF_WRITE_REG+0x1c,0x01);
		Write_Reg(NRF_WRITE_REG+0x1d,0x06);
	}
	else								//TX2
	{
		Write_Reg(NRF_WRITE_REG + CONFIG, 0x0e);   		 // IRQ�շ�����жϿ���,16λCRC,������
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
	/*д��5���ֽڵĵ�ַ. */ 
	Write_Buf(NRF_WRITE_REG+TX_ADDR,TX_ADDRESS,5); 
	/*����д��ĵ�ַ */ 
	Read_Buf(TX_ADDR,buf1,5); 
	/*�Ƚ�*/ 
	for(i=0;i<5;i++) 
	{ 
		if(buf1[i]!=TX_ADDRESS[i]) 
			break; 
	} 
	if(i==5)
		return true; //MCU��NRF�ɹ����� 
	else
		return false; //MCU��NRF���������� 
}

void FTC_NRF::Check_Event(void)
{
	u8 sta = Read_Reg(NRF_READ_REG + NRFRegSTATUS);
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	if(sta & (1<<RX_DR))//�����ж�
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
