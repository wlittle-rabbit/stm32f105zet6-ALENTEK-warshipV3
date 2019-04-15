#include "main.h"

//逻辑1应该是560us低+1680us高，逻辑0应该是560us低+560us高
//NEC遥控指令的数据格式为：同步码头、地址码、地址反码、控制码、控制反码。
//同步码由一个9ms的低电平和一个4.5ms的高电平组成，地址码、地址反码、控制码、控制反码均是8位数据格式。
//按照低位在前，高位在后的顺序发送。

//尝试过不在中断处理，但发现总是不能正常捕获。
//static u8 TIM4_RED_CAP_FLAG;
//static u8 TIM4_RED_IT_FLAG;
//static int tim4_IT_Update_count;
static u8 startbit_flag;
static u8 lowup_once_flag;
static u32 red_content;
static int red_cnt;
//static int capnum;
static u8 addr_code;
static u8 control_code;
static u8 get_keyflag;
void init_redcontrol()
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 ,ENABLE);

  GPIO_InitTypeDef GPIO_InitStructure; 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_SetBits(GPIOB,GPIO_Pin_9);
  
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;   
  TIM_TimeBaseStructure.TIM_Period = 20000; //最大20ms溢出
  TIM_TimeBaseStructure.TIM_Prescaler = (72-1); //f/(PSC[15:0]+1)  计数器1us加1
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM_CounterMode_Up从0计数到TIM_Period   TIM_CounterMode_Down从TIM_Period减到0
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
  
  TIM_ICInitTypeDef TIM_ICInitStructure;
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_OCPolarity_High;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//每探测到1个边沿执行一次
  TIM_ICInitStructure.TIM_ICFilter = 0x03;//配置输入滤波数，8个定时器周期滤波
  TIM_ICInit(TIM4, &TIM_ICInitStructure);
  
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  TIM_Cmd(TIM4,ENABLE);
  TIM_ITConfig(TIM4,TIM_IT_CC4|TIM_IT_Update,ENABLE); 
  close_DS1();
}
void TIM4_IRQHandler()
{
  //int len=0;
  //char s_byte[8];
  int high_num=0;
  if((TIM_GetITStatus(TIM4,TIM_IT_CC4))!=RESET){
    if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)==Bit_SET){//上升沿
      open_DS1();
      TIM_OC4PolarityConfig(TIM4,TIM_OCPolarity_Low);
      TIM_SetCounter(TIM4,0);
      lowup_once_flag=1;
    }
    else{  //下降沿
      close_DS1();
      high_num=TIM_GetCapture4(TIM4);
      TIM_OC4PolarityConfig(TIM4,TIM_OCPolarity_High);
      if(lowup_once_flag){
        if(startbit_flag){
          if(high_num>300&&high_num<800){
            red_content<<=1;
            red_content|=0;
          }
          else if(high_num>1400&&high_num<1800){
            red_content<<=1;
            red_content|=1;
          }
          else if(high_num>2200&&high_num<2800){
            red_cnt++;
            get_keyflag=1;
          }
          
        }
        else{
          if(high_num>4200&&high_num<4800){
            startbit_flag=1;
          }
        }
      }
      lowup_once_flag=0;
    }
   //capnum++;
  }
  if((TIM_GetITStatus(TIM4,TIM_IT_Update))!=RESET){
   // tim4_IT_Update_count++; 
    //if(tim4_IT_Update_count>300){
    //  tim4_IT_Update_count=0;
      //len=u16_to_string(capnum,s_byte);
      //uartsend_string(s_byte,len);
     // uartsend_string("capnum\r\n",sizeof("capnum\r\n"));
    //}
  }
  TIM_ClearITPendingBit(TIM4, TIM_IT_CC4|TIM_IT_Update);
}

