#include "main.h"

void init_usart1(int rate)
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  USART_InitTypeDef USART_InitStructure;
  USART_InitStructure.USART_BaudRate = rate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  USART_Init(USART1, &USART_InitStructure);
  USART_Cmd(USART1, ENABLE);
  
  //ʹ�ܴ���1�ж�-������������ж�
  /*USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//�����ж�
  // �����ж����ȼ�-�������������ж����ȼ�����
  NVIC_InitTypeDef NVIC_InitStrue;
  NVIC_InitStrue.NVIC_IRQChannel= USART1_IRQn;
  NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;//IRQ ͨ��ʹ��
  NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ� 1
  NVIC_InitStrue.NVIC_IRQChannelSubPriority=1;//�����ȼ� 1
  NVIC_Init(&NVIC_InitStrue);//�ж����ȼ���ʼ��
  */
}
void uartsend_string(char *p,int n)
{
  for(int i=0;i<n;i++){
    USART_SendData(USART1,*p++);
    while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)!=SET);
  }
}
void uartsend_char(char a)
{
  USART_SendData(USART1,a);
  while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)!=SET);
}
char uartrecv_char_forever(void)
{
  while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==RESET);
  return USART_ReceiveData(USART1);
}
void uartrecv_string_forever(char *p)
{
  int i=0;
  while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==RESET);
  p[0]=USART_ReceiveData(USART1);
  USART_ClearFlag(USART1,USART_FLAG_RXNE);
  while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==SET)
  {
    i++;
    p[i]=USART_ReceiveData(USART1);
    USART_ClearFlag(USART1,USART_FLAG_RXNE);
  }
}
char int_to_char(int a)
{
  return a+'0';
}
int get_data_len(int a)
{
  int len=0;
 if(a/100000>0){
    len=6; 
  }
  else if(a/10000>0){
    len=5; 
  }
  else if(a/1000>0){
    len=4;
  }
  else if(a/100>0){
    len=3;
  }
  else if(a/10>0){
    len=2;
  }
  else{
    len=1;
  }
  return len;
}
int u16_to_string(int a,char *p)
{
  int len=0;
  int data[8];
  int temp=0;

  len=get_data_len(a);
  temp=a;
  for(int i=0;i<len;i++){
    data[i]=temp%10;
    temp=temp/10;
  }
  for(int i=0;i<len;i++){
    *(p+i)=int_to_char(data[len-i-1]);
  }
  return len;
}
 
#ifdef BLUETOOTH
// �жϷ�����
void USART1_IRQHandler(void)
{
    u8 res;
    if(USART_GetITStatus(USART1,USART_IT_RXNE))// ���յ�����
    {
        res= USART_ReceiveData(USART1); // ��ô���1���յ�������
	if(res>0x40)
         open_DS1();
        else
         close_DS1();
    }
    USART_ClearITPendingBit(USART1,USART_IT_RXNE);
}
#endif