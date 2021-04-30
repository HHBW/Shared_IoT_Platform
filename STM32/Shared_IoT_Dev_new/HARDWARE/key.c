#include <stm32f10x.h>
#include "key.h"
#include "delay.h"


/**************************************************************
*功  能：按键引脚端口配置
*参  数: 无
*返回值: 无
**************************************************************/
void Key_Init(void)
{
	//定义结构体变量
	GPIO_InitTypeDef GPIO_TypeStructure;
	//开启GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);
	//选择端口
	GPIO_TypeStructure.GPIO_Pin = KEY1_PIN|KEY2_PIN;
	//选择工作模式 输入上拉
	GPIO_TypeStructure.GPIO_Mode = GPIO_Mode_IPU;
	//初始化GPIOB
	GPIO_Init(KEY_PORT2,&GPIO_TypeStructure);
	
	GPIO_TypeStructure.GPIO_Pin = KEY3_PIN|KEY4_PIN;
	GPIO_Init(KEY_PORT1,&GPIO_TypeStructure);
}

/**************************************************************
*功  能：按键输入 LED响应
*参  数: 无
*返回值: 0 1 2 3 4 
**************************************************************/
uint8_t KEY_Scan(void)
{
	if(KEY1 == 0)		                    //按键1检测
	{
		//Delay_ms(5);						//按键消抖
		if(KEY1 == 0)						//按键再次检测
		{
			while(KEY1 == 0);				//松手检测
			return 1;						//返回值
		}
	}
	if(KEY2 == 0)							 
	{
		//Delay_ms(5);						 
		if(KEY2 == 0)
		{
			while(KEY2 == 0);				//松手检测
			return 2;
		}
	}
	if(KEY3 == 0)							
	{
		//Delay_ms(5);						 
		if(KEY3 == 0)
		{
			while(KEY3 == 0);				//松手检测
			return 3;
		}
	}
	if(KEY4 == 0)							 
	{
		//Delay_ms(5);
		if(KEY4 == 0)
		{
			while(KEY4 == 0);				//松手检测
			return 4;
		}
	}
	return 0;								//无按键按下 返回0
}
