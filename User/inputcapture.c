#include "main.h"

static u8 FLAG_CC1;
static int tim5_count;
static char scap[8];
static int clen;
//Tout= ((arr+1)*(psc+1))/Tclk
void init_inputcapture()
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //timer5 1000us
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;   
  TIM_TimeBaseStructure.TIM_Period = 999;
  TIM_TimeBaseStructure.TIM_Prescaler = 72-1; //f/(PSC[15:0]+1)
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM_CounterMode_Up从0计数到TIM_Period   TIM_CounterMode_Down从TIM_Period减到0
  TIM_TimeBaseInit(TIM5, & TIM_TimeBaseStructure);
  
  TIM_ICInitTypeDef TIM_ICInitStructure;
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//每探测到1个边沿执行一次
  TIM_ICInitStructure.TIM_ICFilter = 0x0;//配置输入滤波数，不滤波
  TIM_ICInit(TIM5, &TIM_ICInitStructure);
  
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  TIM_ITConfig(TIM5,TIM_IT_CC1|TIM_IT_Update,ENABLE); 
  TIM_Cmd(TIM5,ENABLE);
}

void TIM5_IRQHandler()
{
  if(TIM_GetITStatus(TIM5,TIM_IT_CC1)!=RESET){
    if(FLAG_CC1!=1){
      FLAG_CC1=1;
      open_DS1();
      TIM_SetCounter(TIM5,0);
      tim5_count=0;
      TIM_OC1PolarityConfig(TIM5,TIM_OCPolarity_Low);
    }
    else{
      FLAG_CC1=0;
      close_DS1();
      
      clen=u16_to_string(tim5_count,scap);
      uartsend_string(scap,clen);
      uartsend_string("*100us",sizeof("*100us"));
      uartsend_string("\r\n",sizeof("\r\n"));
      TIM_OC1PolarityConfig(TIM5,TIM_OCPolarity_High);
    }
    TIM_ClearITPendingBit(TIM5, TIM_IT_CC1);
  }
  else if(TIM_GetITStatus(TIM5,TIM_IT_Update)!=RESET){
    tim5_count++;
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
  }
}

/*static u8 tmp_flag;
void TIM5_IRQHandler()
{
  if(TIM_GetITStatus(TIM5,TIM_IT_CC1)!=RESET){
    if(FLAG_CC1!=1){
      FLAG_CC1=1;
      open_DS1();
      TIM_OC1PolarityConfig(TIM5,TIM_OCPolarity_Low);
    }
    else{
      FLAG_CC1=0;
      close_DS1();
      TIM_OC1PolarityConfig(TIM5,TIM_OCPolarity_High);
    }
    uartsend_string("cap",sizeof("cap"));
    uartsend_string("\r\n",sizeof("\r\n"));
    TIM_ClearITPendingBit(TIM5, TIM_IT_CC1);
  }
  else if(TIM_GetITStatus(TIM5,TIM_IT_Update)!=RESET){
    tim5_count++;
    if(tim5_count>50000){
      if(tmp_flag!=1){
        tmp_flag=1;
        open_DS0();
      }
      else{
        tmp_flag=0;
        close_DS0();
      }
      tim5_count=0; 
    }
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
  }
  //TIM_SetCounter(TIM5,0);
}*/

u32 get_tim5count()
{
  return tim5_count;
}
void reset_tim5count()
{
  tim5_count=0;
}