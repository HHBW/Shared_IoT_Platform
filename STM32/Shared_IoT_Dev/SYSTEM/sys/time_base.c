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
	/*定义 结构体变量*/
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/*时钟使能*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);   
	/*定时器TIM3初始化*/
	TIM_TimeBaseStructure.TIM_Period = arr;						//设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 			    	//设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 	//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM向上计数模式
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);				//根据指定的参数初始化TIMx的时间基数单位
	/*使能指定的TIM3中断,允许更新中断*/
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE); 				
	/*中断优先级NVIC设置*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;					//TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;		//先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;				//从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure); 										//初始化NVIC寄存器
	/*使能TIMx*/
	TIM_Cmd(TIM6, ENABLE); 
	TIM_ClearFlag(TIM6, TIM_FLAG_Update); 	
}

void TIM6_IRQHandler(void)            		//TIM3中断
{
	
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
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
	TIM_ClearITPendingBit(TIM6, TIM_IT_Update);       //清除TIMx更新中断标志 
}
