/**
*ÊµÑéÃû³Æ£º´®¿ÚÍ¨ÐÅÊµÑé

*Ó²¼þÄ£¿é£ºÒÆ¶¯»¥Áª¿ª·¢Æ½Ì¨£¨STM32£©  

*ÊµÑé½ÓÏß£ºÒÆ¶¯»¥Áª¿ª·¢Æ½Ì¨£¨STM32£©-----´®¿Ú1  LED1
									PA9---------TX
									PA10--------RX
									PD8--------LED1

*ÊµÑéÏÖÏó£º ÊµÏÖ×ÔÊÕ×Ô·¢¹¦ÄÜ
			      ´®¿ÚÉèÖÃ£º115200 8N1
*¸üÐÂÊ±¼ä£º2018.06.28

*ÐÞ¸Ä¼ÇÂ¼£º

*/

#include <stm32f10x.h>
#include "delay.h"
#include "usart.h"
#include "stdio.h"
#include "oled_drv.h"
#include "time_base.h"
#include "key.h"
#include "led.h"
#include "dma.h"
#include "main.h"
#include "socket.h"
#include "ds18b20.h"
#include "dht11.h"

#define CMD_LINE_SIZE 20
#define DEMO (unsigned char *)"hello world i'm DEMO!!"
#define DS18B20_PERIOD 1
void Ds18b20_Get_Data(unsigned int arg1,unsigned int arg2);
void Dev_Setup(void);

u8 SendBuff[SEND_BUF_SIZE]={0};
u8 RecvBuff[RECV_BUF_SIZE]={0};
u8 RecvBuff2[RECV_BUF_SIZE]={0};

unsigned char sk_established=0;
unsigned char sensor_wait=0;

u8 Cmd_To_Send[][CMD_LINE_SIZE]={
	"AT\r\n",
	"AT+RST\r\n",
	"AT+CIPLAB?\r\n"
};

U_DMA udma={RecvBuff,RecvBuff2,SendBuff,0,0,0,0,0};
unsigned char ds18b20_buffer[128]={0};
unsigned char dht11_buffer[128]={0};
SK_ASSIST Dev_Tempture_DS18B20={
	"ds18b20_root",
	0,
	1,
	0,
	Server_IP,
	Server_Port,
	Host_IP,
	Host_Port,
	Wifi_Ssid,
	Wifi_Pwd,
	0,
	0,
	0,
	1,
	ds18b20_buffer,
	(void*)0
};

SK_ASSIST Dev_Tempture_DHT11={
	"dht11_root",
	0,
	1,
	0,
	Server_IP,
	Server_Port,
	Host_IP,
	Host_Port,
	Wifi_Ssid,
	Wifi_Pwd,
	0,
	0,
	0,
	2,
	dht11_buffer,
	(void*)0
};

//ÖØ¶¨Ïò£¬ÈÃprintfÊä³öµ½´®¿Ú ×¢Òâ´ËÇ°Ó¦µ±Ñ¡ÖÐ Use Micro LIB
//int fputc(int ch, FILE *f)
//{
//    USART_SendData(USART2,ch);
//    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
//    return ch;
//}

void MemClear(unsigned char *dbuff,unsigned int buff_len){
	for(;buff_len>0;buff_len--){
		dbuff[buff_len]=0;
	}
	dbuff[0]=0;
}
void MemCopy(unsigned char *sbuff,unsigned char *dbuff,unsigned int len){
	unsigned int i;
	for(i=0;i<len;i++){
		dbuff[i]=sbuff[i];
	}
}
unsigned int Size_Of_Str(unsigned char str[]){
	unsigned int i;
	for(i=0;str[i]!='\0';i++);
	return i;
}
void Send_Wrapper(unsigned char *sbuff,unsigned char *dbuff,unsigned char len,unsigned char is_end){
	MemCopy(sbuff,dbuff,len);
	udma.Send_Len=len;
	if(is_end){
		dbuff[len]='\r';
		dbuff[len+1]='\n';
		udma.Send_Len+=2;
	}
	dbuff[udma.Send_Len]='\0';
	udma.Send_Len++;
}

