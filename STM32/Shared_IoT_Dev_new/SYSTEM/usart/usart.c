#include <stm32f10x.h>
#include "usart.h"
#include "misc.h"
#include "oled_drv.h"
#include "dma.h"
#include "main.h"

u8 USART_RX_BUF[USART1_RxData_length]; 	//����1��������

u8 Rx_num = 0;							//���������м����
u8 USART1_Rx_OK = RESET;				//�������ݱ�־λ
u8 x=0,y=1;

/**************************************************************
*��  �ܣ�����1��ʼ��
*��  ��: baudrate  ������
*����ֵ: ��
**************************************************************/
void USART1_Configure(u32 baudrate)
{
	//����ṹ�����
	GPIO_InitTypeDef GPIO_TypeDefStructure;
	USART_InitTypeDef USART_TypeDefStructure;
	NVIC_InitTypeDef NVIC_TypeDefStructure;
	
	//����GPIOAʱ�� ��������ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA
						   |RCC_APB2Periph_USART1,ENABLE);
	//TX  �����������
	GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_TypeDefStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_TypeDefStructure);
	//RX ��������
	GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_TypeDefStructure);
	
	//������
	USART_TypeDefStructure.USART_BaudRate = baudrate;
	//������ʽ    �����뷢��
	USART_TypeDefStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	//Ӳ��������  ��
	USART_TypeDefStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	//У��λ	  ��
	USART_TypeDefStructure.USART_Parity =USART_Parity_No;
	//ֹͣλ	  1
	USART_TypeDefStructure.USART_StopBits = USART_StopBits_1;
	//���ݳ���  8
	USART_TypeDefStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&USART_TypeDefStructure);
#if USART1_ITRX_ENABLE  //0 ��ѯģʽ 1 �ж�ģʽ
	//�ж����ȼ�����
	NVIC_TypeDefStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_TypeDefStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_TypeDefStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_TypeDefStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_TypeDefStructure);
	//��������1�����ж�
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
#endif
	
	//�򿪴���1
	USART_Cmd(USART1,ENABLE);
	
}

void USART2_Configure(u32 baudrate)
{
	//����ṹ�����
	GPIO_InitTypeDef GPIO_TypeDefStructure;
	USART_InitTypeDef USART_TypeDefStructure;
	NVIC_InitTypeDef NVIC_TypeDefStructure;
	
	//����GPIOAʱ�� ��������ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	//TX  �����������
	GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_TypeDefStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_TypeDefStructure);
	//RX ��������
	GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_3; 
	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_TypeDefStructure);
	
	//������
	USART_TypeDefStructure.USART_BaudRate = baudrate;
	//������ʽ    �����뷢��
	USART_TypeDefStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	//Ӳ��������  ��
	USART_TypeDefStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	//У��λ	  ��
	USART_TypeDefStructure.USART_Parity =USART_Parity_No;
	//ֹͣλ	  1
	USART_TypeDefStructure.USART_StopBits = USART_StopBits_1;
	//���ݳ���  8
	USART_TypeDefStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2,&USART_TypeDefStructure);
#if USART2_ITRX_ENABLE  //0 ��ѯģʽ 1 �ж�ģʽ
	//�ж����ȼ�����
	NVIC_TypeDefStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_TypeDefStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_TypeDefStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_TypeDefStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_TypeDefStructure);
	//��������1�����ж�
//	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);	/* ʹ�ܿ����ж� */
#endif
	
	//�򿪴���2
	USART_Cmd(USART2,ENABLE);
	
	NVIC_TypeDefStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn|DMA1_Channel7_IRQn;       
  NVIC_TypeDefStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_TypeDefStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_TypeDefStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_TypeDefStructure);
	
}
/**************************************************************
*��  �ܣ����ڲ�ѯģʽ
*��  ��: ��
*����ֵ: ��
**************************************************************/
void USART1_Rx_Handler(void)
{
	u8 Res;
	static u8 x=0,y=1;
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET)  //�жϽ��ձ�־
	{
		Res =USART_ReceiveData(USART1);	//��ȡ���յ�������
		if(Res<0x20) Res=0x20;
		OLED_ShowChar_6x8_Pos(x,y,Res);
		x++;
		if(x==21){
			x=0;
			y++;
		}
		USART_SendData(USART1, Res);	//�򴮿�1��������
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���		
     } 
}

