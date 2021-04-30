#include "dma.h"
#include "socket.h"
#include "ds18b20.h"
#include "dht11.h"

#define SK_TEST_FLAG 0
//						FUNCTION MODEL
unsigned char Server_IP[18]="192.168.4.1";
unsigned char Server_Port[5]="333";
unsigned char ds18b20_field[20]={0};
unsigned char dht11_field1[20]={0};
unsigned char dht11_field2[20]={0};
unsigned char dht11_send_ok=0;
unsigned char ds18b20_send_ok=0;

unsigned char *DHT11_Data_Field[2]={dht11_field1,dht11_field2};
unsigned char *DS18B20_Data_Field[1]={ds18b20_field};
#if SK_TEST_FLAG
	unsigned char Host_IP[18]={0};
	unsigned char Host_Port[5]={0};
	unsigned char Wifi_Ssid[32]={0};
	unsigned char Wifi_Pwd[16]={0};
#else
	unsigned char Host_IP[18]={"8.140.164.1"};
	unsigned char Host_Port[5]={"9400"};
	unsigned char Wifi_Ssid[32]={"åˆ›æ–°ç¤¾"};
	unsigned char Wifi_Pwd[16]={"shiyanshi"};
#endif
//////////////////////////////////////////////////////////////
void ESP_Start_Server(void){
	static unsigned char step=0;
	switch(step){
		case 0:{
			step++;
			DMA1_Start_Send("AT+CIPMUX=1",1);
			break;
		}
		case 1:{
			step++;
			DMA1_Start_Send("AT+CIPSERVER=1",1);
			break;
		}
		default:{
			step++;
			if(step>=3){
				step=0;
			}
			break;
		}
	}
}

void Socket_Send(unsigned char key){
	switch(key){
		case 1:{ //connect the server
			//DMA1_Start_Send("AT+CIPSTART=\"TCP\",\"192.168.31.240\",9400",1);
			//Socket_Format_Send("AT+CIFSR");
			DMA1_Start_Send("AT+CIFSR",1);
			break;
		}
		case 2:{//send start
			DMA1_Start_Send("AT",1);
			
			break;
		}
		case 3:{//AT cmd test
			//DMA1_Start_Send("AT",1);
			//DMA1_Start_Send("AT+RST",1);
			//ESP_Start_Server();
			Socket_Create_TCPClient(&Dev_Tempture_DS18B20);
			
			break;
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   		}
		case 4:{//open the tcp server
			//DMA1_Start_Send("AT+CIPMUX=1",1);
			DMA1_Start_Send("AT+CIPCLOSE",1);
			//sk_established=0;
			break;
		}
		default:break;
	}
}
void Socket_Connect_Wifi(SK_ASSIST *Dev){
	unsigned char h_send[128]="AT+CWJAP=";//insert from 9st
	unsigned int len=9;
	h_send[len]='"';
	len++;
	len+=Str_Splice(h_send,Dev->Wifi_SSID);
	h_send[len]='"';//ip end
	len++;
	h_send[len]=',';
	len++;
	h_send[len]='"';
	len++;
	len+=Str_Splice(h_send,Dev->Wifi_PWD);
	h_send[len]='"';
	DMA1_Start_Send(h_send,1);
}
//////////////////////////////////////////////////////////////////////////////
void Socket_Create_TCPClient(SK_ASSIST *Dev){
	
	unsigned int len=18;
	unsigned char h_send[128]="AT+CIPSTART=\"TCP\",";//insert from 18st
	//sk_established=0;
	h_send[len]='"';
	len++;
	len+=Str_Splice(h_send,Dev->Host_IP); //ip_len=15
	h_send[len]='"';//ip end
	len++;
	h_send[len]=',';
	len++;
	len+=Str_Splice(h_send,Dev->Host_Port);
	DMA1_Start_Send(h_send,1);
	//sk_established=1;
}

