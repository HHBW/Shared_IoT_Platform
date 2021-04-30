#include <stm32f10x.h>
#include "delay.h"

/**************************************************************
*��  �ܣ����Ŷ˿�����
*��  ��: ��
*����ֵ: ��
**************************************************************/
void LED_Init(void)
{

	//����ṹ�����
	GPIO_InitTypeDef GPIO_TypeStructure;
	//����GPIODʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	//ѡ��˿�
	GPIO_TypeStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9
								  |GPIO_Pin_10|GPIO_Pin_11;
	//ѡ����ģʽ  ͨ���������
	GPIO_TypeStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	//ѡ���������ٶ�  50MHz
	GPIO_TypeStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//��ʼ��GPIOD
	GPIO_Init(GPIOD,&GPIO_TypeStructure);
	//GPIOD8~11 �˿��øߵ�ƽ
	GPIO_SetBits(GPIOD,GPIO_Pin_8|GPIO_Pin_9
				       |GPIO_Pin_10|GPIO_Pin_11);
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
