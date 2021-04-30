#include "oled_drv.h"
#include "delay.h"
#include "codetab.h"
#include "dma.h"

 
#define GPIO_PIN_SET(port, pin, d)    P##port##_##pin## = d  //ע��

/**************************************************************
*��  �ܣ�OLED��ʼ��
*��  ��: ��
*����ֵ: �� 
**************************************************************/

unsigned char OLED_Screen_Buff[DIS_BUFFER]={0};  //size 6*8
OLED_Show_Handler OLED_Assistant={0,0,0,0,0,0,0,0};
void OLED_Hinit(void) 
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);	//ʹ��PB,PE�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;	 		 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;							//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;							//IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOE, &GPIO_InitStructure);					     

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;						// �˿�����, �������
	GPIO_Init(GPIOB, &GPIO_InitStructure);										//������� ��IO���ٶ�Ϊ50MHz
}



#define LCD_LED(d)		GPIO_WriteBit(GPIOD, GPIO_Pin_8, d)		//PD8
#define LCD_D1(d)		GPIO_WriteBit(GPIOB, GPIO_Pin_8, d)		//PB8
#define LCD_D0(d)		GPIO_WriteBit(GPIOB, GPIO_Pin_9, d)		//PB9 
#define LCD_DC(d)		GPIO_WriteBit(GPIOE, GPIO_Pin_1, d)		//PE1 
#define LCD_RES(d)		GPIO_WriteBit(GPIOE, GPIO_Pin_2, d)		//PE2
#define LCD_CS(d)		GPIO_WriteBit(GPIOE, GPIO_Pin_3, d)		//PE3

#define LCD_CLK(d)		LCD_D0(d)
#define LCD_MOSI(d)		LCD_D1(d)

#define LCD_IODELAY	

#define OLED_CS_Clr()  LCD_CS(0)
#define OLED_CS_Set()  LCD_CS(1)

#define OLED_RST_Clr() LCD_RES(0)
#define OLED_RST_Set() LCD_RES(1)

#define OLED_DC_Clr() LCD_DC(0)
#define OLED_DC_Set() LCD_DC(1)

#define OLED_WR_Clr() 
#define OLED_WR_Set() 

#define OLED_RD_Clr() 
#define OLED_RD_Set() 

#define OLED_SCLK_Clr() LCD_CLK(0)
#define OLED_SCLK_Set() LCD_CLK(1)

#define OLED_SDIN_Clr() LCD_MOSI(0)
#define OLED_SDIN_Set() LCD_MOSI(1)

void OLED_ledTest(uint8_t d)
{
	LCD_LED(d);
}

/**************************************************************
*��  �ܣ�OLED��������
*��  ��: dat ���� cmd ����
*����ֵ: �� 
**************************************************************/
void OLED_WR_Byte(uint8_t dat,uint8_t cmd)
{
#if 1
	uint8_t i;			  
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();		  
	OLED_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
		   OLED_SDIN_Set();
		else 
		   OLED_SDIN_Clr();
		OLED_SCLK_Set();
		dat<<=1;   
	}				 		  
	OLED_CS_Set();
	OLED_DC_Set();   
#else
	if(cmd)
		LCD_DO_WRITE();
	else
		LCD_DO_CONTROL();	
		LCD_SPI_BEGIN();
		//LCD_DO_WRITE();
		LCD_SPI_TX(dat);//ch);
		LCD_SPI_WAIT_RXRDY();
		LCD_SPI_END();
#endif
} 

/**************************************************************
*��  �ܣ�OLED���ù��
*��  ��: x ������   y ������
*����ֵ: �� 
**************************************************************/
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
//	OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD); 
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
}   	  

/**************************************************************
*��  �ܣ�����OLED��ʾ
*��  ��: ��
*����ֵ: �� 
**************************************************************/
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}

/**************************************************************
*��  �ܣ��ر�OLED��ʾ
*��  ��: ��
*����ֵ: �� 
**************************************************************/
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 	
/**************************************************************
*��  �ܣ���������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!
*��  ��: ��
*����ֵ: �� 
**************************************************************/
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	}//������ʾ
}
			 
