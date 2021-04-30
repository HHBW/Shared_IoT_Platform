#include "stm32f10x.h"
#include "DHT11.h"
#include "delay.h"
#include "oled_drv.h"
#include "main.H"
uint8_t buf[5];

unsigned char DHT_Temperature=0;
unsigned char DHT_Humidity=0;
unsigned char DHT_Humi_Fraction=0;
unsigned char DHT_Temp_Fraction=0;
unsigned char DHTT_Buffer[4]={0};
unsigned char DHTH_Buffer[4]={0};
unsigned char DHT_SOC=0;
unsigned char DHT_EOC=0;

void DHTT__Buffer_Updata(void){
	DHTT_Buffer[0]=DHT_Temperature/1000;
	DHTT_Buffer[1]=DHT_Temperature%1000/100;
	DHTT_Buffer[2]=DHT_Temperature%100/10;
	DHTT_Buffer[3]=DHT_Temperature%10;
}
void DHTH__Buffer_Updata(void){
	DHTH_Buffer[0]=DHT_Humidity/1000;
	DHTH_Buffer[1]=DHT_Humidity%1000/100;
	DHTH_Buffer[2]=DHT_Humidity%100/10;
	DHTH_Buffer[3]=DHT_Humidity%10;
}
void Display_DHT_TH(void){
	char i,j;
	for(i=0,j=0;i<5;i++){
		if(i!=3){
			OLED_ShowChar_6x8_Pos(3+i,3,DHTT_Buffer[j]);
			j++;
		}
		else{
			OLED_ShowChar_6x8_Pos(3+i,3,'.');
		}
	}
	for(i=0,j=0;i<5;i++){
		if(i!=3){
			OLED_ShowChar_6x8_Pos(3+i,4,DHTH_Buffer[j]);
			j++;
		}
		else{
			OLED_ShowChar_6x8_Pos(3+i,4,'.');
		}
	}
}

//复位DHT11
void DHT11_Rst(void)	   
{                 
	DHT11_IO_Out(); 	//SET OUTPUT
	DHT11_DQ_OUT(0); 	//拉低DQ
	Delay_ms(20);    	//拉低至少18ms
	DHT11_DQ_OUT(1); 	//DQ=1 
	Delay_us(30);     	//主机拉高20~40us
}
//IO设置为输入模式
void DHT11_IO_In(void)
{
	GPIO_InitTypeDef GPIO_TypeStructure;

	//选择端口
	GPIO_TypeStructure.GPIO_Pin = DHT11_PIN;
	//选择工作模式  通用推挽输出
	GPIO_TypeStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

	//初始化GPIOA
	GPIO_Init(GPIOA,&GPIO_TypeStructure);	
	
	GPIO_Write(GPIOA,0xFF);	
}

//IO设置为输出模式
void DHT11_IO_Out(void)
{
	GPIO_InitTypeDef GPIO_TypeStructure;
	
	//选择端口
	GPIO_TypeStructure.GPIO_Pin = DHT11_PIN;
	//选择工作模式  通用推挽输出
	GPIO_TypeStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	//选择输出最大速度  50MHz
	GPIO_TypeStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//初始化GPIOA
	GPIO_Init(GPIOA,&GPIO_TypeStructure);	
	
	GPIO_Write(GPIOA,0xFF);	
}

//等待DHT11的回应
//返回1:未检测到DHT11的存在
//返回0:存在
uint8_t DHT11_Check(void) 	   
{   
	uint8_t retry=0;
	DHT11_IO_In();         //SET INPUT	 
    while (DHT11_DQ_IN&&retry<100)//DHT11会拉低40~80us
	{
		retry++;
		Delay_us(1);
	}	 
	if(retry>=100)
		return 1;
	else 
		retry=0;
    while (!DHT11_DQ_IN&&retry<100)//DHT11拉低后会再次拉高40~80us
	{
		retry++;
		Delay_us(1);
	};
	if(retry>=100)
		return 1;	    
	return 0;
}

//从DHT11读取一个位
//返回值：1/0
uint8_t DHT11_Read_Bit(void) 			 
{
 	uint8_t retry=0;
	while(DHT11_DQ_IN&&retry<100)//等待变为低电平
	{
		retry++;
		Delay_us(1);
	}
	retry=0;
	while(!DHT11_DQ_IN&&retry<100)//等待变高电平
	{
		retry++;
		Delay_us(1);
	}
	Delay_us(40);//等待40us
	if(DHT11_DQ_IN)return 1;
	else return 0;		   
}

//从DHT11读取一个字节
//返回值：读到的数据
uint8_t DHT11_Read_Byte(void)    
{        
    uint8_t i,dat;
    dat=0;
	for (i=0;i<8;i++) 
	{
//   	while(DHT11_DQ_IN);
//		while(!DHT11_DQ_IN);
//		Delay_us(40);
		dat<<=1; 
//		if(DHT11_DQ_IN){
//	    dat|=0x01;
//		}
		dat|=DHT11_Read_Bit();
  }						    
    return dat;
}

//从DHT11读取一次数据
//temp:温度值(范围:0~50°)
//humi:湿度值(范围:20%~90%)
//返回值：0,正常;1,读取失败
//uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *te,uint8_t *humi,uint8_t *hu)    
//{        
// 	uint8_t buf[5];
//	uint8_t i;
//	DHT11_Rst();
//	if(DHT11_Check()==0)
//	{
//		for(i=0;i<5;i++)//读取40位数据
//		{
//			buf[i]=DHT11_Read_Byte();
//		}
//		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
//		{
//			*humi=buf[0];
//			*hu = buf[1];
//			*temp=buf[2];
//			*te = buf[3];
//		}
//	}
//	else 
//		return 1;
//	return 0;	    
//}

uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *te,uint8_t *humi,uint8_t *hu)    
{        
 	
	uint8_t i;
	static unsigned char step=0;
	switch(step){
		case 0:{
			DMA_Cmd(DMA1_Channel6, DISABLE );
			DMA_Cmd(DMA1_Channel7, DISABLE );
			DHT11_Rst();
			step++;
			break;
		}
		case 1:{
			if(DHT11_Check()==0){
				step++;
			}
			else{
				step=0;
			}
			break;
		}
		case 2:{
			for(i=0;i<5;i++){
				buf[i]=DHT11_Read_Byte();
			}
			step=7;
			break;
		}
		case 7:{
			if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4]){
				*humi=buf[0];
				*hu = buf[1];
				*temp=buf[2];
				*te = buf[3];
				step++;
				break;
			}
			else{
				step=0;
				break;
			}
		}
		case 8:{
			DHTT__Buffer_Updata();
			DHTH__Buffer_Updata();
			DHT_EOC=1;
			DHT_SOC=0;
			step=0;
			DMA_Cmd(DMA1_Channel6, ENABLE );
			DMA_Cmd(DMA1_Channel7, ENABLE );
			break;
		}
		default:break;
	}
	return 0;	    
}

//初始化DHT11的IO口 DQ 同时检测DHT11的存在
//返回1:不存在
//返回0:存在    	 
uint8_t DHT11_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	DHT11_IO_Out();
	DHT11_Rst();
	return DHT11_Check();
}
