#ifndef __KEY_H_
#define __KEY_H_
#include "stm32f10x.h"
#include "sys.h"

//#define KEY1 PEin(7)
//#define KEY2 PEin(8)
//#define KEY3 PEin(9)
//#define KEY4 PEin(10)
#define KEY_PORT1 GPIOA
#define KEY_PORT2 GPIOB
#define KEY1_PIN GPIO_Pin_8
#define KEY2_PIN GPIO_Pin_9
#define KEY3_PIN GPIO_Pin_11
#define KEY4_PIN GPIO_Pin_12

#define KEY1 PBin(8)
#define KEY2 PBin(9)
#define KEY3 PAin(11)
#define KEY4 PAin(12)

void Key_Init(void);	//按键初始化
uint8_t KEY_Scan(void); //按键输入触发

#endif