/**************************************************************
*��  �ܣ���ָ��λ����ʾһ���ַ�,���������ַ�
*��  ��: x:0~127  y:0~63  
*����ֵ: �� 
**************************************************************/
void OLED_ShowChar(u8 x,u8 y,u8 chr)
{      	
	unsigned char c=0,i=0;	
	c=chr-' ';				//�õ�ƫ�ƺ��ֵ
	
	if(x>Max_Column-1){x=0;y=y+2;}
	if(SIZE ==16)
	{
		OLED_Set_Pos(x,y);	
		for(i=0;i<8;i++)
		OLED_WR_Byte(F8X16[c*16+i],OLED_DATA); 
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
		OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
	}
	else 
	{	
		OLED_Set_Pos(x,y+1);
		for(i=0;i<6;i++)
		OLED_WR_Byte(F6x8[c][i],OLED_DATA);				
	}
}

/**************************************************************
*��  �ܣ�m^n����
*��  ��: m n
*����ֵ: M^N 
**************************************************************/
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  

/**************************************************************
*��  �ܣ���ʾ2������
*��  ��: x,y :�������	len :���ֵ�λ�� size:�����С
*����ֵ: �� 
**************************************************************/
void OLED_ShowNum(u8 x,u8 y,u16 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ');
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0'); 
	}
}

/**************************************************************
*��  �ܣ���ʾһ���ַ��Ŵ�
*��  ��: x,y :�������	chr �ַ����׵�ַ
*����ֵ: �� 
**************************************************************/
void OLED_ShowString(u8 x,u8 y,u8 *chr)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{
		OLED_ShowChar(x,y,chr[j]);
		x+=8;
		if(x>120){x=0;y+=2;}
		j++;
	}
}

/**************************************************************
*��  �ܣ���ʾ����
*��  ��: x,y :�������	no ��Ӧ����ģ
*����ֵ: �� 
**************************************************************/
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
	for(t=0;t<16;t++)
	{
		OLED_WR_Byte(String16_16[2*no][t],OLED_DATA);
		adder+=1;
	}	
	OLED_Set_Pos(x,y+1);	
	for(t=0;t<16;t++)
	{	
		OLED_WR_Byte(String16_16[2*no+1][t],OLED_DATA);
		adder+=1;
	}					
} 


/********************************************
*������������ʾBMPͼƬ128��64       
*��������ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7
*����ֵ����          
*********************************************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
	unsigned int j=0;
	unsigned char x,y;
  
	if(y1%8==0) y=y1/8;      
	else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
		for(x=x0;x<x1;x++)
		{      
			OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
		}
	}
} 

/**************************************************************
*��  �ܣ���ʼ��SSD1306	
*��  ��: ��
*����ֵ: �� 
**************************************************************/
void OLED_SInit(void)
{ 	
  	OLED_RST_Set();
	Delay_ms(100);
	OLED_RST_Clr();
	Delay_ms(100);
	OLED_RST_Set(); 
					  
	OLED_WR_Byte(0xAE,OLED_CMD);	//--turn off oled panel
//	OLED_WR_Byte(0x02,OLED_CMD);	//---set low column address
//	OLED_WR_Byte(0x10,OLED_CMD);	//---set high column address

	OLED_WR_Byte(0x40,OLED_CMD);	//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);	//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD);	// Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_CMD);	//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
	OLED_WR_Byte(0xC8,OLED_CMD);	//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
	OLED_WR_Byte(0xA6,OLED_CMD);	//--set normal display
	OLED_WR_Byte(0xA8,OLED_CMD);	//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);	//--1/64 duty
	OLED_WR_Byte(0x81,OLED_CMD);	//�Աȶ�����
	OLED_WR_Byte(0xfF,OLED_CMD);	//1~255;Ĭ��0X7F (��������,Խ��Խ��)
	OLED_WR_Byte(0xD3,OLED_CMD);	//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);	//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD);	//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);	//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD);	//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);	//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_CMD);	//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);	//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD);	//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);	//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);	//
	OLED_WR_Byte(0x8D,OLED_CMD);	//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);	//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);	// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);	// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_CMD);	//--turn on oled panel
	
	OLED_WR_Byte(0xAF,OLED_CMD); 	/*display ON*/ 
	OLED_Clear();
	OLED_Set_Pos(0,0); 	
}  

