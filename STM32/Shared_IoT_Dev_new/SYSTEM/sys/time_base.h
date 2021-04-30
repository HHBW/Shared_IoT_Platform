#ifndef __TIME_BASE_H__
#define __TIME_BASE_H__
#include "stm32f10x.h"
void TIM3_Int_Init(uint16_t arr,uint16_t psc);

extern unsigned int Time_Counter;
extern unsigned char Flag_1ms;
extern unsigned char Flag_5ms;
extern unsigned char Flag_10ms;
extern unsigned char Flag_20ms;
extern unsigned char Flag_50ms;
extern unsigned char Flag_100ms;
extern unsigned char Flag_200ms;
extern unsigned char Flag_500ms;
extern unsigned char Flag_1s;

#endif
