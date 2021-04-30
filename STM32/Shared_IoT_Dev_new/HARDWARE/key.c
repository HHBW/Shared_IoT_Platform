#include <stm32f10x.h>
#include "key.h"
#include "delay.h"


/**************************************************************
*��  �ܣ��������Ŷ˿�����
*��  ��: ��
*����ֵ: ��
**************************************************************/
void Key_Init(void)
{
	//����ṹ�����
	GPIO_InitTypeDef GPIO_TypeStructure;
	//����GPIOBʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);
	//ѡ��˿�
	GPIO_TypeStructure.GPIO_Pin = KEY1_PIN|KEY2_PIN;
	//ѡ����ģʽ ��������
	GPIO_TypeStructure.GPIO_Mode = GPIO_Mode_IPU;
	//��ʼ��GPIOB
	GPIO_Init(KEY_PORT2,&GPIO_TypeStructure);
	
	GPIO_TypeStructure.GPIO_Pin = KEY3_PIN|KEY4_PIN;
	GPIO_Init(KEY_PORT1,&GPIO_TypeStructure);
}

/**************************************************************
*��  �ܣ��������� LED��Ӧ
*��  ��: ��
*����ֵ: 0 1 2 3 4 
**************************************************************/
uint8_t KEY_Scan(void)
{
	if(KEY1 == 0)		                    //����1���
	{
		//Delay_ms(5);						//��������
		if(KEY1 == 0)						//�����ٴμ��
		{
			while(KEY1 == 0);				//���ּ��
			return 1;						//����ֵ
		}
	}
	if(KEY2 == 0)							 
	{
		//Delay_ms(5);						 
		if(KEY2 == 0)
		{
			while(KEY2 == 0);				//���ּ��
			return 2;
		}
	}
	if(KEY3 == 0)							
	{
		//Delay_ms(5);						 
		if(KEY3 == 0)
		{
			while(KEY3 == 0);				//���ּ��
			return 3;
		}
	}
	if(KEY4 == 0)							 
	{
		//Delay_ms(5);
		if(KEY4 == 0)
		{
			while(KEY4 == 0);				//���ּ��
			return 4;
		}
	}
	return 0;								//�ް������� ����0
}
