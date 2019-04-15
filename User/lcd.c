#include "main.h"

#define LCD_CS_DOWN   GPIO_ResetBits(GPIOG,GPIO_Pin_12)
#define LCD_CS_UP     GPIO_SetBits(GPIOG,GPIO_Pin_12)
#define LCD_CMD       GPIO_ResetBits(GPIOG,GPIO_Pin_0)
#define LCD_DATA      GPIO_SetBits(GPIOG,GPIO_Pin_0)
#define LCD_WR_DOWN   GPIO_ResetBits(GPIOD,GPIO_Pin_5)
#define LCD_WR_UP     GPIO_SetBits(GPIOD,GPIO_Pin_5)
#define LCD_RD_DOWN   GPIO_ResetBits(GPIOD,GPIO_Pin_4)
#define LCD_RD_UP     GPIO_SetBits(GPIOD,GPIO_Pin_4)

//RS线接到了 FMSC的A10上，16根数据线对应fmsc 16位款宽
void init_lcd()
{
  GPIO_InitTypeDef GPIO_InitStructure;
  //PG12 CS， 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  //PD5 WR 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  //PD4 RD
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
   //LCD_BL PB0 背光
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; ; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  //PG0 RS
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  //D0-1 PD14-15    D2-3 PD0-1     D13-15 PD8-10     D4-D12 PE7-PE15
  //D0~D7
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_14|GPIO_Pin_15|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOE, &GPIO_InitStructure);
 
  //以下代码很重要，初始化时一定要确定好初始化后的引脚状态，否则引脚状态不确定，导致拉高拉低动作没有完成，使得初始化失败。
  GPIO_SetBits(GPIOD,GPIO_Pin_5|GPIO_Pin_4|GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_14|GPIO_Pin_15|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10);
  GPIO_SetBits(GPIOB,GPIO_Pin_0);
  GPIO_SetBits(GPIOG,GPIO_Pin_12|GPIO_Pin_0);
  GPIO_SetBits(GPIOE, GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
}
  
void lcd_dataout(uint16_t data)
{
   GPIO_SetBits(GPIOD,(data&0x0003)<<14);//D0-1 PD14-15 
   GPIO_SetBits(GPIOD,(data&0x000C)>>2);//D2-3 PD0-1 
   GPIO_SetBits(GPIOD,(data&0xE000)>>5);// D13-15 PD8-10 
   GPIO_SetBits(GPIOE,(data&0x1FF0)<<3);// D4-D12 PE7-PE15
}
u16 lcd_dataread()
{
  u16 d0to1=0;
  u16 d2to3=0;
  u16 d4to12=0;
  u16 d13to15=0;
  d0to1=GPIO_ReadOutputData(GPIOD)>>14;
  d2to3=GPIO_ReadOutputData(GPIOD)<<2;
  d4to12=GPIO_ReadOutputData(GPIOE)>>3;
  d13to15=GPIO_ReadOutputData(GPIOD)<<5;
  return d0to1&0x0003+d2to3&0x000C+d4to12&0x1FF0+d13to15&0xE000;
}
 void lcd_writecmd(u16 data)
 {
  LCD_CS_DOWN;
  LCD_CMD;
  LCD_RD_UP;
  LCD_WR_DOWN;
  lcd_dataout(data);
  LCD_WR_UP;
  LCD_CS_UP;
 }
 void lcd_writedata(u16 data)
 {
  LCD_CS_DOWN;
  LCD_DATA;
  LCD_RD_UP;
  LCD_WR_DOWN;
  lcd_dataout(data);
  LCD_WR_UP;
  LCD_CS_UP;
 }
 u16 lcd_read()
 {
   u16 temp;
  LCD_CS_DOWN;
  LCD_DATA;
  LCD_RD_DOWN;
  temp=lcd_dataread();
  LCD_RD_UP;
  LCD_CS_UP;
   return temp;
 }
u16 lcd_readID()
{
   u16 ID_highbyte=0;
   u16 ID_lowbyte=0;
  //send 0xD3 ,read xx,read 00,readID high byte,read ID low byte
   lcd_writecmd(0xD3);
   lcd_writecmd(0xD3);
   lcd_read();
   lcd_read();
   ID_highbyte=lcd_read();
   ID_lowbyte=lcd_read();
   return ID_highbyte<<8+ID_lowbyte&0x00FF;
}
void lcd_test()
{
  u16 id=0;
  u8 len;
  char s[16];
  id=lcd_readID();
  len=u16_to_string(id,s);
  uartsend_string("LCDID is",sizeof("LCDID is"));
  uartsend_string(s,len);
  uartsend_string("\r\n",sizeof("\r\n"));
}