/**************************************************************
*��  �ܣ�OLED����
*��  ��: ��
*����ֵ: �� 
**************************************************************/	
void OLED_Test(void)
{
	static uint8_t c = 0;
	static uint8_t t = '1';
	if(c == 0)
	{
		OLED_Clear();
		OLED_ShowCHinese(0,0,0);	//��
		OLED_ShowCHinese(18,0,1);	//��
		OLED_ShowCHinese(36,0,2);	//԰
		OLED_ShowCHinese(54,0,3);	//��
		OLED_ShowCHinese(72,0,4);	//��
		OLED_ShowCHinese(90,0,5);	//��
		OLED_ShowCHinese(108,0,6);	//��
		OLED_ShowString(0,3,"1.3' OLED TEST");
		//OLED_ShowString(8,2,"ZHONGJINGYUAN");  
	    //OLED_ShowString(20,4,"2014/05/01");  
		OLED_ShowString(0,6,"ASCII:");  
		OLED_ShowString(63,6,"CODE:");  
		OLED_ShowChar(48,6,t);		//��ʾASCII�ַ�	   
		t++;
		if(t>'~')t=' ';
		OLED_ShowNum(103,6,t,3,16); //��ʾASCII�ַ�����ֵ 				
	}
#if EN_BMP
	else if(c == 1)
	{
		OLED_Clear();
		OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1);  //ͼƬ��ʾ(ͼƬ��ʾ���ã����ɵ��ֱ�ϴ󣬻�ռ�ý϶�ռ䣬FLASH�ռ�8K��������)
	}
	else 
	{
		OLED_Clear();
		OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP2);
	}
#endif
	if(++c > 2)
		c = 0;		
}

/**************************************************************
*��  �ܣ�OLED����2
*��  ��: ��
*����ֵ: �� 
**************************************************************/	
void OLED_Disp_Logo(void)
{
	OLED_ShowString(8,0,"<<R&C Zigbee>>\0");
}

/**************************************************************
*��  �ܣ�OLED��ʾ ʮ������
*��  ��: x,y ����  d ��Ҫ��ʾ������
*����ֵ: �� 
**************************************************************/	
void OLED_Disp_hex16(uint8_t x,uint8_t y,uint32_t d)
{
	uint8_t tbuf[7];
	uint8_t i,t;
	
	tbuf[0] = '0';
	tbuf[1] = 'x';
	for(i = 0;i<4;i++)
	{
		t = (d>>12)&0x0f;
		tbuf[i+2] = (t < 10)? (t + '0'):(t + ('A'-10));
		d <<= 4;
	}
	tbuf[6] = 0;
	OLED_ShowString(x,y,tbuf);
}

/**************************************************************
*��  �ܣ�OLED��ʾָ��������
*��  ��: ch  id addr ��Ҫ��ʾ������
*����ֵ: �� 
**************************************************************/	
void OLED_Disp_Info(uint16_t id,uint8_t ch,uint8_t addr)
{
	uint8_t t;
	OLED_ShowString(0,2,"CHANNEL:\0");
	if(ch >10)
		t = 2;
	else
		t = 1;
	OLED_ShowNum(8*9,2,ch,t,16);
	OLED_ShowString(0,4,"PAN_ID :\0");
	OLED_Disp_hex16(8*9,4,id);
	OLED_ShowString(0,6,"PAN_ADD:\0");
	OLED_ShowNum(8*9,6,addr,3,16);
}
/**************************************************************
*��  �ܣ�OLED��ʾ����3
*��  ��: ch  id addr ��Ҫ��ʾ������
*����ֵ: �� 
**************************************************************/	
void OLED_Disp_All(uint16_t id,uint8_t ch,uint8_t addr)
{ 
	OLED_Hinit();
	OLED_SInit();
	OLED_Clear();
	OLED_Disp_Logo();
	OLED_Disp_Info(id,ch,addr);
}

//----------------------------------------OLED DEMO TEST FOLLOWING:---------------------------------------------------//
unsigned char NumToChar(unsigned char num){
	unsigned char nchar_list[10]={'0','1','2','3','4','5','6','7','8','9'};
	if(num<10){
		return nchar_list[num];
	}
	else if(num==0x0a || num==0x0d){
		return ' ';
	}
	return num;
}

void OLED_Display_BMP(unsigned char *pic){
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++){
			OLED_WR_Byte(*pic++,OLED_DATA); 
		}
	}
}
void OLED_Display_4x8_Num(void){
	u8 i,j;
	OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
	OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
	OLED_WR_Byte (0x10,OLED_CMD);
	for(i=0;i<9;i++){
		for(j=0;j<4;j++){
			OLED_WR_Byte(F4x8[i][j],OLED_DATA);
		}
		
	}
}

