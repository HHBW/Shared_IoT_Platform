#ifndef __DS18B20_H
#define __DS18B20_H 
#include "sys.h"   
 
#define DS18B20_PIN GPIO_Pin_0
#define DS18B20_PORT GPIOB
//IO方向设置
//#define DS18B20_IO_IN()  {GPIOG->CRH&=0XFFFF0FFF;GPIOG->CRH|=8<<12;}
//#define DS18B20_IO_OUT() {GPIOG->CRH&=0XFFFF0FFF;GPIOG->CRH|=3<<12;} 
#define DS18B20_IO_IN()     DQ1820_IN() 
#define DS18B20_IO_OUT()   DQ1820_OUT()
////IO操作函数		


#define	DS18B20_DQ_OUT PBout(DS18B20_PIN)		//数据端口	PA0 
#define	DS18B20_DQ_IN   PBin(DS18B20_PIN)  	//数据端口	PA0 
 
unsigned char DS18B20_Init(void);				//初始化DS18B20
void DQ1820_IN(void);
void DQ1820_OUT(void);
void DS18B20_Get_Temp(void);		//获取温度
void DS18B20_Start(void);			//开始温度转换
void DS18B20_Write_Byte(unsigned char dat);	//写入一个字节
unsigned char DS18B20_Read_Byte(void);			//读出一个字节
unsigned char DS18B20_Read_Bit(void);			//读出一个位
unsigned char DS18B20_Check(void);				//检测是否存在DS18B20
void DS18B20_Rst(void);				//复位DS18B20
void DS18B20_Test(void); 			//DS18B20测试
void DS18B20_WHILE(void); 			//DS18B20检测

extern unsigned char DS18B20_EOC;
extern void Display_DS18B20_Temp(void);
extern unsigned char DS18B20_SOC;
extern unsigned int DS_Temperature;

#endif















