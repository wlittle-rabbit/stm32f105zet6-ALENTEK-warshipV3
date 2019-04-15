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
char int_to_char(int a)
{
  return a+'0';
}
int get_data_len(u16 a)
{
  int len=0;
  if(a/10000>0){
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
