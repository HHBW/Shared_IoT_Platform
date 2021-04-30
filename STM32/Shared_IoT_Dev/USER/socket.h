#ifndef __SOCKET_H__
#define __SOCKET_H__
#include "main.h"

#define ORIGIN_IP "192.168.4.1"
#define ORIGIN_PORT 333
//传感器只负责给服务器发送数据,而服务器负责把传感器数据分发给有此设备的所有用户

extern unsigned char Server_IP[18];
extern unsigned char Host_IP[18];
extern unsigned char Server_Port[5];
extern unsigned char Host_Port[5];
extern unsigned char Wifi_Ssid[32];
extern unsigned char Wifi_Pwd[16];
extern void Socket_Send(unsigned char key);
extern void Dev_Init(SK_ASSIST *Dev);
extern void Socket_Format_Send(unsigned char *msg);
extern void Socket_Create_TCPClient(SK_ASSIST *Dev);
extern unsigned char Data_Packing(SK_ASSIST *Dev,unsigned char **data);
extern void DS18B20_Share_Data_Demo(unsigned int arg);
extern void DHT_Share_Data_Demo(unsigned int arg1,unsigned int arg2);
extern unsigned char dht11_send_ok;
extern unsigned char ds18b20_send_ok;




#endif

