#include "main.h"


//²úÆ·ÌÔ±¦ÍøÖ·https://item.taobao.com/item.htm?spm=a1z09.2.0.0.64042e8dROjFdq&id=543235124257&_u=r1n1vecqd572
void set_ble_baud(int baud)
{
  char baudbuf[10]={0};
  int len=u16_to_string(baud,baudbuf);
  uartsend_string("AT+UART=",sizeof("AT+UART=")-1);
  uartsend_string(baudbuf,len);
  //uartsend_string(">",sizeof("<"));
}

void init_bluetooth(int baud)
{
  init_usart1(baud);
  set_ble_baud(baud);
}

void send_data_blue(char *str)
{
  uartsend_string(str,strlen(str));
}
void recv_data_blue()
{
  uint16_t data=0;
  USART_GetFlagStatus(USART1, USART_FLAG_RXNE);
  data=USART_ReceiveData(USART1);
}