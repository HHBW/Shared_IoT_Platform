#ifndef __DS18B20_H
#define __DS18B20_H 
#include "sys.h"   
 
//IO��������
//#define DS18B20_IO_IN()  {GPIOG->CRH&=0XFFFF0FFF;GPIOG->CRH|=8<<12;}
//#define DS18B20_IO_OUT() {GPIOG->CRH&=0XFFFF0FFF;GPIOG->CRH|=3<<12;} 
#define DS18B20_IO_IN()     DQ1820_IN() 
#define DS18B20_IO_OUT()   DQ1820_OUT()
////IO��������											    
#define	DS18B20_DQ_OUT PAout(0)		//���ݶ˿�	PA0 
#define	DS18B20_DQ_IN   PAin(0)  	//���ݶ˿�	PA0 
 
unsigned char DS18B20_Init(void);				//��ʼ��DS18B20
void DQ1820_IN(void);
void DQ1820_OUT(void);
void DS18B20_Get_Temp(void);		//��ȡ�¶�
void DS18B20_Start(void);			//��ʼ�¶�ת��
void DS18B20_Write_Byte(unsigned char dat);	//д��һ���ֽ�
unsigned char DS18B20_Read_Byte(void);			//����һ���ֽ�
unsigned char DS18B20_Read_Bit(void);			//����һ��λ
unsigned char DS18B20_Check(void);				//����Ƿ����DS18B20
void DS18B20_Rst(void);				//��λDS18B20
void DS18B20_Test(void); 			//DS18B20����
void DS18B20_WHILE(void); 			//DS18B20���

extern unsigned char DS18B20_EOC;
extern void Display_DS18B20_Temp(void);
extern unsigned char DS18B20_SOC;
extern unsigned int DS_Temperature;

#endif















