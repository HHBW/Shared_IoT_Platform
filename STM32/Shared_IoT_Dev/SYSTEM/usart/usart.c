#include <stm32f10x.h>
#include "usart.h"
#include "misc.h"
#include "oled_drv.h"
#include "dma.h"
#include "main.h"

u8 USART_RX_BUF[USART1_RxData_length]; 	//串口1接收数组

u8 Rx_num = 0;							//发送数据中间变量
u8 USART1_Rx_OK = RESET;				//发送数据标志位
u8 x=0,y=1;

/**************************************************************
*功  能：串口1初始化
*参  数: baudrate  波特率
*返回值: 无
**************************************************************/
void USART1_Configure(u32 baudrate)
{
	//定义结构体变量
	GPIO_InitTypeDef GPIO_TypeDefStructure;
	USART_InitTypeDef USART_TypeDefStructure;
	NVIC_InitTypeDef NVIC_TypeDefStructure;
	
	//开启GPIOA时钟 开启串口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA
						   |RCC_APB2Periph_USART1,ENABLE);
	//TX  复用推挽输出
	GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_TypeDefStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_TypeDefStructure);
	//RX 浮空输入
	GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_TypeDefStructure);
	
	//波特率
	USART_TypeDefStructure.USART_BaudRate = baudrate;
	//工作方式    接收与发送
	USART_TypeDefStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	//硬件流控制  无
	USART_TypeDefStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	//校验位	  无
	USART_TypeDefStructure.USART_Parity =USART_Parity_No;
	//停止位	  1
	USART_TypeDefStructure.USART_StopBits = USART_StopBits_1;
	//数据长度  8
	USART_TypeDefStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&USART_TypeDefStructure);
#if USART1_ITRX_ENABLE  //0 查询模式 1 中断模式
	//中断优先级配置
	NVIC_TypeDefStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_TypeDefStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_TypeDefStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_TypeDefStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_TypeDefStructure);
	//开启串口1接收中断
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
#endif
	
	//打开串口1
	USART_Cmd(USART1,ENABLE);
	
}

void USART2_Configure(u32 baudrate)
{
	//定义结构体变量
	GPIO_InitTypeDef GPIO_TypeDefStructure;
	USART_InitTypeDef USART_TypeDefStructure;
	NVIC_InitTypeDef NVIC_TypeDefStructure;
	
	//开启GPIOA时钟 开启串口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	//TX  复用推挽输出
	GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_TypeDefStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_TypeDefStructure);
	//RX 浮空输入
	GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_3; 
	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_TypeDefStructure);
	
	//波特率
	USART_TypeDefStructure.USART_BaudRate = baudrate;
	//工作方式    接收与发送
	USART_TypeDefStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	//硬件流控制  无
	USART_TypeDefStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	//校验位	  无
	USART_TypeDefStructure.USART_Parity =USART_Parity_No;
	//停止位	  1
	USART_TypeDefStructure.USART_StopBits = USART_StopBits_1;
	//数据长度  8
	USART_TypeDefStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2,&USART_TypeDefStructure);
#if USART2_ITRX_ENABLE  //0 查询模式 1 中断模式
	//中断优先级配置
	NVIC_TypeDefStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_TypeDefStructure.NVIC_IRQChannelPreemptionPriority = 8;
	NVIC_TypeDefStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_TypeDefStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_TypeDefStructure);
	//开启串口1接收中断
//	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);	/* 使能空闲中断 */
#endif
	
	//打开串口2
	USART_Cmd(USART2,ENABLE);
	
	NVIC_TypeDefStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn|DMA1_Channel7_IRQn;       
  NVIC_TypeDefStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_TypeDefStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_TypeDefStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_TypeDefStructure);
	
}
/**************************************************************
*功  能：串口查询模式
*参  数: 无
*返回值: 无
**************************************************************/
void USART1_Rx_Handler(void)
{
	u8 Res;
	static u8 x=0,y=1;
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET)  //判断接收标志
	{
		Res =USART_ReceiveData(USART1);	//读取接收到的数据
		if(Res<0x20) Res=0x20;
		OLED_ShowChar_6x8_Pos(x,y,Res);
		x++;
		if(x==21){
			x=0;
			y++;
		}
		USART_SendData(USART1, Res);	//向串口1发送数据
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束		
     } 
}

