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

void TIM3_Int_Init(uint16_t arr,uint16_t psc) //TIM6_Int_Init(10,7200-1);  7200/72M=0.1ms   interrupt while arr=10 and then Time_Conter++(0.1ms*10=1ms)
{
	/*֨ӥ ޡٹͥҤ*/
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/*ʱדʹŜ*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);   
	/*֨ʱǷTIM3Եʼۯ*/
	TIM_TimeBaseStructure.TIM_Period = arr;						//ʨ׃՚Ђһٶټт˂ݾװɫܮ֯քؔ֯טװ՘݄զǷלǚքֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 			    	//ʨ׃ԃ4طΪTIMxʱדƵÊԽ˽քԤؖƵֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 	//ʨ׃ʱדؖٮ:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIMвʏ݆˽ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);				//ٹߝָ֨քӎ˽ԵʼۯTIMxքʱݤܹ˽եλ
	/*ʹŜָ֨քTIM3א׏,Պѭټтא׏*/
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); 				
	/*א׏ԅЈܶNVICʨ׃*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;					//TIM3א׏
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;		//ЈռԅЈܶ0ܶ
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;				//ՓԅЈܶ3ܶ
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQͨրѻʹŜ
	NVIC_Init(&NVIC_InitStructure); 										//ԵʼۯNVIC݄զǷ
	/*ʹŜTIMx*/
	TIM_Cmd(TIM3, ENABLE); 
	TIM_ClearFlag(TIM3, TIM_FLAG_Update); 	
}

void TIM3_IRQHandler(void)            		//TIM3א׏
{
	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //ݬөTIM3ټтא׏עʺԫر
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
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);       //ȥԽTIMxټтא׏Ҫ־ 
}
