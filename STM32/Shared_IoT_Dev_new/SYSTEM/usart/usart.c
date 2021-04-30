#include <stm32f10x.h>
#include "usart.h"
#include "misc.h"
#include "oled_drv.h"
#include "dma.h"
#include "main.h"

u8 USART_RX_BUF[USART1_RxData_length]; 	//´®¿Ú1½ÓÊÕÊý×é

u8 Rx_num = 0;							//·¢ËÍÊý¾ÝÖÐ¼ä±äÁ¿
u8 USART1_Rx_OK = RESET;				//·¢ËÍÊý¾Ý±êÖ¾Î»
u8 x=0,y=1;

/**************************************************************
*¹¦  ÄÜ£º´®¿Ú1³õÊ¼»¯
*²Î  Êý: baudrate  ²¨ÌØÂÊ
*·µ»ØÖµ: ÎÞ
**************************************************************/
void USART1_Configure(u32 baudrate)
{
	//¶¨Òå½á¹¹Ìå±äÁ¿
	GPIO_InitTypeDef GPIO_TypeDefStructure;
	USART_InitTypeDef USART_TypeDefStructure;
	NVIC_InitTypeDef NVIC_TypeDefStructure;
	
	//¿ªÆôGPIOAÊ±ÖÓ ¿ªÆô´®¿ÚÊ±ÖÓ
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA
						   |RCC_APB2Periph_USART1,ENABLE);
	//TX  ¸´ÓÃÍÆÍìÊä³ö
	GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_TypeDefStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_TypeDefStructure);
	//RX ¸¡¿ÕÊäÈë
	GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_TypeDefStructure);
	
	//²¨ÌØÂÊ
	USART_TypeDefStructure.USART_BaudRate = baudrate;
	//¹¤×÷·½Ê½    ½ÓÊÕÓë·¢ËÍ
	USART_TypeDefStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	//Ó²¼þÁ÷¿ØÖÆ  ÎÞ
	USART_TypeDefStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	//Ð£ÑéÎ»	  ÎÞ
	USART_TypeDefStructure.USART_Parity =USART_Parity_No;
	//Í£Ö¹Î»	  1
	USART_TypeDefStructure.USART_StopBits = USART_StopBits_1;
	//Êý¾Ý³¤¶È  8
	USART_TypeDefStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&USART_TypeDefStructure);
#if USART1_ITRX_ENABLE  //0 ²éÑ¯Ä£Ê½ 1 ÖÐ¶ÏÄ£Ê½
	//ÖÐ¶ÏÓÅÏÈ¼¶ÅäÖÃ
	NVIC_TypeDefStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_TypeDefStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_TypeDefStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_TypeDefStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_TypeDefStructure);
	//¿ªÆô´®¿Ú1½ÓÊÕÖÐ¶Ï
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
#endif
	
	//´ò¿ª´®¿Ú1
	USART_Cmd(USART1,ENABLE);
	
}

void USART2_Configure(u32 baudrate)
{
	//¶¨Òå½á¹¹Ìå±äÁ¿
	GPIO_InitTypeDef GPIO_TypeDefStructure;
	USART_InitTypeDef USART_TypeDefStructure;
	NVIC_InitTypeDef NVIC_TypeDefStructure;
	
	//¿ªÆôGPIOAÊ±ÖÓ ¿ªÆô´®¿ÚÊ±ÖÓ
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	//TX  ¸´ÓÃÍÆÍìÊä³ö
	GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_TypeDefStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_TypeDefStructure);
	//RX ¸¡¿ÕÊäÈë
	GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_3; 
	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_TypeDefStructure);
	
	//²¨ÌØÂÊ
	USART_TypeDefStructure.USART_BaudRate = baudrate;
	//¹¤×÷·½Ê½    ½ÓÊÕÓë·¢ËÍ
	USART_TypeDefStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	//Ó²¼þÁ÷¿ØÖÆ  ÎÞ
	USART_TypeDefStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	//Ð£ÑéÎ»	  ÎÞ
	USART_TypeDefStructure.USART_Parity =USART_Parity_No;
	//Í£Ö¹Î»	  1
	USART_TypeDefStructure.USART_StopBits = USART_StopBits_1;
	//Êý¾Ý³¤¶È  8
	USART_TypeDefStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2,&USART_TypeDefStructure);
