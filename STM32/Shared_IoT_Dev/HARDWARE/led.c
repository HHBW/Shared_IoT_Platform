#include <stm32f10x.h>
#include "delay.h"

/**************************************************************
*功  能：引脚端口配置
*参  数: 无
*返回值: 无
**************************************************************/
void LED_Init(void)
{

	//定义结构体变量
	GPIO_InitTypeDef GPIO_TypeStructure;
	//开启GPIOD时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	//选择端口
	GPIO_TypeStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9
								  |GPIO_Pin_10|GPIO_Pin_11;
	//选择工作模式  通用推挽输出
	GPIO_TypeStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	//选择输出最大速度  50MHz
	GPIO_TypeStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//初始化GPIOD
	GPIO_Init(GPIOD,&GPIO_TypeStructure);
	//GPIOD8~11 端口置高电平
	GPIO_SetBits(GPIOD,GPIO_Pin_8|GPIO_Pin_9
				       |GPIO_Pin_10|GPIO_Pin_11);
}


/**************************************************************
*功  能：实现四个LED灯的流水效果
*参  数: 无
*返回值: 无
**************************************************************/
void lightwater(void)
{
	uint8_t  i = 0;
	uint16_t temp_Pin = GPIO_Pin_8;	     //GPIO_Pin_8 = 0000 0001 0000 0000
	for(i = 0;i < 4;i++)
	{
		GPIO_ResetBits(GPIOD,temp_Pin);  //GPIOD8~11端口输出低电平 LED亮
		Delay_ms(500);                   //延时500ms
		GPIO_SetBits(GPIOD,temp_Pin);    //GPIOD8~11端口输出高电平 LED灭
		temp_Pin <<= 1;					 //左移一位
	}
}
