#include "main.h"

void msleep(int num)
{
  for(int i=0;i<num;i++){
    for(int j=0;j<0x21FF;j++);
  }
}

/*
systick是一个24位倒计数定时器，计数到0，STK_LOAD寄存器自动重载初值，
只要使能位不清除，就不停息。

*/
void delay_init()
{
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);//时钟源72M/8
  
}

void delay_us(u32 nus)
{
  u32 temp;
  SysTick->LOAD=9*nus;//设置重载值，必须在1和0x00FFFFFF
  SysTick->VAL=0x00; //清空计数器
  SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;//开始倒数		  
  do
  {
    temp=SysTick->CTRL;
  }while((temp&0x01)&&!(temp&(1<<16)));	// 等待时间到达 
  SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;//关闭计数器
  SysTick->VAL =0X00;   
}
void delay_ms(uint16_t nms)
{	 		  	  
	uint32_t temp;		   
	SysTick->LOAD=(uint32_t)nms*9000;				//ê±???ó??(SysTick->LOAD?a24bit)
	SysTick->VAL =0x00;							//??????êy?÷
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//?aê?μ1êy  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//μè′yê±??μ?′?   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//1?±???êy?÷
	SysTick->VAL =0X00;       					//??????êy?÷	  	    
}
/*void delay_ms(uint16_t nms)
{
  int i=0;
  for(i=0;i<nms;i++)
    delay_us(1000);
}*/