#if USART2_ITRX_ENABLE  //0 ²éÑ¯Ä£Ê½ 1 ÖÐ¶ÏÄ£Ê½
	//ÖÐ¶ÏÓÅÏÈ¼¶ÅäÖÃ
	NVIC_TypeDefStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_TypeDefStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_TypeDefStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_TypeDefStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_TypeDefStructure);
	//¿ªÆô´®¿Ú1½ÓÊÕÖÐ¶Ï
//	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);	/* Ê¹ÄÜ¿ÕÏÐÖÐ¶Ï */
#endif
	
	//´ò¿ª´®¿Ú2
	USART_Cmd(USART2,ENABLE);
	
	NVIC_TypeDefStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn|DMA1_Channel7_IRQn;       
  NVIC_TypeDefStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_TypeDefStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_TypeDefStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_TypeDefStructure);
	
}
/**************************************************************
*¹¦  ÄÜ£º´®¿Ú²éÑ¯Ä£Ê½
*²Î  Êý: ÎÞ
*·µ»ØÖµ: ÎÞ
**************************************************************/
void USART1_Rx_Handler(void)
{
	u8 Res;
	static u8 x=0,y=1;
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET)  //ÅÐ¶Ï½ÓÊÕ±êÖ¾
	{
		Res =USART_ReceiveData(USART1);	//¶ÁÈ¡½ÓÊÕµ½µÄÊý¾Ý
		if(Res<0x20) Res=0x20;
		OLED_ShowChar_6x8_Pos(x,y,Res);
		x++;
		if(x==21){
			x=0;
			y++;
		}
		USART_SendData(USART1, Res);	//Ïò´®¿Ú1·¢ËÍÊý¾Ý
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//µÈ´ý·¢ËÍ½áÊø		
     } 
}

void USART2_Rx_Handler(void)
{
	u8 Res;
	if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) != RESET)  //ÅÐ¶Ï½ÓÊÕ±êÖ¾
	{
		Res =USART_ReceiveData(USART2);	//¶ÁÈ¡½ÓÊÕµ½µÄÊý¾Ý
		if(Res<0x20) Res=0x20;
		OLED_ShowChar_6x8_Pos(x,y,Res);
		x++;
		if(x==21){
			x=0;
			y++;
		}
		USART_SendData(USART2, Res);	//Ïò´®¿Ú1·¢ËÍÊý¾Ý
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//µÈ´ý·¢ËÍ½áÊø		
     } 
}
/**************************************************************
*¹¦  ÄÜ£º´®¿ÚÖÐ¶Ï·þÎñº¯Êý
*²Î  Êý: ÎÞ
*·µ»ØÖµ: ÎÞ
**************************************************************/
u16 USART_RX_STA=0;       //½ÓÊÕ×´Ì¬±ê¼Ç
void USART1_IRQHandler()
{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)     //½ÓÊÕÖÐ¶Ï
	{
		Res =USART_ReceiveData(USART1);	//¶ÁÈ¡½ÓÊÕµ½µÄÊý¾Ý
		USART_SendData(USART1, Res);   	//Ïò´®¿Ú1·¢ËÍÊý¾Ý
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//µÈ´ý·¢ËÍ½áÊø
	} 
}

