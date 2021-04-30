#ifndef __LED_H_
#define __LED_H_

#include "sys.h"

#define LED1 PDout(8)
#define LED2 PDout(9)
#define LED3 PDout(10)
#define LED4 PDout(11)

void LED_Init(void);	//LED初始化
void lightwater(void);  //流水灯

#endif