void Roll_Screen(unsigned char mode){//Roll_Screen(ROLL_HORIZONTAL_LEFT_WITH_VERTICAL_MODE);
	if(mode==ROLL_HORIZONTAL_LEFT_WITH_VERTICAL_MODE){
		//B-D,start page - end page
		OLED_WR_Byte(0x2e,OLED_CMD);//�ع���
		OLED_WR_Byte(0x2A,OLED_CMD);//29���ң�2a����
		OLED_WR_Byte(0x00,OLED_CMD);//A:���ֽ�
		OLED_WR_Byte(0x00,OLED_CMD);//B:ˮƽ��ʼҳ
		OLED_WR_Byte(0x02,OLED_CMD);//C:ˮƽ�����ٶ� 128fps
		OLED_WR_Byte(0x00,OLED_CMD);//D:ˮƽ����ҳ  ������
		OLED_WR_Byte(0x08,OLED_CMD);//E:ÿ�δ�ֱ����λ��
		OLED_WR_Byte(0x2f,OLED_CMD);
		
//		OLED_WR_Byte(0x2e,OLED_CMD);

	}
	else if(mode==ROLL_HORIZONTAL_RIGHT_WITH_VERTICAL__MODE){
		OLED_WR_Byte(0x2e,OLED_CMD);//�ع���
		OLED_WR_Byte(0x29,OLED_CMD);//29���ң�2a����
		OLED_WR_Byte(0x00,OLED_CMD);//A:���ֽ�
		OLED_WR_Byte(0x00,OLED_CMD);//B:ˮƽ��ʼҳ
		OLED_WR_Byte(0x00,OLED_CMD);//C:ˮƽ�����ٶ�
		OLED_WR_Byte(0x01,OLED_CMD);//D:ˮƽ����ҳ
		OLED_WR_Byte(0x01,OLED_CMD);//E:ÿ�δ�ֱ����λ��
		OLED_WR_Byte(0x2f,OLED_CMD);
	}
	else if(mode==ROLL_VERTICAL_MODE){
		OLED_WR_Byte(0x2e,OLED_CMD);//�ع���
		OLED_WR_Byte(0x26,OLED_CMD);//29���ң�2a����
		OLED_WR_Byte(0x00,OLED_CMD);//A:���ֽ�
		OLED_WR_Byte(0x00,OLED_CMD);//B:ˮƽ��ʼҳ
		OLED_WR_Byte(0x00,OLED_CMD);//C:ˮƽ�����ٶ�
		OLED_WR_Byte(0x01,OLED_CMD);//D:ˮƽ����ҳ
		OLED_WR_Byte(0x01,OLED_CMD);//E:ÿ�δ�ֱ����λ��
		OLED_WR_Byte(0x2f,OLED_CMD);
	}

}

void OLED_ShowChar_6x8_Pos(unsigned char x,unsigned char y,unsigned char arg){//show num or char
	//x :0~20
	//y :0~7
	//size :6*8
	
	u8 j;
	if(arg>0x7f){
		return;
	}
	OLED_Set_Pos(x*6,y);
	arg=NumToChar(arg);
	arg-=' ';
	for(j=0;j<6;j++){
		OLED_WR_Byte(F6x8[arg][j],OLED_DATA);
	}
}
void OLED_ShowStr_6x8(unsigned x,unsigned char y,unsigned char *str){
	while((*str)!='\0'){
		if(x>20){
			x=0;
			y+=1;
		}
		if((y==7)&&(x==20)){
			break;
		}
		OLED_ShowChar_6x8_Pos(x,y,*str);
		str++;
		x++;
	}

}

void OLED_Show_Array(void){
	unsigned char i,j,c=0;
	for(i=0;i<8;i++){
		for(j=0;j<21;j++){
			OLED_ShowChar_6x8_Pos(j,i,' '+c);
			Delay_ms(20);
			c++;
			if(c==91) return;
		}
	}
}



unsigned char Cal_Line_Remain(unsigned int arg){//���㲻��һ�е����ݵĳ���,���ڻ���ʱ������Ļָ��ƫ�Ƶ�
	unsigned char remain;
	remain=arg-(unsigned char)(arg/20)*20;
	remain=20-remain;
	return remain;
}
void Cal_Screen_Axis(unsigned int wptr,unsigned char *x,unsigned char *y){//������Ҫ��ʾ������
	*y=(unsigned char)(wptr/20);
	*x=wptr-(*y)*20;
}


