#include "ds18b20.h"
#include "oled_drv.h"
#include "delay.h"
#include "main.H"
 
uint8_t display_flag;
unsigned int DS_Temperature=0;
unsigned char DS18b20_Buffer[4]={0};
unsigned char DS18B20_EOC=0;
unsigned char DS18B20_SOC=0;
 
void Display_DS18B20_Temp(void){
	char i,j;
	for(i=0,j=0;i<5;i++){
		if(i!=3){
			OLED_ShowChar_6x8_Pos(3+i,2,DS18b20_Buffer[j]);
			j++;
		}
		else{
			OLED_ShowChar_6x8_Pos(3+i,2,'.');
		}
	}
}

void DS18B20__Buffer_Updata(void){
	DS18b20_Buffer[0]=DS_Temperature/1000;
	DS18b20_Buffer[1]=DS_Temperature%1000/100;
	DS18b20_Buffer[2]=DS_Temperature%100/10;
	DS18b20_Buffer[3]=DS_Temperature%10;
}
/**************************************************************
*功  能：复位DS18B20
*参  数: 无
*返回值: 无 
**************************************************************/
void DS18B20_Rst(void)	   
{                 
	DS18B20_IO_OUT(); //SET PA0 OUTPUT
    DS18B20_DQ_OUT=0; //拉低DQ
    Delay_us(750);    //拉低750us
    DS18B20_DQ_OUT=1; //DQ=1 
	Delay_us(15);     //15US
	DS18B20_IO_IN();
	Delay_us(100);
}

/**************************************************************
*功  能：等待DS18B20的回应
*参  数: 无
*返回值: 返回1:未检测到DS18B20的存在   返回0:存在
**************************************************************/
uint8_t DS18B20_Check(void) 	   
{   
	uint8_t retry=0;
	DS18B20_IO_IN();//SET PA0 INPUT	 
    while (DS18B20_DQ_IN&&retry<200)
	{
		retry++;
		Delay_us(1);
	};	 
	if(retry>=200)return 1;
	else retry=0;
    while (!DS18B20_DQ_IN&&retry<240)
	{
		retry++;
		Delay_us(1);
	};
	if(retry>=240)return 1;	    
	return 0;
}

/**************************************************************
*功  能：从DS18B20读取一个位
*参  数: 无
*返回值: 1/0 
**************************************************************/
uint8_t DS18B20_Read_Bit(void) 			 // read one bit
{
    uint8_t data;
	DS18B20_IO_OUT();	//SET PA0 OUTPUT
    DS18B20_DQ_OUT=0; 
	Delay_us(2);
    DS18B20_DQ_OUT=1; 
	DS18B20_IO_IN();	//SET PA0 INPUT
	Delay_us(12);
	if(DS18B20_DQ_IN)data=1;
    else data=0;	 
    Delay_us(50);           
    return data;
}

/**************************************************************
*功  能：从DS18B20读取一个字节
*参  数: 无
*返回值: 读到的数据 
**************************************************************/
uint8_t DS18B20_Read_Byte(void)    // read one byte
{        
    uint8_t i,j,dat;
    dat=0;
	for (i=1;i<=8;i++) 
	{
        j=DS18B20_Read_Bit();
        dat=(j<<7)|(dat>>1);
    }						    
    return dat;
}

/**************************************************************
*功  能：写一个字节到DS18B20
*参  数: dat：要写入的字节
*返回值: 无 
**************************************************************/
void DS18B20_Write_Byte(uint8_t dat)     
 {             
    uint8_t j;
    uint8_t testb;
	DS18B20_IO_OUT();			//SET PA0 OUTPUT;
    for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;
        if (testb) 
        {
            DS18B20_DQ_OUT=0;	// Write 1
            Delay_us(2);                            
            DS18B20_DQ_OUT=1;
            Delay_us(60);             
        }
        else 
        {
            DS18B20_DQ_OUT=0;	// Write 0
            Delay_us(60);             
            DS18B20_DQ_OUT=1;
            Delay_us(2);                          
        }
    }
}

/**************************************************************
*功  能：开始温度转换
*参  数: 无
*返回值: 无 
**************************************************************/
void DS18B20_Start(void)		// ds1820 start convert
{   						               
    DS18B20_Rst();	   
	DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);	// skip rom
    DS18B20_Write_Byte(0x44);	// convert
} 

/**************************************************************
*功  能：初始化DS18B20的IO口 DQ 同时检测DS的存在
*参  数: 无
*返回值: 返回1:不存在  返回0:存在
**************************************************************/
uint8_t DS18B20_Init(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能PG端口时钟
	
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				//PA0端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	//初始化IO口
 	GPIO_SetBits(GPIOA,GPIO_Pin_0);			//PG11 输出高
	DS18B20_Rst();
	
	return DS18B20_Check();
}    