void Socket_Format_Send(unsigned char *msg){
	unsigned int len;
	unsigned char its[10];
//	unsigned char h_send[15]="AT+CIPSEND=0,";
	unsigned char h_send[15]="AT+CIPSEND=";
	static unsigned char step=0;
	
	if(step==0){
		len=Size_Of_Str(msg);
		Int_To_Str(len,its,0);
		Str_Splice(h_send,its);
		DMA1_Start_Send(h_send,1);//²»ÄÜÁ¢¼´·¢ËÍmsg
	}
	else{
		DMA1_Start_Send(msg,0);
	}
	step++;
	if(step==2){
		step=0;
	}
}
unsigned char Data_Packing(SK_ASSIST *Dev,unsigned char **data){
	unsigned char *pdata;
	unsigned char i;
	unsigned char h_send[128]="^";
	unsigned int len=1;
	len+=Str_Splice(h_send,"dev_data");
	h_send[len]='%';
	len++;
	len+=Str_Splice(h_send,Dev->Dev_Identy);
	h_send[len]='%';
	len++;
	for(i=0;i<Dev->Dev_Data_Len;i++){
		pdata=data[i];
		len+=Str_Splice(h_send,pdata);
		h_send[len]='%';
		len++;
	}
	h_send[len]='$';
	len++;
	for(i=0;i<len;i++){
		Dev->Dev_Ubuffer[i]=h_send[i];
	}
	Dev->Dev_Ubuffer[i]='\0';
	
}
void DHT_Get_Data(unsigned int arg1,unsigned int arg2){
	unsigned char sarg1[10];
	unsigned char sarg2[10];
	unsigned char dfield1[20]="æ¸©åº¦:%";
	unsigned char dfield2[15]="æ¹¿åº¦:%";
	unsigned char i;
	Int_To_Str(arg1,sarg1,0);
	Int_To_Str(arg2,sarg2,0);
	Str_Splice(dfield1,sarg1);
	Str_Splice(dfield2,sarg2);
	for(i=0;dfield1[i]!='\0';i++){
		DHT11_Data_Field[0][i]=dfield1[i];
	}
	//DS18B20_Data_Field[0][i]='%';
	for(i=0;dfield2[i]!='\0';i++){
		DHT11_Data_Field[1][i]=dfield2[i];
	}
	//DS18B20_Data_Field[1][i]='%';
	
}

void DHT_Share_Data_Demo(unsigned int arg1,unsigned int arg2){
	static unsigned char step=0;
	if(step==0){
		DHT_Get_Data(arg1,arg2);
		step++;
	}
	else if(step==1){
		Data_Packing(&Dev_Tempture_DHT11,DHT11_Data_Field);
		step++;
	}
	else if(step==2){
		
		Socket_Format_Send(Dev_Tempture_DHT11.Dev_Ubuffer);
		step=4;
	}
	else if(step>3){
		step--;
	}
	else if(step==3){
		Socket_Format_Send(Dev_Tempture_DHT11.Dev_Ubuffer);
		dht11_send_ok=1;
		DHT_EOC=0;
		step=0;
	}
}

void DS18B20_Get_Data(unsigned int arg){
	unsigned char sarg[10];
	unsigned char dfield[20]="æ¸©åº¦:%";
	unsigned char i;
	Int_To_Str(arg,sarg,1);
	Str_Splice(dfield,sarg);
	for(i=0;dfield[i]!='\0';i++){
		DS18B20_Data_Field[0][i]=dfield[i];
	}
	
}

void DS18B20_Share_Data_Demo(unsigned int arg){
	static unsigned char step=0;
	if(step==0){
		DS18B20_Get_Data(arg);
		step++;
	}
	else if(step==1){
		Data_Packing(&Dev_Tempture_DS18B20,DS18B20_Data_Field);
		step++;
	}
	
	else if(step==2){
		Socket_Format_Send(Dev_Tempture_DS18B20.Dev_Ubuffer);
		step=4;
	}
	else if(step>3){
		step--;
	}
	else if(step==3){
		Socket_Format_Send(Dev_Tempture_DS18B20.Dev_Ubuffer);
		ds18b20_send_ok=1;
		DS18B20_EOC=0;
		step=0;
	}
}

///////////////////////////////////////////////////////////////////////////////

