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
void delay_ms(uint16_t nms)
{	 		  	  
	uint32_t temp;		   
	SysTick->LOAD=(uint32_t)nms*9000;				//����???��??(SysTick->LOAD?a24bit)
	SysTick->VAL =0x00;							//??????��y?��
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//?a��?��1��y  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//�̨���y����??��?��?   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//1?��???��y?��
	SysTick->VAL =0X00;       					//??????��y?��	  	    
}
/*void delay_ms(uint16_t nms)
{
  int i=0;
  for(i=0;i<nms;i++)
    delay_us(1000);
}*/