/**************************************************************
*功  能：配置输出端口
*参  数: 无
*返回值: 无 
**************************************************************/
void DQ1820_OUT(void)
{ 
  GPIO_InitTypeDef GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		//开漏输出
  GPIO_Init(GPIOA,&GPIO_InitStructure);
}

/**************************************************************
*功  能：配置输入端口
*参  数: 无
*返回值: 无 
**************************************************************/
void DQ1820_IN(void)  
{
  GPIO_InitTypeDef GPIO_InitStructure;
   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
  GPIO_Init(GPIOA,&GPIO_InitStructure);
} 
 
/**************************************************************
*功  能：从ds18b20得到温度值，精度：0.1C  温度值 （-550~1250）
*参  数: 无
*返回值: 无 
**************************************************************/
//short DS18B20_Get_Temp(void)
//{
//    uint8_t temp;
//    uint8_t TL,TH;
//	short tem;
//    DS18B20_Start ();			//ds1820 start convert
//    DS18B20_Rst();
//    DS18B20_Check();	 
//    DS18B20_Write_Byte(0xcc);	//skip rom
//    DS18B20_Write_Byte(0xbe);	//convert	    
//    TL=DS18B20_Read_Byte();		//LSB   
//    TH=DS18B20_Read_Byte();		//MSB  
//	    	  
//    if(TH>7)
//    {
//        TH=~TH;
//        TL=~TL; 
//        temp=0;				//温度为负  
//    }else temp=1;			//温度为正	  	  
//    tem=TH;					//获得高八位
//    tem<<=8;    
//    tem+=TL;				//获得低八位
//    tem=(float)tem*0.625;	//转换     
//	if(temp)return tem;		//返回温度值
//	else return -tem;    
//} 
void DS18B20_Get_Temp(void)
{
  static unsigned char TL,TH;
	
	static unsigned char step=0;
	short tem;
	switch(step){
		case 0:{
			DMA_Cmd(DMA1_Channel6, DISABLE );
			DMA_Cmd(DMA1_Channel7, DISABLE );
			DS18B20_Rst();
			step++;
			break;
		}
		case 1:{
			DS18B20_Write_Byte(0xcc);
			step++;
			break;
		}
		case 2:{
			DS18B20_Write_Byte(0x44);
			step++;
			break;
		}
		case 3:{
			DS18B20_Rst();
			step++;
			break;
		}
		case 4:{
			DS18B20_Write_Byte(0xcc);
			step++;
			break;
		}
		case 5:{
			DS18B20_Write_Byte(0xbe);
			step++;
			break;
		}
		case 6:{
			TL=DS18B20_Read_Byte();
			step++;
			break;
		}
		case 7:{
			TH=DS18B20_Read_Byte();
			step++;
			break;
		}
		case 8:{
			tem=TH;					
			tem<<=8;    
			tem+=TL;				
			DS_Temperature=(float)tem*0.625;
			step=0;
			DS18B20__Buffer_Updata();
			DS18B20_EOC=1;
			DS18B20_SOC=0;
			DMA_Cmd(DMA1_Channel6, ENABLE );
			DMA_Cmd(DMA1_Channel7, ENABLE );
			break;
		}
		default:break;
	}
}

/**************************************************************
*功  能：DS18B20传感器检测
*参  数: 无
*返回值: 无 
**************************************************************/
//void DS18B20_WHILE(void)			
//{
//	unsigned char init_0 = 0;
//	while(DS18B20_Init())
//	{
//		init_0 ++;
//		if(init_0 == 240)
//		OLED_ShowString(15,0,"DS18B20 Error",16);
//	}
//	OLED_ShowString(15,0,"DS18B20 OK!!!",16);
//}

/**************************************************************
*功  能：OLED显示温度值
*参  数: 无
*返回值: 无 
**************************************************************/
//void DS18B20_Test(void) 
//{
//	short temperature;
//	uint8_t i=8;
//	uint8_t startaddr = 20;
//	
//	if(display_flag == 0)
//	{
//		display_flag ++;
//		OLED_Show_Str(startaddr,4,"温度:",16);
//		OLED_Show_Str(startaddr+i*7,4,".",16);
//		OLED_Show_Str(startaddr+i*9,4,"℃",16);
//	}
//	
//	//temperature=DS18B20_Get_Temp();
//	if(temperature<0)
//	{
//		OLED_ShowString(startaddr+i*4,4,"-",16); 				//显示负号
//		temperature=-temperature;							//转为正数
//	}
//	else
//		OLED_ShowString(startaddr+i*4,4," ",16); 				//去掉负号
//	
//	OLED_ShowInt32Num(startaddr+i*5,4,temperature/10,2,16);     	//显示正数部分
//	OLED_ShowInt32Num(startaddr+i*8,4,temperature%10,1,16);     	//显示小数部分

//}


 
