
#ifndef __LCD_H
#define __LCD_H		
//#include "sys.h"	 
#include "stdlib.h"

typedef struct  
{										    
	u16 width;			//LCD 
	u16 height;			//LCD 
	u16 id;				//LCD ID
	u8  dir;			//横屏1 竖屏0	
	u16	wramcmd;		//开始写gram指令
	u16  setxcmd;		//设置x坐标
	u16  setycmd;		//设置y坐标
}_lcd_dev; 	  

//LCD参数
extern _lcd_dev lcddev;	//管理lcd重要参数  
extern u16  POINT_COLOR;//画笔颜色默认红色    
extern u16  BACK_COLOR; //背景颜色默认白


//////////////////////////////////////////////////////////////////////////////////	 
//-----------------LCD端口定义---------------- 
#define	LCD_LED PBout(0) //LCD背光  PB0 	    
//LCD地址结构
typedef struct
{
	vu16 LCD_REG;
	vu16 LCD_RAM;
} LCD_TypeDef;
			    
#define LCD_BASE        ((u32)(0x6C000000 | 0x000007FE))
#define LCD             ((LCD_TypeDef *) LCD_BASE)
//////////////////////////////////////////////////////////////////////////////////
	 
//É¨Ãè·½Ïò¶¨Òå
#define L2R_U2D  0 //´Ó×óµ½ÓÒ,´ÓÉÏµ½ÏÂ
#define L2R_D2U  1 //´Ó×óµ½ÓÒ,´ÓÏÂµ½ÉÏ
#define R2L_U2D  2 //´ÓÓÒµ½×ó,´ÓÉÏµ½ÏÂ
#define R2L_D2U  3 //´ÓÓÒµ½×ó,´ÓÏÂµ½ÉÏ

#define U2D_L2R  4 //´ÓÉÏµ½ÏÂ,´Ó×óµ½ÓÒ
#define U2D_R2L  5 //´ÓÉÏµ½ÏÂ,´ÓÓÒµ½×ó
#define D2U_L2R  6 //´ÓÏÂµ½ÉÏ,´Ó×óµ½ÓÒ
#define D2U_R2L  7 //´ÓÏÂµ½ÉÏ,´ÓÓÒµ½×ó	 

#define DFT_SCAN_DIR  L2R_U2D  //Ä¬ÈÏµÄÉ¨Ãè·½Ïò

//»­±ÊÑÕÉ«
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //×ØÉ«
#define BRRED 			 0XFC07 //×ØºìÉ«
#define GRAY  			 0X8430 //»ÒÉ«
//GUIÑÕÉ«

#define DARKBLUE      	 0X01CF	//ÉîÀ¶É«
#define LIGHTBLUE      	 0X7D7C	//Ç³À¶É«  
#define GRAYBLUE       	 0X5458 //»ÒÀ¶É«
//ÒÔÉÏÈýÉ«ÎªPANELµÄÑÕÉ« 
 
#define LIGHTGREEN     	 0X841F //Ç³ÂÌÉ«
//#define LIGHTGRAY        0XEF5B //Ç³»ÒÉ«(PANNEL)
#define LGRAY 			 0XC618 //Ç³»ÒÉ«(PANNEL),´°Ìå±³¾°É«

#define LGRAYBLUE        0XA651 //Ç³»ÒÀ¶É«(ÖÐ¼ä²ãÑÕÉ«)
#define LBBLUE           0X2B12 //Ç³×ØÀ¶É«(Ñ¡ÔñÌõÄ¿µÄ·´É«)
	    															  
void LCD_Init(void);													   	//³õÊ¼»¯
void LCD_DisplayOn(void);													//¿ªÏÔÊ¾
void LCD_DisplayOff(void);													//¹ØÏÔÊ¾
void LCD_Clear(u16 Color);	 												//ÇåÆÁ
void LCD_SetCursor(u16 Xpos, u16 Ypos);										//ÉèÖÃ¹â±ê
void LCD_DrawPoint(u16 x,u16 y);											//»­µã
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color);								//¿ìËÙ»­µã
u16  LCD_ReadPoint(u16 x,u16 y); 											//¶Áµã 
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r);						 			//»­Ô²
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);							//»­Ïß
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);		   				//»­¾ØÐÎ
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);		   				//Ìî³äµ¥É«
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);				//Ìî³äÖ¸¶¨ÑÕÉ«
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);						//ÏÔÊ¾Ò»¸ö×Ö·û
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);  						//ÏÔÊ¾Ò»¸öÊý×Ö
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);				//ÏÔÊ¾ Êý×Ö
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);		//ÏÔÊ¾Ò»¸ö×Ö·û´®,12/16×ÖÌå

void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue);
u16 LCD_ReadReg(u16 LCD_Reg);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(u16 RGB_Code);
void LCD_SSD_BackLightSet(u8 pwm);							//SSD1963 ±³¹â¿ØÖÆ
void LCD_Scan_Dir(u8 dir);									//ÉèÖÃÆÁÉ¨Ãè·½Ïò
void LCD_Display_Dir(u8 dir);								//ÉèÖÃÆÁÄ»ÏÔÊ¾·½Ïò
void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height);	//ÉèÖÃ´°¿Ú					   						   																			 
//LCD·Ö±æÂÊÉèÖÃ
#define SSD_HOR_RESOLUTION		800		//LCDË®Æ½·Ö±æÂÊ
#define SSD_VER_RESOLUTION		480		//LCD´¹Ö±·Ö±æÂÊ
//LCDÇý¶¯²ÎÊýÉèÖÃ
#define SSD_HOR_PULSE_WIDTH		1		//Ë®Æ½Âö¿í
#define SSD_HOR_BACK_PORCH		46		//Ë®Æ½Ç°ÀÈ
#define SSD_HOR_FRONT_PORCH		210		//Ë®Æ½ºóÀÈ

#define SSD_VER_PULSE_WIDTH		1		//´¹Ö±Âö¿í
#define SSD_VER_BACK_PORCH		23		//´¹Ö±Ç°ÀÈ
#define SSD_VER_FRONT_PORCH		22		//´¹Ö±Ç°ÀÈ
//ÈçÏÂ¼¸¸ö²ÎÊý£¬×Ô¶¯¼ÆËã
#define SSD_HT	(SSD_HOR_RESOLUTION+SSD_HOR_BACK_PORCH+SSD_HOR_FRONT_PORCH)
#define SSD_HPS	(SSD_HOR_BACK_PORCH)
#define SSD_VT 	(SSD_VER_RESOLUTION+SSD_VER_BACK_PORCH+SSD_VER_FRONT_PORCH)
#define SSD_VPS (SSD_VER_BACK_PORCH)

#endif  