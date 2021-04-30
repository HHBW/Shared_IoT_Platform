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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	//ѡ��˿�
	GPIO_TypeStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8
								  |GPIO_Pin_9|GPIO_Pin_10;
	//ѡ����ģʽ ��������
	GPIO_TypeStructure.GPIO_Mode = GPIO_Mode_IPU;
	//��ʼ��GPIOB
	GPIO_Init(GPIOE,&GPIO_TypeStructure);
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