void scan_redkey()
{
  int len=0;
  char s_byte[8];
  if(get_keyflag==1){
    get_keyflag=0;
    addr_code=red_content>>24;
    control_code=red_content>>8;
    len=u16_to_string(addr_code,s_byte);
    uartsend_string(s_byte,len);
    uartsend_string(" is addr_code\r\n",sizeof(" is addr_code\r\n"));
    len=u16_to_string(control_code,s_byte);
    uartsend_string(s_byte,len);
    uartsend_string(" is control_code\r\n",sizeof(" is control_code\r\n"));
  }
  else{
    uartsend_string("no redkey now\r\n",sizeof("no redkey now\r\n"));
  }
}
/*
int read_start_bit()//同步码
{
  int high_num=0;
  int len=0;
  char s_byte[8];
  
  //while(TIM4_RED_CAP_FLAG!=1);
  //TIM4_RED_CAP_FLAG=0;
  while(TIM_GetFlagStatus(TIM4,TIM_FLAG_CC4)==RESET);
  TIM_ClearFlag(TIM4,TIM_FLAG_CC4);
  
  

  while(TIM_GetFlagStatus(TIM4,TIM_FLAG_CC4)==RESET);
  TIM_ClearFlag(TIM4,TIM_FLAG_CC4);
  
  if((high_num<7000)&&(high_num>2500)){//if((low_num<13000)&&(low_num>7000)&&(high_num<7000)&&(high_num>2500)){
    //uartsend_string("get start_bit",sizeof("get start_bit"));
    //uartsend_string("\r\n",sizeof("\r\n"));
    return 1;
  }
  else{
    uartsend_string("error read_start_bit",sizeof("error read_start_bit"));
    uartsend_string("\r\n",sizeof("\r\n"));
    len=u16_to_string(high_num,s_byte);
    uartsend_string(s_byte,len);
    uartsend_string("\r\n",sizeof("\r\n"));
    
    return -1;
  }
}
int read_bit()
{
  int bit=0;
  int high_num=0;
  int len=0;
  char s_byte[8];
  //char 
  //等待到上升沿标志，设置下次捕获为下降沿，计数器设置为0
  //等待下降沿标志，清楚标志位，获取计数
  while(TIM_GetFlagStatus(TIM4,TIM_FLAG_CC4)==RESET);
  TIM_ClearFlag(TIM4,TIM_FLAG_CC4);
  TIM_OC4PolarityConfig(TIM4,TIM_OCPolarity_Low);
  TIM_SetCounter(TIM4,0);
  
  while(TIM_GetFlagStatus(TIM4,TIM_FLAG_CC4)==RESET);
  TIM_ClearFlag(TIM4,TIM_FLAG_CC4);
  high_num=TIM_GetCapture4(TIM4);
  TIM_OC4PolarityConfig(TIM4,TIM_OCPolarity_High);
 
  if((high_num>1300)&&(high_num<1980)){
    bit=1;
    uartsend_char('I');
    
  }
  else if((high_num>300)&&(high_num<800)){
    bit=0;
    uartsend_char('O');
  }
  else{
    bit=-1;
    len=u16_to_string(high_num,s_byte);
    uartsend_string(s_byte,len);
    uartsend_string("high_num error\r\n",sizeof("high_num error\r\n"));
    len=u16_to_string(capnum,s_byte);
    uartsend_string(s_byte,len);
    uartsend_string("bit error\r\n",sizeof("bit error\r\n"));
  }
  return bit;
}
int read_byte()
{
  int byte=0;
  int bit=0; 
  for(int i=0;i<8;i++){
    bit=read_bit();
    if(bit==-1){
      return -1;
    }
    byte=byte|(bit>>i);
  }
  return byte;
}
int redcontrol_read_command()
{
  int addr_code=0;
  int addr_oppcode=0;
  int control_code=0;
  int control_oppcode=0;
  char s_byte[8];
  int len=0;
  if(read_start_bit()!=1){
    //uartsend_string("read_start_bit error",sizeof("read_start_bit error"));
    //uartsend_string("\r\n",sizeof("\r\n"));
    return -1;
  }
  addr_code=read_byte();
  if(addr_code==-1){
    uartsend_string("addr_code error",sizeof("addr_code error"));
    uartsend_string("\r\n",sizeof("\r\n"));
    return -1;
  }
  addr_oppcode=read_byte();
  if(addr_oppcode==-1){
    uartsend_string("addr_oppcode error",sizeof("addr_oppcode error"));
    uartsend_string("\r\n",sizeof("\r\n"));
    return -1;
  }
  control_code=read_byte();
  if(control_code==-1){
    uartsend_string("control_code error",sizeof("control_code error"));
    uartsend_string("\r\n",sizeof("\r\n"));
    return -1;
  }
  control_oppcode=read_byte();
  if(control_oppcode==-1){
    uartsend_string("control_oppcode error",sizeof("control_oppcode error"));
    uartsend_string("\r\n",sizeof("\r\n"));
    return -1;
  }
  len=u16_to_string(addr_code,s_byte);
  uartsend_string(s_byte,len);
  uartsend_string("\r\n",sizeof("\r\n"));
  len=u16_to_string(addr_oppcode,s_byte);
  uartsend_string(s_byte,len);
  uartsend_string("\r\n",sizeof("\r\n"));
  len=u16_to_string(control_code,s_byte);
  uartsend_string(s_byte,len);
  uartsend_string("\r\n",sizeof("\r\n"));
  len=u16_to_string(control_oppcode,s_byte);
  uartsend_string(s_byte,len);
  uartsend_string("\r\n",sizeof("\r\n"));
  return 1;
}
*/

/*
void TIM4_IRQHandler()
{
  int len=0;
  char s_byte[8];
  if((TIM_GetITStatus(TIM4,TIM_IT_CC4))!=RESET){
    //TIM4_RED_CAP_FLAG=1;
    if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)==Bit_SET){//上升沿
      open_DS1();
    }
    else{  //下降沿
      close_DS1();
    }
   capnum++;
  }
  if((TIM_GetITStatus(TIM4,TIM_IT_Update))!=RESET){
    tim4_IT_Update_count++; 
    if(tim4_IT_Update_count>300){
      tim4_IT_Update_count=0;
      //uartsend_string("TIM_IT_Update",sizeof("TIM_IT_Update"));
      //uartsend_string("\r\n",sizeof("\r\n"));
      len=u16_to_string(capnum,s_byte);
      uartsend_string(s_byte,len);
      uartsend_string("capnum\r\n",sizeof("capnum\r\n"));
    }
    //TIM4_RED_IT_FLAG=1;
  }
  TIM_ClearITPendingBit(TIM4, TIM_IT_CC4|TIM_IT_Update);
}
*/

