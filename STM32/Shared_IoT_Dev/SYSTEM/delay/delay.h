#ifndef __DELAY_H_
#define __DELAY_H_
#include <stm32f10x.h>


void Delay_Init(void);	//��ʱ��ʼ��
void Delay_us(u32 nus); //΢�뼶��ʱ
void Delay_ms(u16 num); //���뼶��ʱ

#endif


