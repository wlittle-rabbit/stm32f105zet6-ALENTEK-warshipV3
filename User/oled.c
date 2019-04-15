#include "main.h"
#include "xiaohuabao.h"
//每次初始化前都要复位一下，RST拉低复位
//OLED连接方式为8080并口方式
//操作过程如下：先操作DC，再拉低cs片选，然后拉低WR或者RD，在RD或者WR上升沿读取或者写入
//读取数据时，读到的第一个字节弃之不要，第二个才是真正需要的第一个数据
//显存为128*64bit，分为8页，每页128个字节
//为防止写入的数据覆盖之前的状态，可以在单片机SRAM里保存一个128*8字节，修改完后，一次性写入到oled的GRAM

#define WR_DOWN GPIO_ResetBits(GPIOG,GPIO_Pin_14)
#define WR_UP GPIO_SetBits(GPIOG,GPIO_Pin_14)
#define CS_ENABLE GPIO_ResetBits(GPIOD,GPIO_Pin_6)
#define CS_DISABLE GPIO_SetBits(GPIOD,GPIO_Pin_6)
#define DC_CMD  GPIO_ResetBits(GPIOD,GPIO_Pin_3)
#define DC_DATA  GPIO_SetBits(GPIOD,GPIO_Pin_3)
char oledgram[128][8]={0};
void RST_oled();
void oled_elecopen();
void oled_wrbytecmd(uint8_t byte);
void oled_open();
void oled_clear();
void init_oled()
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  //D0~D7
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  //PD3 DC，命令数据标志，0读写命令，1读写数据
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  //PG13 RD,PG15 RST，PG14 WR，
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15; 
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  //PD6 CS， 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  //以下代码很重要，初始化时一定要确定好初始化后的引脚状态，否则引脚状态不确定，导致拉高拉低动作没有完成，使得初始化失败。
  GPIO_SetBits(GPIOD,GPIO_Pin_3|GPIO_Pin_6);
  GPIO_SetBits(GPIOC,0xFF);
  GPIO_SetBits(GPIOG,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
  
  RST_oled();
   //Internal_setting();
  oled_elecopen();
  oled_wrbytecmd(0x20);//页地址模式 ：2 列地址模式：0 行地址模式：1
  oled_wrbytecmd(0x02);
  oled_open();
  oled_clear();
}
void oled_initfor_wr()
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}
void RST_oled()
{
  GPIO_ResetBits(GPIOG,GPIO_Pin_15);
  msleep(100);
  GPIO_SetBits(GPIOG,GPIO_Pin_15);
}
void oled_dataout(uint8_t byte)
{
  if((byte>>7)&0x01)
    GPIO_SetBits(GPIOC,GPIO_Pin_7);
  else
    GPIO_ResetBits(GPIOC,GPIO_Pin_7);
  if((byte>>6)&0x01)
    GPIO_SetBits(GPIOC,GPIO_Pin_6);
  else
    GPIO_ResetBits(GPIOC,GPIO_Pin_6);
   if((byte>>5)&0x01)
    GPIO_SetBits(GPIOC,GPIO_Pin_5);
  else
    GPIO_ResetBits(GPIOC,GPIO_Pin_5);
   if((byte>>4)&0x01)
    GPIO_SetBits(GPIOC,GPIO_Pin_4);
  else
    GPIO_ResetBits(GPIOC,GPIO_Pin_4);
   if((byte>>3)&0x01)
    GPIO_SetBits(GPIOC,GPIO_Pin_3);
  else
    GPIO_ResetBits(GPIOC,GPIO_Pin_3);
   if((byte>>2)&0x01)
    GPIO_SetBits(GPIOC,GPIO_Pin_2);
  else
    GPIO_ResetBits(GPIOC,GPIO_Pin_2);
   if((byte>>1)&0x01)
    GPIO_SetBits(GPIOC,GPIO_Pin_1);
  else
    GPIO_ResetBits(GPIOC,GPIO_Pin_1);
   if((byte>>0)&0x01)
    GPIO_SetBits(GPIOC,GPIO_Pin_0);
  else
    GPIO_ResetBits(GPIOC,GPIO_Pin_0);
}
void oled_wrbytecmd(uint8_t byte)
{
  DC_CMD;
  CS_ENABLE;
  WR_DOWN;
  oled_dataout(byte);
  WR_UP;
  CS_DISABLE;
}
void oled_wrbytedat(uint8_t byte)
{
  DC_DATA;
  CS_ENABLE;
  WR_DOWN;
  oled_dataout(byte);
  WR_UP;
  CS_DISABLE;
}
void oled_open()
{
  oled_wrbytecmd(0xAF);
}
void oled_close()
{
  oled_wrbytecmd(0xAE);
}
void oled_elecopen()
{
  oled_wrbytecmd(0x8D);
  oled_wrbytecmd(0x14);
}
void oled_elecclose()
{
  oled_wrbytecmd(0x8D);
  oled_wrbytecmd(0x10);
}
void oled_refresh_gram()
{
  for(int i=0;i<8;i++){
     oled_wrbytecmd(0xB0+i);
     oled_wrbytecmd(0x00);
     oled_wrbytecmd(0x10);
     for(int j=0;j<128;j++){
       oled_wrbytedat(oledgram[j][i]);
     }
  }
}
void oled_drawpoint(u8 X,u8 Y)
{
  u8 tmp=0;
  u8 x,y;
  if(X>=128||Y>=64){
    uartsend_string("oled para error",sizeof("oled para error"));
    return;
  }
  x=127-X;
  y=63-Y;
  tmp=1<<(y%8);
  oledgram[x][y/8]|=tmp;
}
void oled_clear(void)  
{  
	u8 i,n;  
	for(i=0;i<8;i++)for(n=0;n<128;n++)oledgram[n][i]=0X00;  
	oled_refresh_gram();
}
void Internal_setting()
{
  oled_wrbytecmd(0xAE); //关闭显示
  oled_wrbytecmd(0xD5); //éè??ê±?ó·??μòò×ó,?eμ′?μ?ê
  oled_wrbytecmd(80);   //[3:0],·??μòò×ó;[7:4],?eμ′?μ?ê
  oled_wrbytecmd(0xA8); //éè???y?ˉ?·êy
  oled_wrbytecmd(0X3F); //??è?0X3F(1/64) 
  oled_wrbytecmd(0xD3); //éè????ê???ò?
  oled_wrbytecmd(0X00); //??è??a0

  oled_wrbytecmd(0x40); //设置显示开始行
                                                                                                      
  oled_wrbytecmd(0x8D); //μ?oé±?éè??
  oled_wrbytecmd(0x14); //bit2￡??a??/1?±?
  oled_wrbytecmd(0x20); //éè???ú′?μ??·?￡ê?
  oled_wrbytecmd(0x02); //[1:0],00￡?áDμ??·?￡ê?;01￡?DDμ??·?￡ê?;10,ò3μ??·?￡ê?;??è?10;
  oled_wrbytecmd(0xA1); //?????¨ò?éè??,bit0:0,0->0;1,0->127;
  oled_wrbytecmd(0xC0); //éè??COMé¨?è·??ò;bit3:0,??í¨?￡ê?;1,???¨ò??￡ê? COM[N-1]->COM0;N:?y?ˉ?·êy
  oled_wrbytecmd(0xDA); //éè??COMó2?tòy??????
  oled_wrbytecmd(0x12); //[5:4]????
           
  oled_wrbytecmd(0x81); //??±è?èéè??
  oled_wrbytecmd(0xEF); //1~255;??è?0X7F (áá?èéè??,??′ó??áá)
  oled_wrbytecmd(0xD9); //éè???¤3?μ??ü?ú
  oled_wrbytecmd(0xf1); //[3:0],PHASE 1;[7:4],PHASE 2;
  oled_wrbytecmd(0xDB); //éè??VCOMH μ??1±??ê
  oled_wrbytecmd(0x30); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

  oled_wrbytecmd(0xA4); //è?????ê??a??;bit0:1,?a??;0,1?±?;(°×?á/oú?á)
  oled_wrbytecmd(0xA6); //设置正常显示A6 翻转显示A7    						   
  oled_wrbytecmd(0xAF); //打开显示	 
  oled_clear();
}
void oled_drawword(u8 *addr,u8 wordwidth,u8 wordheight,u8 offsetX,u8 offsetY)
{
  u8 heightbyte=0;
  heightbyte=wordheight/8;
  for(int i=0;i<wordwidth;i++){
    for(int j=0;j<heightbyte;j++){
      for(int k=0;k<8;k++){
        if((addr[heightbyte*i+j]&(1<<k))!=0){
          oled_drawpoint(i+offsetX,j*8+7-k+offsetY);
        }
      }
    }
  }
}
void oled_test()
{
  /*for(int i=0;i<8;i++)
    for(int n=0;n<128;n++)
      oledgram[n][i]=0XFF;  */
 /* for(int i=0;i<64;i++)
    for(int j=0;j<32;j++)
       oled_drawpoint(i,j);*/
  oled_drawword(xiao,32,32,0,0);
  oled_drawword(hua,32,32,32,0);
  oled_drawword(bao,32,32,64,0);
  oled_drawword(shi,32,32,96,0);
  oled_drawword(nan,32,32,0,32);
  oled_drawword(hai,32,32,32,32);
  oled_drawword(nv,32,32,64,32);
  oled_drawword(hai,32,32,96,32);
  oled_refresh_gram();
  
  msleep(1000);
  oled_clear(); 
  msleep(500);
}