void USART2_Rx_Handler(void)
{
	u8 Res;
	if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) != RESET)  //�жϽ��ձ�־
	{
		Res =USART_ReceiveData(USART2);	//��ȡ���յ�������
		if(Res<0x20) Res=0x20;
		OLED_ShowChar_6x8_Pos(x,y,Res);
		x++;
		if(x==21){
			x=0;
			y++;
		}
		USART_SendData(USART2, Res);	//�򴮿�1��������
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//�ȴ����ͽ���		
     } 
}
/**************************************************************
*��  �ܣ������жϷ�����
*��  ��: ��
*����ֵ: ��
**************************************************************/
u16 USART_RX_STA=0;       //����״̬���
void USART1_IRQHandler()
{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)     //�����ж�
	{
		Res =USART_ReceiveData(USART1);	//��ȡ���յ�������
		USART_SendData(USART1, Res);   	//�򴮿�1��������
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	} 
}

void USART2_IRQHandler()
{
//	if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)//注意！不能使用if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)来判断
//  {
//    USART_ReceiveData(USART2);
//  }
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)     //�����ж�
	{
		//����Ӧ���������ճ���,ÿһ�ο����ж�,�����ͨ��DMA_GetCurrDataCounter�õ����ν������ݳ���
		USART_ReceiveData(USART2);
		DMA_Cmd(DMA1_Channel6,DISABLE);/* �رս���DMA  */
		DMA_ClearFlag(DMA1_FLAG_TC6);/* �����־λ */
		/* �������ô������ݳ���, Ҫ����ʵ���ݳ�,��ֹ����*/
		udma.Recv_Len=RECV_BUF_SIZE-DMA_GetCurrDataCounter(DMA1_Channel6);
//		MemClear(udma.Send_Buffer,SEND_BUF_SIZE);
//		MemCopy(udma.Recv_Buffer,udma.Send_Buffer,udma.Recv_Len);
		OLED_Assistant.OLED_ShowBuf_Last_DLen=OLED_Assistant.OLED_ShowBuf_DLen;
		//Copy_To_ShowBuff(udma);
		udma.Recv_Finish_Flag=1;
		OLED_Assistant.OLED_Write_EN=1;//���ղ����������ݲ���ʾ,���ݳ��ȶ�
		if(udma.Recv_Buffer_bFlag==0){
			udma.Recv_Buffer_bFlag=1;
			DMA1_Channel6->CMAR=(u32)udma.Recv_Buffer2;
			
		}
		else{
			udma.Recv_Buffer_bFlag=0;
			DMA1_Channel6->CMAR=(u32)udma.Recv_Buffer;
		}
		
		DMA_SetCurrDataCounter(DMA1_Channel6,RECV_BUF_SIZE);  
		/* ��DMA */
		DMA_Cmd(DMA1_Channel6,ENABLE);
//		DMA_Send_EN=1;
	}
	 
}

/**************************************************************
*��  �ܣ�����1���ֽ�Ϊ��λ����
*��  ��: data ���͵��ֽ�
*����ֵ: ��
**************************************************************/
void USART1_Send_Byte(uint8_t data)
{
	//����1��������
	USART_SendData(USART1,data);
	//�ȴ��������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);

}

/**************************************************************
*��  �ܣ�����1ָ�����ȷ�������
*��  ��: str ���ݵ�ַָ��  length_num ���ݳ���
*����ֵ: ��
**************************************************************/
void USART1_Send_Data_length(uint8_t *str,uint8_t length_num)
{
	uint8_t Tx_num = 0;
	do
	{	
		USART1_Send_Byte(*(str+Tx_num));	//ָ���һ
		Tx_num++;
	}
	while(Tx_num < length_num);				//ָ��С�����ݳ���
}

/**************************************************************
*��  �ܣ�����1�����ַ���
*��  ��: str ���ݵ�ַָ��
*����ֵ: ��
**************************************************************/
void USART1_Send_String(uint8_t *str)
{
	u8 Tx_num = 0;
	do
	{
		USART1_Send_Byte(*(str+Tx_num));	//ָ���һ
		Tx_num++;
	}
	while(*(str+Tx_num) != '\0');			//�жϸ�ָ���Ӧ�ַ��Ƿ�Ϊ������
}


