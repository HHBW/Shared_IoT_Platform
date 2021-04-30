#include "dma.h"
#include "oled_drv.h"
#include "main.h"
#include "usart.h"

u16 DMA1_MEM_LEN=0;//����DMAÿ�����ݴ��͵ĳ��� 	    

u8 DMA_Send_EN=0;
/**************************************************************
*��  �ܣ�����һ��DMA����
*��  ��: DMA_CHx DMAͨ��
*����ֵ: ��
*ע  �ͣ�
				 DMA1�ĸ�ͨ������
				 ����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
				 �Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
				 DMA_CHx:DMAͨ��CHx
				 cpar:�����ַ
				 cmar:�洢����ַ
				 cndtr:���ݴ����� 
**************************************************************/
void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr,uint32_t mode,uint32_t dir)
{
	DMA_InitTypeDef DMA_InitStructure;
	//ʹ��DMA����
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	
	//��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
	DMA_DeInit(DMA_CHx); 
	DMA_Cmd(DMA_CHx, DISABLE);  
	//����
	DMA1_MEM_LEN=cndtr;
	//DMA�������ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;
	//DMA�ڴ����ַ	
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar; 
	//���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
//	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; 
	DMA_InitStructure.DMA_DIR = dir;
	//DMAͨ����DMA����Ĵ�С	
	DMA_InitStructure.DMA_BufferSize = cndtr;  
	//�����ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	//�ڴ��ַ�Ĵ�������	
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  
	//���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  
	//���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 
	//��������������ģʽ
	DMA_InitStructure.DMA_Mode = mode; 
	//DMAͨ�� xӵ�������ȼ�
		if(dir==DMA_DIR_PeripheralSRC){
			DMA_InitStructure.DMA_Priority = DMA_Priority_High;
		}
		else{
			DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
		}			
	//DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��	
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  
	//����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���
	DMA_Init(DMA_CHx, &DMA_InitStructure); 
//	if(dir==DMA_DIR_PeripheralSRC){ //recv mode
//		DMA_ITConfig(DMA_CHx, DMA_IT_TC|DMA_IT_HT|DMA_IT_TE, ENABLE);/* ʹ��DMA�����������������ж� */
//	}
//	else{
//		DMA_ITConfig(DMA_CHx, DMA_IT_TC|DMA_IT_TE, ENABLE);
//	}
	DMA_ITConfig(DMA_CHx, DMA_IT_TC|DMA_IT_TE, ENABLE);//DMAѭ�����ղ����ж�(��֪����ѭ��ģʽ�ܲ��ܽ��ж�
	DMA_Cmd(DMA_CHx, ENABLE);
}  

/**************************************************************
*��  �ܣ�����һ��DMA����
*��  ��: DMA_CHx DMAͨ��
*����ֵ: ��
**************************************************************/
//void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
//{ 
//	//�ر�USART1 TX DMA1 ��ָʾ��ͨ��
//	DMA_Cmd(DMA_CHx, DISABLE );  
//	//DMAͨ����DMA����Ĵ�С	
// 	DMA_SetCurrDataCounter(DMA_CHx,DMA1_MEM_LEN);
//	//ʹ��USART1 TX DMA1 ��ָʾ��ͨ�� 
// 	DMA_Cmd(DMA_CHx, ENABLE);  
//}	 
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u16 MEM_LEN)
{ 
	//�ر�USART1 TX DMA1 ��ָʾ��ͨ��
	DMA_Cmd(DMA_CHx, DISABLE );  
	//DMAͨ����DMA����Ĵ�С	
 	DMA_SetCurrDataCounter(DMA_CHx,MEM_LEN);
	//ʹ��USART1 TX DMA1 ��ָʾ��ͨ�� 
 	DMA_Cmd(DMA_CHx, ENABLE);  
}	

void DMA1_Channel7_IRQHandler(void){  //USART2_TX 
	if(DMA_GetITStatus(DMA1_IT_TC7))//�ж�ͨ��4�������
		{
			DMA_ClearFlag(DMA1_FLAG_TC7);			//���ͨ��4������ɱ�־
			//OLED_ShowChar(50,3,'D');
		}
	
}
void DMA1_Channel6_IRQHandler(void){
	if(DMA_GetITStatus(DMA1_IT_TC6)!= RESET)		//DMA������ɱ�־
	{
		DMA_ClearITPendingBit(DMA1_IT_TC6); 		//����жϱ�־ 
		USART_ClearFlag(USART2,USART_FLAG_TC);		//���USART2��־λ
		DMA_Cmd(DMA1_Channel6, DISABLE );
		udma.Recv_Len=RECV_BUF_SIZE;
		if(udma.Recv_Buffer_bFlag==0){
			udma.Recv_Buffer_bFlag=1;
			DMA1_Channel6->CMAR=(u32)udma.Recv_Buffer2;
			
		}
		else{
			udma.Recv_Buffer_bFlag=0;
			DMA1_Channel6->CMAR=(u32)udma.Recv_Buffer;
		}
		udma.Recv_Finish_Flag=1;
		
		DMA1_Channel6->CNDTR = RECV_BUF_SIZE;	//DMAͨ����DMA����Ĵ�С,���ն�������
		DMA_Cmd(DMA1_Channel6, ENABLE);
	}

}

void DMA1_Start_Send(unsigned char *buf,unsigned char iseof){
	unsigned int len=0;
	len=Size_Of_Str(buf);
	Send_Wrapper(buf,udma.Send_Buffer,len,iseof);
	MYDMA_Enable(DMA1_Channel7,udma.Send_Len);
}