unsigned int Cal_Recv_Len(unsigned int last_len,unsigned int new_len){//������յ������ݳ���,���ڲ�������ʾ
	unsigned int recv_len;
	if(new_len>=last_len){  //��ֹѭ������ʱ�Ľ���ָ���ܵ���һ�ν���ָ��ǰ��
		recv_len=new_len-last_len;
	}
	else{
		recv_len=(DIS_BUFFER-last_len)+recv_len;
	}
	return recv_len;
}

void OLED_Screen_Show(void){
	unsigned int i,tmp,len;
	len=Cal_Recv_Len(OLED_Assistant.OLED_ShowBuf_Last_DLen,OLED_Assistant.OLED_ShowBuf_DLen);
//	for(i=0;i<len;i++)
	for(i=0;i<len;i++){
		tmp=OLED_Screen_Buff[OLED_Assistant.OLED_ShowBuf_RPtr];
		if(tmp=='\0'){//�����ַ�����
			OLED_Assistant.OLED_Write_EN=0;
			break;
		}
		if(tmp=='\r'){//����
			OLED_Assistant.OLED_ShowBuf_RPtr+=2;
			tmp=Cal_Line_Remain(OLED_Assistant.OLED_Screen_WPtr)+1;
			OLED_Assistant.OLED_Screen_WPtr+=tmp;
			i+=2;
			continue;
		}
		if(OLED_Assistant.OLED_Screen_WPtr>=DIS_ONE_PAGE_BUF){
			OLED_Assistant.OLED_Flush_EN=1;//��Ļд����ˢ��
			OLED_Assistant.OLED_Screen_WPtr=0;
		}
		
		if(OLED_Assistant.OLED_Flush_EN){
			OLED_Clear();
			OLED_Assistant.OLED_Flush_EN=0;
		}
		Cal_Screen_Axis(OLED_Assistant.OLED_Screen_WPtr,&OLED_Assistant.OLED_X,&OLED_Assistant.OLED_Y);//��������
		OLED_ShowChar_6x8_Pos(OLED_Assistant.OLED_X,OLED_Assistant.OLED_Y,tmp);
		OLED_Assistant.OLED_Screen_WPtr++;
		OLED_Assistant.OLED_ShowBuf_RPtr++;
		if(OLED_Assistant.OLED_ShowBuf_RPtr>=DIS_BUFFER){
			OLED_Assistant.OLED_ShowBuf_RPtr=0;
		}
	}
	if(OLED_Assistant.OLED_ShowBuf_RPtr==OLED_Assistant.OLED_ShowBuf_DLen){
		OLED_Assistant.OLED_Write_EN=0;  //ʵʱ����
	}
	
}

void OLED_Dispaly_Test(void){
	//OLED_Display_BMP(BMP1);
	//OLED_Display_4x8_Num();
//	OLED_ShowChar_6x8_Pos(20,7,'A');
	//OLED_Show_Array();
	OLED_ShowStr_6x8(3,2,"hello world i'm jiang wei there is china!");
//	u8 i,n;		    
//	for(i=0;i<8;i++)  
//	{  
//		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
//		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
//		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
//		for(n=0;n<128;n++){
//			OLED_WR_Byte(0xaa,OLED_DATA); 
//			Delay_ms(50);
//		}
//	}//������ʾ
}

/*
Failed
void OLED_Roll_Screen(void){
	unsigned char i,j;
	for(i=0;i<20;i++){ 
		if(OLED_Screen_Buff[i]=='\r'){//���һ���Ƿ��лس�����,
			i+=2;
			break;
		}
	}
	for(j=0;j<DIS_BUFFER-i;j++){ //����һ�е�������ǰ��
		OLED_Screen_Buff[j]=OLED_Screen_Buff[j+i];
	}
	for(;j<DIS_BUFFER;j++){
		OLED_Screen_Buff[j]=' ';
	}
	OLED_Clear();
}
void OLED_Screen_Show(void){
	unsigned int dis_buffer=sizeof(OLED_Screen_Buff);
	unsigned char i,var;
	for(i=0;i<dis_buffer;i++){
		var=OLED_Screen_Buff[i];
		if(var!='\0'){
			if(var=='\r'){ //��⵽�س�������������һ����ʾ
				OLED_Y++;
				OLED_X=0;
				i++;		//����\n
			}
			if((OLED_Y==7)&&(OLED_X==20)){
				OLED_Roll_Screen();
				OLED_X=0;
				OLED_Y=0;
				break;
			}
			if(OLED_X>20){
				OLED_X=0;
				OLED_Y++;
			}
			
		}
		else{
			break;
		}
	}
}
*/