void Parser_SK(SK_ASSIST *Dev){
	unsigned char i,j;
	for(j=0,i=0;Dev->Dev_Ubuffer[i]!='%';i++){
		Dev->Wifi_SSID[j]=Dev->Dev_Ubuffer[i];
		j++;
	}
	for(j=0,i++;Dev->Dev_Ubuffer[i]!='%';i++){  //ÉÏÒ»´ÎIÖ¸Ïò'%',ËùÒÔÒª++
		Dev->Wifi_PWD[j]=Dev->Dev_Ubuffer[i];
		j++;
	}
	
	for(j=0,i++;Dev->Dev_Ubuffer[i]!='%';i++){
		Dev->Host_IP[j]=Dev->Dev_Ubuffer[i];
		j++;
	}
	for(j=0,i++;Dev->Dev_Ubuffer[i]!='%';i++){
		Dev->Host_Port[j]=Dev->Dev_Ubuffer[i];
		j++;
	}
	Dev->Dev_Is_Init_Status=0;
}
void Sk_Send_Demo(SK_ASSIST *Dev){
	//unsigned char msg[128];
	unsigned char i,j;
	unsigned int len;
	for(i=0,j=0;Dev->Dev_Identy[i]!='\0';i++){
		Dev->Dev_Ubuffer[j]=Dev->Dev_Identy[i];
		j++;
	}
	for(i=0;Dev->Host_IP[i]!='\0';i++){
		Dev->Dev_Ubuffer[j]=Dev->Host_IP[i];
		j++;
	}
	Dev->Dev_Ubuffer[j]='\0';
	
	
}
void Dev_Init(SK_ASSIST *Dev){
	//read rom to check the dev if a owned devices
	static unsigned char step=0;
	switch (step){
		case 0:{
			DMA1_Start_Send("AT+CIPMUX=1",1);
			step++;
			break;
		}
		case 1:{
			DMA1_Start_Send("AT+CIPSERVER=1",1);
			step++;
			//step=5;//test point
			break;
		}
		case 2:{
			if((Dev->Dev_Buf_Len>0)&&Dev->Dev_Is_Init_Status){
					Parser_SK(Dev);
					step++;
			}
			break;
		}
		case 3:{
			Socket_Format_Send(Dev->Dev_Identy);
			step++;
			break;
		}
		case 4:{
			Socket_Format_Send(Dev->Dev_Identy);
			step++;
			break;
		}
		case 5:{
			//DMA1_Start_Send("AT+RST",1);//ÏÔÊ¾¹¦ÄÜÓÐÎÊÌâ,½ÓÊÕ¹ý¶àÊý¾ÝÈÝÒ×Æ®
			DMA1_Start_Send("AT+CIPSERVER=0",1);
			step++;
			break;
		}
		case 6:{
			DMA1_Start_Send("AT+CIPMUX=0",1);
			step++;//wait for esp rst
			
			break;
		}
		case 7:{
			//Socket_Format_Send(Dev->Dev_Ubuffer);
			//DMA1_Start_Send("AT",1);
			Socket_Connect_Wifi(Dev);
			step++;
			
			break;
		}
		case 8:{
			//Socket_Connect_Wifi(Dev);
			step++;
			break;
		}
		case 14:{
			Socket_Create_TCPClient(Dev);
			step++;
			break;
		}
		case 15:{
			//Socket_Format_Send(Dev->Dev_Ubuffer);
			step++;
			Dev->Dev_Is_Init_Status=1;
			Dev->Dev_Buf_Len=0;
			break;
		}
		case 16:{
			step++;
			break;
			//Socket_Format_Send("hello world");
		}
		case 17:{
			step=2;
			//Socket_Format_Send("hello world");
			break;
		} 
		default:{
			step++;
			break;
		}
		
	}
	
}


//void Dev_Init(SK_ASSIST *Dev){  //type: demo
//	//read rom to check the dev if a owned devices
//	static unsigned char step=0;
//	switch (step){
//		case 0:{
//			if((Dev->Dev_Buf_Len>0)&&Dev->Dev_Is_Init_Status){
//					Parser_SK(Dev);
//					step++;
//			}
//			break;
//		}
//		case 1:{
//			Sk_Send_Demo(Dev);
//			step++;
//			break;
//		}
//		case 2:{
//			Socket_Format_Send(Dev->Dev_Ubuffer);
//			step++;
//			break;
//		}
//		case 3:{
//			Socket_Format_Send(Dev->Dev_Ubuffer);
//			step=0;
//			Dev->Dev_Is_Init_Status=1;
//			Dev->Dev_Buf_Len=0;
//			break;
//		}
//		default:break;
//		
//	}
//	
//}
