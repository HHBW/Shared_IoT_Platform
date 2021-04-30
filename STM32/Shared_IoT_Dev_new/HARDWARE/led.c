#include <stm32f10x.h>
#include "delay.h"

/**************************************************************
*¹¦  ÄÜ£ºÒý½Å¶Ë¿ÚÅäÖÃ
*²Î  Êý: ÎÞ
*·µ»ØÖµ: ÎÞ
**************************************************************/
void LED_Init(void)
{

	//Ö¨Ó¥Þ¡Ù¹Í¥Ò¤
	GPIO_InitTypeDef GPIO_TypeStructure;
	//ßªÇ´GPIODÊ±×“
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	//Ñ¡Õ±×‹à š
	GPIO_TypeStructure.GPIO_Pin = GPIO_Pin_15;
	//Ñ¡Õ±Ù¤Ø·Ä£Ê½  Í¨ÔƒÎ†Î¬Ë¤Ô¶
	GPIO_TypeStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	//Ñ¡Õ±Ë¤Ô¶Ø®Õ³Ì™×ˆ  50MHz
	GPIO_TypeStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//ÔµÊ¼Û¯GPIOD
	GPIO_Init(GPIOB,&GPIO_TypeStructure);
	//GPIOD8~11 ×‹à š×ƒÙŸÖ§Æ½
	GPIO_SetBits(GPIOB,GPIO_Pin_15);
}


/**************************************************************
*¹¦  ÄÜ£ºÊµÏÖËÄ¸öLEDµÆµÄÁ÷Ë®Ð§¹û
*²Î  Êý: ÎÞ
*·µ»ØÖµ: ÎÞ
**************************************************************/
void lightwater(void)
{
	uint8_t  i = 0;
	uint16_t temp_Pin = GPIO_Pin_8;	     //GPIO_Pin_8 = 0000 0001 0000 0000
	for(i = 0;i < 4;i++)
	{
		GPIO_ResetBits(GPIOD,temp_Pin);  //GPIOD8~11¶Ë¿ÚÊä³öµÍµçÆ½ LEDÁÁ
		Delay_ms(500);                   //ÑÓÊ±500ms
		GPIO_SetBits(GPIOD,temp_Pin);    //GPIOD8~11¶Ë¿ÚÊä³ö¸ßµçÆ½ LEDÃð
		temp_Pin <<= 1;					 //×óÒÆÒ»Î»
	}
}