void Update_OLED_Buffer(void){
	
}
int main()
{
	unsigned char tmp=0,key=0;
	Dev_Setup();
 	while(1)
	{
		if(Flag_1ms){
			Flag_1ms=0;
			if(udma.Recv_Finish_Flag){
				udma.Recv_Finish_Flag=0;
				Copy_To_Buffer(&udma,&Dev_Tempture_DS18B20);
				
			}
		}
		if(Flag_5ms){
		}
		if(Flag_20ms){
			Flag_20ms=0;
			tmp=KEY_Scan();
			if(tmp>0){
				key=tmp;
				
			}
		}
		if(Flag_100ms){
			Flag_100ms=0;
			if(OLED_Assistant.OLED_Write_EN){
				OLED_Screen_Show();
			}
		}
		if(Flag_200ms){
			Flag_200ms=0;
			Socket_Send(key);
			key=0;
			if(key==4){
				OLED_Clear();
				key=0;
			}
			if(key==1){
				
				key=0;
			}
		}
		
		if(Flag_500ms){
			Flag_500ms=0;
			if(DHT_EOC&&sensor_wait){

				DHT_Share_Data_Demo(DHT_Temperature,DHT_Humidity);

			}
			if(DS18B20_EOC&&(sensor_wait==0)){
				DS18B20_Share_Data_Demo(DS_Temperature);

			}
			//LED1=1;
		}
		
		if(Flag_1s){
			Flag_1s=0;
			//DMA1_Start_Send("hello world",1);
			//LED1=0;
			
			if(sk_established<5){
				sk_established++;
			}
			else if(sk_established==5){
				Socket_Create_TCPClient(&Dev_Tempture_DS18B20);
				sk_established++;
				
			}
			else if(sk_established==6){
				sk_established++;
				OLED_ShowStr_6x8(5,0,"             ");
				DS18B20_SOC=1;
			}
			else if(sk_established){
				if(dht11_send_ok){
					dht11_send_ok=0;
					DS18B20_SOC=1;
					sensor_wait=0;
				}
				if(ds18b20_send_ok){
					ds18b20_send_ok=0;
					DHT_SOC=1;
					sensor_wait=1;
					
				}
				
			}
		
		}
		if(DHT_SOC&&sensor_wait){
				DHT11_Read_Data(&DHT_Temperature,&DHT_Temp_Fraction,&DHT_Humidity,&DHT_Humi_Fraction);
		}
	}
}

void Dev_Setup(void){
	Delay_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	TIM3_Int_Init(10,7200-1);
	USART2_Configure(115200);
	OLED_Hinit(); 	
	OLED_SInit();		
	OLED_Clear();
	DS18B20_Init();
	DHT11_Init();
	LED_Init();
	Key_Init();
	
	MYDMA_Config(DMA1_Channel6,(u32)&(USART2->DR),(u32)RecvBuff,RECV_BUF_SIZE,DMA_Mode_Circular,DMA_DIR_PeripheralSRC);//mode Ñ­Û·,Õ“Î¢Ê¨××¢ÌÕ½SRAM
	MYDMA_Config(DMA1_Channel7,(u32)&(USART2->DR),(u32)SendBuff,SEND_BUF_SIZE,DMA_Mode_Normal,DMA_DIR_PeripheralDST);//mode Ø‡Ñ­Û·,Õ“SRAM××¢ÌÕ½Î¢Ê¨
	DMA_ClearFlag(DMA1_FLAG_TC7);
	DMA_ClearFlag(DMA1_FLAG_HT6);
	DMA_ClearFlag(DMA1_FLAG_TC6);
	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);
	OLED_ShowStr_6x8(5,0,"Share IoT Dev");
}

