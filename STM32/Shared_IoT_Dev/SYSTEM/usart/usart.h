#ifndef __USART_H_
#define __USART_H_
#include <stm32f10x.h>

#define USART1_ITRX_ENABLE 0            //�����ж��Ƿ���
#define USART2_ITRX_ENABLE 1 
#define USART1_RxData_length  255				// ����1�������ݳ���

#define SEND_BUF_SIZE 1024
//#define RECV_BUF_SIZE 5*sizeof(OLED_Screen_Buff)//5 ҳ����
#define RECV_BUF_SIZE 1024  //5 ҳ����

extern u8  USART_RX_BUF[USART1_RxData_length]; 	//���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���

void USART1_Configure(u32 baudrate);    //����1��ʼ��
void USART2_Configure(u32 baudrate);
void USART1_Send_Data_length(uint8_t *str,uint8_t length_num); //����1��������
void USART1_Send_String(uint8_t *str);	//����1�����ַ���
void uart_init(u32 bound);
void USART1_Rx_Handler(void);
void USART2_Rx_Handler(void);
extern u8 x,y;

#endif
