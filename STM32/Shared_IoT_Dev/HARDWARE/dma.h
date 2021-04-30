#ifndef __DMA_H
#define __DMA_H
#include "stm32f10x.h"


extern void MYDMA_Config(DMA_Channel_TypeDef*DMA_CHx,uint32_t cpar,uint32_t cmar,uint16_t cndtr,uint32_t mode,uint32_t dir);//≈‰÷√DMA1_CHx

extern void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u16 MEM_LEN);// πƒ‹DMA1_CHx
extern u16 DMA1_MEM_LEN;
extern u8 DMA_Send_EN;
extern void DMA1_Start_Send(unsigned char *buf,unsigned char iseof);
#endif


