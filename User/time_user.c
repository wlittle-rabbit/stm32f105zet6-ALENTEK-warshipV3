#include "main.h"

void msleep(int num)
{
  for(int i=0;i<num;i++){
    for(int j=0;j<0x21FF;j++);
  }
}

/*
systick��һ��24λ��������ʱ����������0��STK_LOAD�Ĵ����Զ����س�ֵ��
ֻҪʹ��λ��������Ͳ�ͣϢ��

*/
void delay_init()
{
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);//ʱ��Դ72M/8
  
}

void delay_us(u32 nus)
{
  u32 temp;
  SysTick->LOAD=9*nus;//��������ֵ��������1��0x00FFFFFF
  SysTick->VAL=0x00; //��ռ�����
  SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;//��ʼ����		  
  do
  {
    temp=SysTick->CTRL;
  }while((temp&0x01)&&!(temp&(1<<16)));	// �ȴ�ʱ�䵽�� 
  SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;//�رռ�����
  SysTick->VAL =0X00;   
}
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*9000;				//����???��??(SysTick->LOAD?a24bit)
	SysTick->VAL =0x00;							//??????��y?��
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//?a��?��1��y  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//�̨���y����??��?��?   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//1?��???��y?��
	SysTick->VAL =0X00;       					//??????��y?��	  	    
} 