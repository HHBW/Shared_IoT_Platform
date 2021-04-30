#include "dma.h"
#include "oled_drv.h"
#include "main.h"
#include "usart.h"

u16 DMA1_MEM_LEN=0;//保存DMA每次数据传送的长度 	    

u8 DMA_Send_EN=0;
/**************************************************************
*功  能：开启一次DMA传输
*参  数: DMA_CHx DMA通道
*返回值: 无
*注  释：
				 DMA1的各通道配置
				 这里的传输形式是固定的,这点要根据不同的情况来修改
				 从存储器->外设模式/8位数据宽度/存储器增量模式
				 DMA_CHx:DMA通道CHx
				 cpar:外设地址
				 cmar:存储器地址
				 cndtr:数据传输量 
**************************************************************/
void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr,uint32_t mode,uint32_t dir)
{
	DMA_InitTypeDef DMA_InitStructure;
	//使能DMA传输
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	
	//将DMA的通道1寄存器重设为缺省值
	DMA_DeInit(DMA_CHx); 
	DMA_Cmd(DMA_CHx, DISABLE);  
	//长度
	DMA1_MEM_LEN=cndtr;
	//DMA外设基地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;
	//DMA内存基地址	
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar; 
	//数据传输方向，从内存读取发送到外设
//	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; 
	DMA_InitStructure.DMA_DIR = dir;
	//DMA通道的DMA缓存的大小	
	DMA_InitStructure.DMA_BufferSize = cndtr;  
	//外设地址寄存器不变
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	//内存地址寄存器递增	
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  
	//数据宽度为8位
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  
	//数据宽度为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 
	//工作在正常缓存模式
	DMA_InitStructure.DMA_Mode = mode; 
	//DMA通道 x拥有中优先级
		if(dir==DMA_DIR_PeripheralSRC){
			DMA_InitStructure.DMA_Priority = DMA_Priority_High;
		}
		else{
			DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
		}			
	//DMA通道x没有设置为内存到内存传输	
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  
	//根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
	DMA_Init(DMA_CHx, &DMA_InitStructure); 
//	if(dir==DMA_DIR_PeripheralSRC){ //recv mode
//		DMA_ITConfig(DMA_CHx, DMA_IT_TC|DMA_IT_HT|DMA_IT_TE, ENABLE);/* 使能DMA半满、溢满、错误中断 */
//	}
//	else{
//		DMA_ITConfig(DMA_CHx, DMA_IT_TC|DMA_IT_TE, ENABLE);
//	}
	DMA_ITConfig(DMA_CHx, DMA_IT_TC|DMA_IT_TE, ENABLE);//DMA循环接收不进中断(不知道不循环模式能不能进中断
	DMA_Cmd(DMA_CHx, ENABLE);
}  

/**************************************************************
*功  能：开启一次DMA传输
*参  数: DMA_CHx DMA通道
*返回值: 无
**************************************************************/
//void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
//{ 
//	//关闭USART1 TX DMA1 所指示的通道
//	DMA_Cmd(DMA_CHx, DISABLE );  
//	//DMA通道的DMA缓存的大小	
// 	DMA_SetCurrDataCounter(DMA_CHx,DMA1_MEM_LEN);
//	//使能USART1 TX DMA1 所指示的通道 
// 	DMA_Cmd(DMA_CHx, ENABLE);  
//}	 
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u16 MEM_LEN)
{ 
	//关闭USART1 TX DMA1 所指示的通道
	DMA_Cmd(DMA_CHx, DISABLE );  
	//DMA通道的DMA缓存的大小	
 	DMA_SetCurrDataCounter(DMA_CHx,MEM_LEN);
	//使能USART1 TX DMA1 所指示的通道 
 	DMA_Cmd(DMA_CHx, ENABLE);  
}	

void DMA1_Channel7_IRQHandler(void){  //USART2_TX 
	if(DMA_GetITStatus(DMA1_IT_TC7))//判断通道4传输完成
		{
			DMA_ClearFlag(DMA1_FLAG_TC7);			//清除通道4传输完成标志
			//OLED_ShowChar(50,3,'D');
		}
	
}
void DMA1_Channel6_IRQHandler(void){
	if(DMA_GetITStatus(DMA1_IT_TC6)!= RESET)		//DMA接收完成标志
	{
		DMA_ClearITPendingBit(DMA1_IT_TC6); 		//清除中断标志 
		USART_ClearFlag(USART2,USART_FLAG_TC);		//清除USART2标志位
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
		
		DMA1_Channel6->CNDTR = RECV_BUF_SIZE;	//DMA通道的DMA缓存的大小,接收定长数据
		DMA_Cmd(DMA1_Channel6, ENABLE);
	}

}

void DMA1_Start_Send(unsigned char *buf,unsigned char iseof){
	unsigned int len=0;
	len=Size_Of_Str(buf);
	Send_Wrapper(buf,udma.Send_Buffer,len,iseof);
	MYDMA_Enable(DMA1_Channel7,udma.Send_Len);
}



