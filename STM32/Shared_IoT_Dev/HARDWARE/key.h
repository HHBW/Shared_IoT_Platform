#ifndef __KEY_H_
#define __KEY_H_
#include "stm32f10x.h"
#include "sys.h"

#define KEY1 PEin(7)
#define KEY2 PEin(8)
#define KEY3 PEin(9)
#define KEY4 PEin(10)

void Key_Init(void);	//按键初始化
uint8_t KEY_Scan(void); //按键输入触发

#endif