void USART2_IRQHandler()
{
//	if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)//æ³¨æ„ï¼ä¸èƒ½ä½¿ç”¨if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)æ¥åˆ¤æ–­
//  {
//    USART_ReceiveData(USART2);
//  }
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)     //½ÓÊÕÖÐ¶Ï
	{
		//¸ù¾ÝÓ¦ÓÃÀ´¶¨½ÓÊÕ³¤¶È,Ã¿Ò»´Î¿ÕÏÐÖÐ¶Ï,Ôò¿ÉÒÔÍ¨¹ýDMA_GetCurrDataCounterÄÃµ½±¾´Î½ÓÊÕÊý¾Ý³¤¶È
		USART_ReceiveData(USART2);
		DMA_Cmd(DMA1_Channel6,DISABLE);/* ¹Ø±Õ½ÓÊÕDMA  */
		DMA_ClearFlag(DMA1_FLAG_TC6);/* Çå³ý±êÖ¾Î» */
		/* ÖØÐÂÉèÖÃ´«ÊäÊý¾Ý³¤¶È, Òª±ÈÕæÊµÊý¾Ý³¤,·ÀÖ¹¸²¸Ç*/
		udma.Recv_Len=RECV_BUF_SIZE-DMA_GetCurrDataCounter(DMA1_Channel6);
//		MemClear(udma.Send_Buffer,SEND_BUF_SIZE);
//		MemCopy(udma.Recv_Buffer,udma.Send_Buffer,udma.Recv_Len);
		OLED_Assistant.OLED_ShowBuf_Last_DLen=OLED_Assistant.OLED_ShowBuf_DLen;
		//Copy_To_ShowBuff(udma);
		udma.Recv_Finish_Flag=1;
		OLED_Assistant.OLED_Write_EN=1;//½ÓÊÕ²»¶¨³¤µÄÊý¾Ý²ÅÏÔÊ¾,Êý¾Ý³¤¶È¶Ì
		if(udma.Recv_Buffer_bFlag==0){
			udma.Recv_Buffer_bFlag=1;
			DMA1_Channel6->CMAR=(u32)udma.Recv_Buffer2;
			
		}
		else{
			udma.Recv_Buffer_bFlag=0;
			DMA1_Channel6->CMAR=(u32)udma.Recv_Buffer;
		}
		
		DMA_SetCurrDataCounter(DMA1_Channel6,RECV_BUF_SIZE);  
		/* ´ò¿ªDMA */
		DMA_Cmd(DMA1_Channel6,ENABLE);
//		DMA_Send_EN=1;
	}
	 
}

/**************************************************************
*¹¦  ÄÜ£º´®¿Ú1ÒÔ×Ö½ÚÎªµ¥Î»·¢ËÍ
*²Î  Êý: data ·¢ËÍµÄ×Ö½Ú
*·µ»ØÖµ: ÎÞ
**************************************************************/
void USART1_Send_Byte(uint8_t data)
{
	//´®¿Ú1·¢ËÍÊý¾Ý
	USART_SendData(USART1,data);
	//µÈ´ý·¢ËÍÍê³É
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);

}

/**************************************************************
*¹¦  ÄÜ£º´®¿Ú1Ö¸¶¨³¤¶È·¢ËÍÊý¾Ý
*²Î  Êý: str Êý¾ÝµØÖ·Ö¸Õë  length_num Êý¾Ý³¤¶È
*·µ»ØÖµ: ÎÞ
**************************************************************/
void USART1_Send_Data_length(uint8_t *str,uint8_t length_num)
{
	uint8_t Tx_num = 0;
	do
	{	
		USART1_Send_Byte(*(str+Tx_num));	//Ö¸Õë¼ÓÒ»
		Tx_num++;
	}
	while(Tx_num < length_num);				//Ö¸ÕëÐ¡ÓÚÊý¾Ý³¤¶È
}

/**************************************************************
*¹¦  ÄÜ£º´®¿Ú1·¢ËÍ×Ö·û´®
*²Î  Êý: str Êý¾ÝµØÖ·Ö¸Õë
*·µ»ØÖµ: ÎÞ
**************************************************************/
void USART1_Send_String(uint8_t *str)
{
	u8 Tx_num = 0;
	do
	{
		USART1_Send_Byte(*(str+Tx_num));	//Ö¸Õë¼ÓÒ»
		Tx_num++;
	}
	while(*(str+Tx_num) != '\0');			//ÅÐ¶Ï¸ÃÖ¸Õë¶ÔÓ¦×Ö·ûÊÇ·ñÎª½áÊø·û
}


