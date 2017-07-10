/******************** (C) COPYRIGHT 2015 FTC ***************************
 * ����		 ��FTC
 * �ļ���  ��FTC_Drv_I2C_soft.cpp
 * ����    �����I2C
**********************************************************************************/
#include "board.h"
#include "FTC_Drv_I2C_soft.h"

FTC_I2C_Soft::FTC_I2C_Soft(void)
{
	
}

//ģ��IIC��ʼ��
void FTC_I2C_Soft::Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
  RCC_APB2PeriphClockCmd(FTC_RCC_I2C , ENABLE );
  GPIO_InitStructure.GPIO_Pin =  I2C_Pin_SCL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
  GPIO_Init(FTC_GPIO_I2C, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  I2C_Pin_SDA;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(FTC_GPIO_I2C, &GPIO_InitStructure);		
}


void FTC_I2C_Soft::I2C_delay(void)
{
   /*u8 i=0; 
   while(i) 
   { 
     i--; 
   } 
		*/ 	
}

int FTC_I2C_Soft::I2C_Start(void)
{
	SDA_H;
	SCL_H;
	I2C_delay();
	if(!SDA_read)return 0;	//SDA��Ϊ�͵�ƽ������æ,�˳�
	SDA_L;
	I2C_delay();
	if(SDA_read) return 0;	//SDA��Ϊ�ߵ�ƽ�����߳���,�˳�
	SDA_L;
	I2C_delay();
	return 1;	
}

void FTC_I2C_Soft::I2C_Stop(void)
{
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SDA_H;
	I2C_delay();
} 

void FTC_I2C_Soft::I2C_Ack(void)
{	
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
}   

void FTC_I2C_Soft::I2C_NoAck(void)
{	
	SCL_L;
	I2C_delay();
	SDA_H;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
} 

int FTC_I2C_Soft::I2C_WaitAck(void) 	 //����Ϊ:=1��ACK,=0��ACK
{
	SCL_L;
	I2C_delay();
	SDA_H;			
	I2C_delay();
	SCL_H;
	I2C_delay();
	if(SDA_read)
	{
      SCL_L;
			I2C_delay();
      return 0;
	}
	SCL_L;
	I2C_delay();
	return 1;
}

void FTC_I2C_Soft::I2C_SendByte(u8 SendByte) //���ݴӸ�λ����λ//
{
    u8 i=8;
    while(i--)
    {
        SCL_L;
        I2C_delay();
      if(SendByte&0x80)
        SDA_H;  
      else 
        SDA_L;   
        SendByte<<=1;
        I2C_delay();
				SCL_H;
				I2C_delay();
    }
    SCL_L;
}  

u8 FTC_I2C_Soft::I2C_ReadByte(void)  //���ݴӸ�λ����λ//
{ 
    u8 i=8;
    u8 ReceiveByte=0;

    SDA_H;				
    while(i--)
    {
      ReceiveByte<<=1;      
      SCL_L;
      I2C_delay();
			SCL_H;
      I2C_delay();	
      if(SDA_read)
      {
        ReceiveByte|=0x01;
      }
    }
    SCL_L;
    return ReceiveByte;
} 

//���ֽ�д��*******************************************
int FTC_I2C_Soft::I2C_Single_Write(u8 SlaveAddress,u8 REG_Address,u8 REG_data)		
{
  	if(!I2C_Start())return 0;
    I2C_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�//I2C_SendByte(((REG_Address & 0x0700) >>7) | SlaveAddress & 0xFFFE);//���ø���ʼ��ַ+������ַ 
    if(!I2C_WaitAck()){I2C_Stop(); return 0;}
    I2C_SendByte(REG_Address );   //���õ���ʼ��ַ      
    I2C_WaitAck();	
    I2C_SendByte(REG_data);
    I2C_WaitAck();   
    I2C_Stop(); 
    return 1;
}

//���ֽڶ�ȡ*****************************************
int FTC_I2C_Soft::I2C_Single_Read(u8 SlaveAddress,u8 REG_Address)
{   
		unsigned char REG_data;     	
		if(!I2C_Start())return 0;
    I2C_SendByte(SlaveAddress); //I2C_SendByte(((REG_Address & 0x0700) >>7) | REG_Address & 0xFFFE);//���ø���ʼ��ַ+������ַ 
    if(!I2C_WaitAck())
		{
			I2C_Stop();
			return 0;
		}
    I2C_SendByte((u8) REG_Address);   //���õ���ʼ��ַ      
    I2C_WaitAck();
    I2C_Start();
    I2C_SendByte(SlaveAddress+1);
    I2C_WaitAck();

		REG_data= I2C_ReadByte();
    I2C_NoAck();
    I2C_Stop();
    //return TRUE;
	return REG_data;

}	

/******************************************************************************
���ֽڶ�ȡ
******************************************************************************/
int FTC_I2C_Soft::I2C_Mult_Read(u8 SlaveAddress,u8 REG_Address,u8 * ptChar,u8 size)
{
    uint8_t i;
    
    if(size < 1)
			return 0;
    if(!I2C_Start())
			return 0;
    I2C_SendByte(SlaveAddress);
    if(!I2C_WaitAck())
		{
			I2C_Stop();
			return 0;
		}
    I2C_SendByte(REG_Address);    
    I2C_WaitAck();
    
    I2C_Start();
    I2C_SendByte(SlaveAddress+1);
    I2C_WaitAck();
    
    for(i=1;i<size; i++)
    {
        *ptChar++ = I2C_ReadByte();
        I2C_Ack();
    }
    *ptChar++ = I2C_ReadByte();
    I2C_NoAck();
    I2C_Stop();
    return 1;    
}	


/******************* (C) COPYRIGHT 2015 FTC *****END OF FILE************/
