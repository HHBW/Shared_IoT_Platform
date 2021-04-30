#ifndef __DHT11_H
#define __DHT11_H 

#include "stm32f10x.h"

/*************************************************************
** IO��������
*************************************************************/	
#define DHT11_PIN GPIO_Pin_1
#define	DHT11_DQ_OUT(X)   	GPIO_WriteBit( GPIOA,DHT11_PIN,(BitAction)X )// ���ݶ˿���� 
#define	DHT11_DQ_IN    		GPIO_ReadInputDataBit( GPIOA,DHT11_PIN )	   // ���ݶ˿�����

unsigned char DHT11_Init(void);//��ʼ��DHT11
unsigned char DHT11_Read_Data(unsigned char *temp,unsigned char *te,unsigned char *humi,unsigned char *hu);//��ȡ��ʪ��
unsigned char DHT11_Read_Byte(void);//����һ���ֽ�
unsigned char DHT11_Read_Bit(void);//����һ��λ
unsigned char DHT11_Check(void);//����Ƿ����DHT11
void DHT11_Rst(void);//��λDHT11    

void DHT11_IO_Out(void);
void DHT11_IO_In(void);

extern unsigned char DHT_Temperature;
extern unsigned char DHT_Humidity;
extern unsigned char DHT_Humi_Fraction;
extern unsigned char DHT_Temp_Fraction;
extern unsigned char DHTT_Buffer[4];
extern unsigned char DHTH_Buffer[4];
extern unsigned char DHT_SOC;
extern unsigned char DHT_EOC;
extern void Display_DHT_TH(void);

#endif















