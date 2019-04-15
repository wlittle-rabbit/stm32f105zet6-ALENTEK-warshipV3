#include "main.h"

void initbeep()
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_ResetBits(GPIOB,GPIO_Pin_8);
}
void openbeep()
{
  GPIO_SetBits(GPIOB,GPIO_Pin_8);
}
void closebeep()
{
  GPIO_ResetBits(GPIOB,GPIO_Pin_8);
}
void soft_pwm(int percent,int frequency,int T_onbeep_ms,int T_offbeep_ms)
{
  int time_ms_highlevel=0;
  int time_ms_lowlevel=0;
  int num=0;
  time_ms_highlevel=10*percent/frequency;    //1000*percent/100/frequency;
  time_ms_lowlevel=10*(100-percent)/frequency;
  num=T_onbeep_ms*frequency/1000; //T_onbeep_ms/(1000/frequency);
  for(int i=0;i<num;i++){
    openbeep();
    msleep(time_ms_highlevel);
    closebeep();
    msleep(time_ms_lowlevel);
  }
  msleep(T_offbeep_ms);
}