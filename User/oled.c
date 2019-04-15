#include "main.h"
#include "xiaohuabao.h"
//ÿ�γ�ʼ��ǰ��Ҫ��λһ�£�RST���͸�λ
//OLED���ӷ�ʽΪ8080���ڷ�ʽ
//�����������£��Ȳ���DC��������csƬѡ��Ȼ������WR����RD����RD����WR�����ض�ȡ����д��
//��ȡ����ʱ�������ĵ�һ���ֽ���֮��Ҫ���ڶ�������������Ҫ�ĵ�һ������
//�Դ�Ϊ128*64bit����Ϊ8ҳ��ÿҳ128���ֽ�
//Ϊ��ֹд������ݸ���֮ǰ��״̬�������ڵ�Ƭ��SRAM�ﱣ��һ��128*8�ֽڣ��޸����һ����д�뵽oled��GRAM

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
  //PD3 DC���������ݱ�־��0��д���1��д����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  //PG13 RD,PG15 RST��PG14 WR��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15; 
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  //PD6 CS�� 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  //���´������Ҫ����ʼ��ʱһ��Ҫȷ���ó�ʼ���������״̬����������״̬��ȷ���������������Ͷ���û����ɣ�ʹ�ó�ʼ��ʧ�ܡ�
  GPIO_SetBits(GPIOD,GPIO_Pin_3|GPIO_Pin_6);
  GPIO_SetBits(GPIOC,0xFF);
  GPIO_SetBits(GPIOG,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
  
  RST_oled();
   //Internal_setting();
  oled_elecopen();
  oled_wrbytecmd(0x20);//ҳ��ַģʽ ��2 �е�ַģʽ��0 �е�ַģʽ��1
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
  oled_wrbytecmd(0xAE); //�ر���ʾ
  oled_wrbytecmd(0xD5); //����??����?����??�̨�������,?e�̡�?��?��
  oled_wrbytecmd(80);   //[3:0],��??�̨�������;[7:4],?e�̡�?��?��
  oled_wrbytecmd(0xA8); //����???y?��?����y
  oled_wrbytecmd(0X3F); //??��?0X3F(1/64) 
  oled_wrbytecmd(0xD3); //����????��???��?
  oled_wrbytecmd(0X00); //??��??a0

  oled_wrbytecmd(0x40); //������ʾ��ʼ��
                                                                                                      
  oled_wrbytecmd(0x8D); //��?o����?����??
  oled_wrbytecmd(0x14); //bit2��??a??/1?��?
  oled_wrbytecmd(0x20); //����???����?��??��?�꨺?
  oled_wrbytecmd(0x02); //[1:0],00��?��D��??��?�꨺?;01��?DD��??��?�꨺?;10,��3��??��?�꨺?;??��?10;
  oled_wrbytecmd(0xA1); //?????����?����??,bit0:0,0->0;1,0->127;
  oled_wrbytecmd(0xC0); //����??COM����?����??��;bit3:0,??����?�꨺?;1,???����??�꨺? COM[N-1]->COM0;N:?y?��?����y
  oled_wrbytecmd(0xDA); //����??COM��2?t��y??????
  oled_wrbytecmd(0x12); //[5:4]????
           
  oled_wrbytecmd(0x81); //??����?������??
  oled_wrbytecmd(0xEF); //1~255;??��?0X7F (����?������??,??�䨮??����)
  oled_wrbytecmd(0xD9); //����???��3?��??��?��
  oled_wrbytecmd(0xf1); //[3:0],PHASE 1;[7:4],PHASE 2;
  oled_wrbytecmd(0xDB); //����??VCOMH ��??1��??��
  oled_wrbytecmd(0x30); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

  oled_wrbytecmd(0xA4); //��?????��??a??;bit0:1,?a??;0,1?��?;(���?��/o��?��)
  oled_wrbytecmd(0xA6); //����������ʾA6 ��ת��ʾA7    						   
  oled_wrbytecmd(0xAF); //����ʾ	 
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
