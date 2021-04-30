#ifndef __USART_H_
#define __USART_H_
#include <stm32f10x.h>

#define USART1_ITRX_ENABLE 0            //接收中断是否开启
#define USART2_ITRX_ENABLE 1 
#define USART1_RxData_length  255				// 串口1接收数据长度

#define SEND_BUF_SIZE 1024
//#define RECV_BUF_SIZE 5*sizeof(OLED_Screen_Buff)//5 页数据
#define RECV_BUF_SIZE 1024  //5 页数据

extern u8  USART_RX_BUF[USART1_RxData_length]; 	//接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记

void USART1_Configure(u32 baudrate);    //串口1初始化
void USART2_Configure(u32 baudrate);
void USART1_Send_Data_length(uint8_t *str,uint8_t length_num); //串口1发送数据
void USART1_Send_String(uint8_t *str);	//串口1发送字符串
void uart_init(u32 bound);
void USART1_Rx_Handler(void);
void USART2_Rx_Handler(void);
extern u8 x,y;

#endif
