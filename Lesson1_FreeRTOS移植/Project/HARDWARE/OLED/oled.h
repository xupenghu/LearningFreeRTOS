
#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	    	
#define OLED_MODE 0
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------OLED IIC端口定义----------------  					   

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_10)//SDA IIC接口的时钟信号
#define OLED_SCLK_Set() GPIO_SetBits(GPIOB,GPIO_Pin_10)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_11)//SCL IIC接口的数据信号
#define OLED_SDIN_Set() GPIO_SetBits(GPIOB,GPIO_Pin_11)

 		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


//OLED控制用函数
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size);
void OLED_Init(void);
void OLED_Refresh_Gram(void);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
#endif  
	 



