#ifndef __FTC_DRV_NRF24L01_H__
#define __FTC_DRV_NRF24L01_H__

#include "board.h"

#define MODEL_RX				1			//��ͨ����
#define MODEL_TX				2			//��ͨ����
#define MODEL_RX2				3			//����ģʽ2,����˫����
#define MODEL_TX2				4			//����ģʽ2,����˫����

#define RX_PLOAD_WIDTH  255  	
#define TX_PLOAD_WIDTH  255  	
#define TX_ADR_WIDTH    5 	 	
#define RX_ADR_WIDTH    5   

class FTC_NRF : public FTC_SPI2
{
	
public:
	
	//��ʼ��,model=1/2/3/4,chΪʵ�õ�ͨ����
	void Init(u8 model, u8 ch);	
	//�������ݰ�,����model 2/4
	void TxPacket(uint8_t * tx_buf, uint8_t len);	
	//�������ݰ�,����model 3
	void TxPacket_AP(uint8_t * tx_buf, uint8_t len);	
	//���NRFģ���Ƿ���������
	bool Check(void);
	//����Ƿ���ͨ���¼�
	void Check_Event(void);

private:

	uint8_t NRF24L01_2_RXDATA[RX_PLOAD_WIDTH];//nrf24l01���յ�������
	uint8_t NRF24L01_2_TXDATA[RX_PLOAD_WIDTH];//nrf24l01��Ҫ���͵�����

	uint8_t Read_Reg(uint8_t reg);
	uint8_t Write_Reg(uint8_t reg, uint8_t value);
	uint8_t Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uchars);
	uint8_t Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uchars);

};

extern FTC_NRF nrf;

#endif















