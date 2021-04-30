#ifndef __MAIN_H__
#define __MAIN_H__

typedef struct{
	unsigned char *Recv_Buffer;
	unsigned char *Recv_Buffer2;  //double buffer
	unsigned char *Send_Buffer;
	unsigned int Recv_Len;
	unsigned int Send_Len;
	unsigned char Recv_Finish_Flag;
	unsigned char Send_Finish_Flag;
	unsigned char Recv_Buffer_bFlag;
}U_DMA;

typedef struct {
	unsigned char *Dev_Identy; //the devices identification
	unsigned char Dev_Reset_Flag;// if the devices need to initial
	unsigned char Dev_Is_Init_Status;// indicate the devices if have a master
	unsigned char Dev_Is_InGroup;
	unsigned char *Server_IP;
	unsigned char *Server_Port;
	unsigned char *Host_IP;
	unsigned char *Host_Port;
	unsigned char *Wifi_SSID;
	unsigned char *Wifi_PWD;
	unsigned char Dev_Recv_EN;
	unsigned char Dev_Send_EN;
	unsigned char Dev_Buf_Len;
	unsigned char Dev_Data_Len;
	unsigned char *Dev_Ubuffer;
	struct SK_ASSIST *Next_Member;
	
}SK_ASSIST;

extern SK_ASSIST Dev_Tempture_DS18B20;
extern SK_ASSIST Dev_Tempture_DHT11;
extern U_DMA udma;
extern unsigned char sk_established;
extern unsigned char sensor_wait;

extern void MemClear(unsigned char *dbuff,unsigned int buff_len);
extern void MemCopy(unsigned char *sbuff,unsigned char *dbuff,unsigned int len);
extern unsigned int Size_Of_Str(unsigned char str[]);
extern void Send_Wrapper(unsigned char *sbuff,unsigned char *dbuff,unsigned char len,unsigned char is_end);
extern void Copy_To_Buffer(U_DMA *udma,SK_ASSIST *udev);
extern unsigned int Str_To_Int(unsigned char *arg);
extern void Int_To_Str(unsigned int num,unsigned char *str,unsigned char pbit);
extern unsigned int Str_Splice(unsigned char *dst,unsigned char *src);

#endif

