#include "time_base.h"
#include "led.h"
#include "ds18b20.h"
#include "main.h"

unsigned int Time_Counter=0;
unsigned char Flag_1ms=0;
unsigned char Flag_5ms=0;
unsigned char Flag_10ms=0;
unsigned char Flag_20ms=0;
unsigned char Flag_50ms=0;
unsigned char Flag_100ms=0;
unsigned char Flag_200ms=0;
unsigned char Flag_500ms=0;
unsigned char Flag_1s=0;

void TIM6_Int_Init(uint16_t arr,uint16_t psc) //TIM6_Int_Init(10,7200-1);  7200/72M=0.1ms   interrupt while arr=10 and then Time_Conter++(0.1ms*10=1ms)
{
	/*���� �ṹ�����*/
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/*ʱ��ʹ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);   
	/*��ʱ��TIM3��ʼ��*/
	TIM_TimeBaseStructure.TIM_Period = arr;						//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 			    	//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 	//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);				//����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	/*ʹ��ָ����TIM3�ж�,��������ж�*/
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE); 				
	/*�ж����ȼ�NVIC����*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;					//TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;		//��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;				//�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure); 										//��ʼ��NVIC�Ĵ���
	/*ʹ��TIMx*/
	TIM_Cmd(TIM6, ENABLE); 
	TIM_ClearFlag(TIM6, TIM_FLAG_Update); 	
}

void TIM6_IRQHandler(void)            		//TIM3�ж�
{
	
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		Time_Counter++;
		if(DS18B20_SOC&&(sensor_wait==0)){
				DS18B20_Get_Temp();
		}
		Flag_1ms=1;
		if(Time_Counter%5==0) Flag_5ms=1;
		if(Time_Counter%10==0) Flag_10ms=1;
		if(Time_Counter%20==0) Flag_20ms=1;
		if(Time_Counter%50==0) Flag_50ms=1;
		if(Time_Counter%100==0) Flag_100ms=1;
		
		if(Time_Counter%200==0) Flag_200ms=1;
		if(Time_Counter%500==0) {
			Flag_500ms=1;
			LED2=1;
		}
		if(Time_Counter%1000==0) {
			Flag_1s=1;
			Time_Counter=0;
			LED2=0;
		}

	}
	TIM_ClearITPendingBit(TIM6, TIM_IT_Update);       //���TIMx�����жϱ�־ 
}
