
#ifndef __OLED_DRV_H__
#define __OLED_DRV_H__

#include "sys.h"
#include "stm32f10x.h"
#include "main.h"





#define OLED_MODE 0
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	  

#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����
#define ROLL_VERTICAL_MODE 0  //oled do not have separated vertical mode
#define ROLL_HORIZONTAL_LEFT_WITH_VERTICAL_MODE 1
#define ROLL_HORIZONTAL_RIGHT_WITH_VERTICAL__MODE 2
#define DIS_ONE_PAGE_BUF 168		//һҳ������
#define DIS_BUFFER DIS_ONE_PAGE_BUF*8  //һҳ������  size()6*8

typedef struct{
	/*
	OLED�Դ�ָ��:���ڶ�λ�Դ��ȡλ��  //��ָ��Ǳ�ָ��,ֻ�Ǳ���,��ͬ
	OLED�Դ����ݳ���:�������ݴ洢(�������ֵ,��Ϊ0,ֱ�Ӹ���)
	OLED�Դ���һ�����ݳ���:���ڼ�����յ����ݳ���
	OLED��Ļλ��ָ��:���ڶ�λ��ǰ��ʾλ��
	OLED��Ļλ��X���� COLUMN
	OLED��Ļλ��Y����	ROW
	OLEDˢ��ʹ�ܱ�ʶ
	OLED��ʾ����ʹ�ܱ�ʶ
	*/
	unsigned int OLED_ShowBuf_RPtr;
	unsigned int OLED_ShowBuf_DLen;
	unsigned int OLED_ShowBuf_Last_DLen;
	unsigned int OLED_Screen_WPtr;
	unsigned char OLED_X;//column (0-20)
	unsigned char OLED_Y;//row  (0-7)
	unsigned char OLED_Flush_EN;
	unsigned char OLED_Write_EN;
}OLED_Show_Handler;

extern OLED_Show_Handler OLED_Assistant;


extern unsigned char OLED_X;//column (0-20)
extern unsigned char OLED_Y;
extern unsigned char OLED_Screen_Buff[DIS_BUFFER];


void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t); 
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr); 
void OLED_ShowNum(u8 x,u8 y,u16 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y, u8 *p);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);

void OLED_Hinit(void);
void OLED_SInit(void);
void OLED_Test(void);

void OLED_Disp_All(uint16_t id,uint8_t ch,uint8_t addr);

void Find_HZ(u8 x,u8 y,u8 *buf,u8 lenth);
void OLED_Dispaly_Test(void);
void OLED_ShowStr_6x8(unsigned x,unsigned char y,unsigned char *str);
void OLED_ShowChar_6x8_Pos(unsigned char x,unsigned char y,unsigned char arg);

void Roll_Screen(unsigned char mode);
void OLED_Screen_Show(void);
void Copy_To_ShowBuff(U_DMA udma);


#endif //__OLED_DRV_H__

