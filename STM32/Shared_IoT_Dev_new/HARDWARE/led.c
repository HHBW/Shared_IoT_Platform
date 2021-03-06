#include <stm32f10x.h>
#include "delay.h"

/**************************************************************
*功  能：引脚端口配置
*参  数: 无
*返回值: 无
**************************************************************/
void LED_Init(void)
{

	//吱鹰蕖俟庭窑
	GPIO_InitTypeDef GPIO_TypeStructure;
	//擢谴GPIOD时讚
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	//选毡讒酄?
	GPIO_TypeStructure.GPIO_Pin = GPIO_Pin_15;
	//选毡伽胤模式  通詢螁维摔远
	GPIO_TypeStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	//选毡摔远禺粘虣讏  50MHz
	GPIO_TypeStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//缘始郫GPIOD
	GPIO_Init(GPIOB,&GPIO_TypeStructure);
	//GPIOD8~11 讒酄氉冑熤?
	GPIO_SetBits(GPIOB,GPIO_Pin_15);
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