void USART2_Rx_Handler(void)
{
	u8 Res;
	if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) != RESET)  //判断接收标志
	{
		Res =USART_ReceiveData(USART2);	//读取接收到的数据
		if(Res<0x20) Res=0x20;
		OLED_ShowChar_6x8_Pos(x,y,Res);
		x++;
		if(x==21){
			x=0;
			y++;
		}
		USART_SendData(USART2, Res);	//向串口1发送数据
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束		
     } 
}
/**************************************************************
*功  能：串口中断服务函数
*参  数: 无
*返回值: 无
**************************************************************/
u16 USART_RX_STA=0;       //接收状态标记
void USART1_IRQHandler()
{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)     //接收中断
	{
		Res =USART_ReceiveData(USART1);	//读取接收到的数据
		USART_SendData(USART1, Res);   	//向串口1发送数据
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
	} 
}

void USART2_IRQHandler()
{
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)     //接收中断
	{
		//根据应用来定接收长度,每一次空闲中断,则可以通过DMA_GetCurrDataCounter拿到本次接收数据长度
		USART_ReceiveData(USART2);
		DMA_Cmd(DMA1_Channel6,DISABLE);/* 关闭接收DMA  */
		DMA_ClearFlag(DMA1_FLAG_TC6);/* 清除标志位 */
		/* 重新设置传输数据长度, 要比真实数据长,防止覆盖*/
		udma.Recv_Len=RECV_BUF_SIZE-DMA_GetCurrDataCounter(DMA1_Channel6);
//		MemClear(udma.Send_Buffer,SEND_BUF_SIZE);
//		MemCopy(udma.Recv_Buffer,udma.Send_Buffer,udma.Recv_Len);
		OLED_Assistant.OLED_ShowBuf_Last_DLen=OLED_Assistant.OLED_ShowBuf_DLen;
		//Copy_To_ShowBuff(udma);
		udma.Recv_Finish_Flag=1;
		OLED_Assistant.OLED_Write_EN=1;//接收不定长的数据才显示,数据长度短
		if(udma.Recv_Buffer_bFlag==0){
			udma.Recv_Buffer_bFlag=1;
			DMA1_Channel6->CMAR=(u32)udma.Recv_Buffer2;
			
		}
		else{
			udma.Recv_Buffer_bFlag=0;
			DMA1_Channel6->CMAR=(u32)udma.Recv_Buffer;
		}
		
		DMA_SetCurrDataCounter(DMA1_Channel6,RECV_BUF_SIZE);  
		/* 打开DMA */
		DMA_Cmd(DMA1_Channel6,ENABLE);
//		DMA_Send_EN=1;
	}
	 
}

/**************************************************************
*功  能：串口1以字节为单位发送
*参  数: data 发送的字节
*返回值: 无
**************************************************************/
void USART1_Send_Byte(uint8_t data)
{
	//串口1发送数据
	USART_SendData(USART1,data);
	//等待发送完成
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);

}

/**************************************************************
*功  能：串口1指定长度发送数据
*参  数: str 数据地址指针  length_num 数据长度
*返回值: 无
**************************************************************/
void USART1_Send_Data_length(uint8_t *str,uint8_t length_num)
{
	uint8_t Tx_num = 0;
	do
	{	
		USART1_Send_Byte(*(str+Tx_num));	//指针加一
		Tx_num++;
	}
	while(Tx_num < length_num);				//指针小于数据长度
}

/**************************************************************
*功  能：串口1发送字符串
*参  数: str 数据地址指针
*返回值: 无
**************************************************************/
void USART1_Send_String(uint8_t *str)
{
	u8 Tx_num = 0;
	do
	{
		USART1_Send_Byte(*(str+Tx_num));	//指针加一
		Tx_num++;
	}
	while(*(str+Tx_num) != '\0');			//判断该指针对应字符是否为结束符
}


