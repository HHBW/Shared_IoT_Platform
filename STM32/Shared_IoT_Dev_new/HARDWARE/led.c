#include <stm32f10x.h>
#include "delay.h"

/**************************************************************
*��  �ܣ����Ŷ˿�����
*��  ��: ��
*����ֵ: ��
**************************************************************/
void LED_Init(void)
{

	//֨ӥޡٹͥҤ
	GPIO_InitTypeDef GPIO_TypeStructure;
	//ߪǴGPIODʱד
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	//ѡձ׋ࠚ
	GPIO_TypeStructure.GPIO_Pin = GPIO_Pin_15;
	//ѡձ٤طģʽ  ͨԃΆάˤԶ
	GPIO_TypeStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	//ѡձˤԶخճ̙׈  50MHz
	GPIO_TypeStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//ԵʼۯGPIOD
	GPIO_Init(GPIOB,&GPIO_TypeStructure);
	//GPIOD8~11 ׋ࠚ׃ٟ֧ƽ
	GPIO_SetBits(GPIOB,GPIO_Pin_15);
}


/**************************************************************
*��  �ܣ�ʵ���ĸ�LED�Ƶ���ˮЧ��
*��  ��: ��
*����ֵ: ��
**************************************************************/
void lightwater(void)
{
	uint8_t  i = 0;
	uint16_t temp_Pin = GPIO_Pin_8;	     //GPIO_Pin_8 = 0000 0001 0000 0000
	for(i = 0;i < 4;i++)
	{
		GPIO_ResetBits(GPIOD,temp_Pin);  //GPIOD8~11�˿�����͵�ƽ LED��
		Delay_ms(500);                   //��ʱ500ms
		GPIO_SetBits(GPIOD,temp_Pin);    //GPIOD8~11�˿�����ߵ�ƽ LED��
		temp_Pin <<= 1;					 //����һλ
	}
}
