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

//��λDHT11
void DHT11_Rst(void)	   
{                 
	DHT11_IO_Out(); 	//SET OUTPUT
	DHT11_DQ_OUT(0); 	//����DQ
	Delay_ms(20);    	//��������18ms
	DHT11_DQ_OUT(1); 	//DQ=1 
	Delay_us(30);     	//��������20~40us
}
//IO����Ϊ����ģʽ
void DHT11_IO_In(void)
{
	GPIO_InitTypeDef GPIO_TypeStructure;

	//ѡ��˿�
	GPIO_TypeStructure.GPIO_Pin = DHT11_PIN;
	//ѡ����ģʽ  ͨ���������
	GPIO_TypeStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

	//��ʼ��GPIOA
	GPIO_Init(GPIOA,&GPIO_TypeStructure);	
	
	GPIO_Write(GPIOA,0xFF);	
}

//IO����Ϊ���ģʽ
void DHT11_IO_Out(void)
{
	GPIO_InitTypeDef GPIO_TypeStructure;
	
	//ѡ��˿�
	GPIO_TypeStructure.GPIO_Pin = DHT11_PIN;
	//ѡ����ģʽ  ͨ���������
	GPIO_TypeStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	//ѡ���������ٶ�  50MHz
	GPIO_TypeStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//��ʼ��GPIOA
	GPIO_Init(GPIOA,&GPIO_TypeStructure);	
	
	GPIO_Write(GPIOA,0xFF);	
}

//�ȴ�DHT11�Ļ�Ӧ
//����1:δ��⵽DHT11�Ĵ���
//����0:����
uint8_t DHT11_Check(void) 	   
{   
	uint8_t retry=0;
	DHT11_IO_In();         //SET INPUT	 
    while (DHT11_DQ_IN&&retry<100)//DHT11������40~80us
	{
		retry++;
		Delay_us(1);
	}	 
	if(retry>=100)
		return 1;
	else 
		retry=0;
    while (!DHT11_DQ_IN&&retry<100)//DHT11���ͺ���ٴ�����40~80us
	{
		retry++;
		Delay_us(1);
	};
	if(retry>=100)
		return 1;	    
	return 0;
}

//��DHT11��ȡһ��λ
//����ֵ��1/0
uint8_t DHT11_Read_Bit(void) 			 
{
 	uint8_t retry=0;
	while(DHT11_DQ_IN&&retry<100)//�ȴ���Ϊ�͵�ƽ
	{
		retry++;
		Delay_us(1);
	}
	retry=0;
	while(!DHT11_DQ_IN&&retry<100)//�ȴ���ߵ�ƽ
	{
		retry++;
		Delay_us(1);
	}
	Delay_us(40);//�ȴ�40us
	if(DHT11_DQ_IN)return 1;
	else return 0;		   
}

//��DHT11��ȡһ���ֽ�
//����ֵ������������
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

//��DHT11��ȡһ������
//temp:�¶�ֵ(��Χ:0~50��)
//humi:ʪ��ֵ(��Χ:20%~90%)
//����ֵ��0,����;1,��ȡʧ��
//uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *te,uint8_t *humi,uint8_t *hu)    
//{        
// 	uint8_t buf[5];
//	uint8_t i;
//	DHT11_Rst();
//	if(DHT11_Check()==0)
//	{
//		for(i=0;i<5;i++)//��ȡ40λ����
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

//��ʼ��DHT11��IO�� DQ ͬʱ���DHT11�Ĵ���
//����1:������
//����0:����    	 
uint8_t DHT11_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	DHT11_IO_Out();
	DHT11_Rst();
	return DHT11_Check();
}