void Copy_To_Buffer(U_DMA *udma,SK_ASSIST *udev){//¸´ÖÆDMA½ÓÊÕµÄ´®¿ÚÊý¾Ý,ÔÚ¿ÕÏÐÖÐ¶ÏÖÐµ÷ÓÃ
	unsigned int i;
	unsigned char var;
	unsigned char *Recv_Bufferx;
	if(udma->Recv_Buffer_bFlag==0){
		Recv_Bufferx=(*udma).Recv_Buffer2;
	}
	else{
		Recv_Bufferx=(*udma).Recv_Buffer;
	}
	for(i=0;i<udma->Recv_Len;i++){
		var=Recv_Bufferx[i];
		OLED_Screen_Buff[OLED_Assistant.OLED_ShowBuf_DLen]=var;//Ã¿´Î½ÓÊÕ¶àÉÙÊý¾Ý¾ÍÐ´Èë¶àÉÙÊý¾Ý
		OLED_Assistant.OLED_ShowBuf_DLen++;
		if(OLED_Assistant.OLED_ShowBuf_DLen==DIS_BUFFER){
			OLED_Assistant.OLED_ShowBuf_DLen=0;  //ÖØÐÂ´ÓÍ·¿ªÊ¼¸²¸ÇÐ´Èë,ÒòÎª×îÇ°ÃæµÄÄÚÈÝÏÔÊ¾¹ýÁË,¿ÉÒÔ¸²¸Ç
		}
		if(var=='^'){
			udev->Dev_Recv_EN=1;
			udev->Dev_Buf_Len=0;
			continue;
		}
		else if(var=='$'){
			udev->Dev_Recv_EN=0;
			continue;
		}
		if(udev->Dev_Recv_EN){
			udev->Dev_Ubuffer[udev->Dev_Buf_Len]=var;
			udev->Dev_Buf_Len++;
		}
		
	}
	OLED_Screen_Buff[OLED_Assistant.OLED_ShowBuf_DLen]='\0';
	
}
unsigned int Str_To_Int(unsigned char *arg){
	unsigned int num=0,bptr=1;
	unsigned char len=0;
	for(;arg[len]!='\0';len++);
	for(;len>0;len--){
		num+=bptr*arg[len-1];
		bptr*=10;
	}
	return num;
}
void Int_To_Str(unsigned int num,unsigned char *str,unsigned char pbit){//num<10000
	unsigned char i=0,var;
	unsigned int tmp;
	if(pbit==0){
		if(tmp>1000){
			tmp=1000;
		}
		else if(tmp>100){
			tmp=100;
		}
		else if(tmp>10){
			tmp=10;
		}
		else{
			tmp=1;
		}
		for(i=0;;i++){
			var=num/tmp;
			str[i]=0x30+var;
			
			num%=tmp;
			
			if(tmp==1){
				break;
			}
			tmp/=10;
		}
	}
	else{
		tmp=4-pbit;
		str[0]=num/1000+0x30;
		str[1]=num%1000/100+0x30;
		str[2]=num%100/10+0x30;
		str[3]=num%10+0x30;
		var=str[tmp];
		str[tmp]='.';
		for(i=tmp+1;i<5;i++){
			tmp=str[i];
			str[i]=var;
			var=tmp;
		}
		
	}
	
}
unsigned int Str_Splice(unsigned char *dst,unsigned char *src){
	unsigned int len;
	unsigned i;
	len=Size_Of_Str(dst);
	for(i=0;src[i]!='\0';i++){
		dst[len]=src[i];
		len++;
	}
	return i;
}


	////////////////////////////////////////////
	
//	while(1){
//		if(Flag_1ms){
//			Flag_1ms=0;
////			if(DS18B20_SOC){
////				DS18B20_Get_Temp();
////			}
//			
//		}
//		if(DHT_SOC){
//				DHT11_Read_Data(&DHT_Temperature,&DHT_Temp_Fraction,&DHT_Humidity,&DHT_Humi_Fraction);
//		}
//		if(Flag_5ms){
//			Flag_5ms=0;
//			
//		}
//		if(Flag_200ms){
//			Flag_200ms=0;
//			
//		}
//		if(Flag_500ms){
//			Flag_500ms=0;
//			//LED1=1;
//			//Display_DS18B20_Temp();
//			//Display_DHT_TH();
//		}
//		if(Flag_1s){
//			Flag_1s=0;
//			LED1=0;  //è¿™è¾¹å¿…é¡»åŠ ä¸Šè¿™ä¸€å¥,æ”¹æˆ1éƒ½ä¸å¯¹.
//				if(DS18B20_EOC==0){
//					DS18B20_SOC=1;
//				}
//				else{
//					DS18B20_EOC=0;
//				}
//				if(DHT_EOC==0){
//					DHT_SOC=1;
//				}
//				else{
//					DHT_EOC=0;
//				}
//			//}
//		}
//	}

	///////////////////////////////////////////////



