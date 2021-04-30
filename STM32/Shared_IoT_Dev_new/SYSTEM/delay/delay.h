#ifndef __DELAY_H_
#define __DELAY_H_
#include <stm32f10x.h>


void Delay_Init(void);	//延时初始化
void Delay_us(u32 nus); //微秒级延时
void Delay_ms(u16 num); //毫秒级延时

#endif


