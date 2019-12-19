#include "main.h"

void init_usart1(int rate)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitTypeDef GPIO_InitStructure; 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  // �����ж����ȼ�-�������������ж����ȼ�����
 /* NVIC_InitTypeDef NVIC_InitStrue;
  NVIC_InitStrue.NVIC_IRQChannel= USART1_IRQn;
  NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;//IRQ ͨ��ʹ��
  NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ� 1
  NVIC_InitStrue.NVIC_IRQChannelSubPriority=1;//�����ȼ� 1
  NVIC_Init(&NVIC_InitStrue);//�ж����ȼ���ʼ��
  */
  USART_InitTypeDef USART_InitStructure;
  USART_InitStructure.USART_BaudRate = rate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  
  USART_Init(USART1, &USART_InitStructure); 
  //ʹ�ܴ���1�ж�-������������ж�
  //USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//�����ж�
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
char uartrecv_char_forever(void)
{
  while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==RESET);
  return USART_ReceiveData(USART1);
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
//����Э��
//֡ͷ-֡��-֡����-����-У���-֡β ����0x55 0x06 0x01 0xFA 0x01 0xAA У���=֡��+֡����+���� �ݲ����ǳ���0xFFȡ����
//�ú�����ʹ���жϣ����԰���ָ��Э����գ�ȱ����ӵ���ȴ�
int usart_receive_frame() 
{ 
  int data=0;
  int Frame_start=0;
  int Frame_len=0;
  int FUNCTION=0;
  int Checksum=0;
  //int Frame_end=0;
  while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==0);
  Frame_start=USART1->DR;
  if(Frame_start!=0x55)
    return -1;
  while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==0);
    Frame_len=USART1->DR;
  
  switch(Frame_len)
  {
    case 6:
      while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==0);
      FUNCTION=USART1->DR;
      while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==0);
      data=USART1->DR;
      while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==0);
      Checksum=USART1->DR;
      break;
    case 7:
      while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==0);
      FUNCTION=USART1->DR;
      while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==0);
      data=USART1->DR;
      while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==0);
      Checksum=USART1->DR;
      break;
    default:
      break;
  }
  while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==0);
    Frame_len=USART1->DR;
    if(Frame_len!=0xAA)
      return -1;
    return data;
}

volatile u8 uartrecv_it_flag=0;

volatile int uartrecv_len=0;
int uartrecv_string_int(char *recvdata,int timeout_us)//�����ڹ涨ʱ���ڽ�����
{
  int i=0;
  int len=0;
  while(!uartrecv_it_flag);//ӵ���ȴ���һ���ֽ�
  while(i<timeout_us/10)
  {
    if(uartrecv_it_flag){
        uartrecv_it_flag=0;
        recvdata[len]= USART_ReceiveData(USART1); 
        len++;
        i=0;
    }
    else{
      delay_us(10);
      i++;
    }
  }
  return len;
}
// �жϷ�����
void USART1_IRQHandler(void)
{
   if(USART_GetITStatus(USART1,USART_IT_RXNE))
   {
     uartrecv_it_flag=1;
    }
    USART_ClearITPendingBit(USART1,USART_IT_RXNE);
}

