/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_pwr.h"
//#include "usb_core.h"
//#include "usb_type.h"
#include <stdio.h>
//#define BEEP 1
//define TIME_DELAY
//#define LIGHT_SENSOR 1
//#define KEY 1
//#define SPI_FLASH 1
//#define TIM_INPUT_CAPTURE 1
//#define RED_CONTROL 1
//#define OLED 1
//#define LCD 1 
//#define I2C 1
//#define BLUETOOTH 1
#define USB 1
void RCC_Configuration_t(void)
{
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();
  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);
  /* Wait till HSE is ready */
  ErrorStatus HSEStartUpStatus_t;
  HSEStartUpStatus_t = RCC_WaitForHSEStartUp();
  if (HSEStartUpStatus_t == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    /* PCLK2 = HCLK/2 */
    RCC_PCLK2Config(RCC_HCLK_Div1);
    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);
    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
    /* Enable PLL */
    RCC_PLLCmd(ENABLE);
    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {}
    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    /* Wait till PLL is used as system clock source */
    while (RCC_GetSYSCLKSource() != 0x08)
    {}
  }

  /* Enable GPIO and devices clocks */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                         RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD |
                         RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF |   
                         RCC_APB2Periph_TIM1  | RCC_APB2Periph_USART1|  
                         RCC_APB2Periph_GPIOG | RCC_APB2Periph_ADC3  |
                         RCC_APB2Periph_AFIO  ,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3  | RCC_APB1Periph_SPI2  |
                         RCC_APB1Periph_TIM5  | RCC_APB1Periph_TIM4 |RCC_APB1Periph_I2C1,ENABLE);
}
void initled()
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  close_DS0();
  close_DS1();
}
void initkey()
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}
#ifdef LIGHT_SENSOR
void init_timer3_ms(int ms)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_TimeBaseStructure.TIM_Period = ms*10-1; //重载计数周期值   
  TIM_TimeBaseStructure.TIM_Prescaler = 7199;//fen ping xi shu分频系数  tim frequency10000hz  10000/10000=1s
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //时钟分频因子
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数
  TIM_TimeBaseInit(TIM3, & TIM_TimeBaseStructure);
}
void timer_delay()
{
  TIM_Cmd(TIM3, ENABLE);
  while(TIM_GetFlagStatus(TIM3, TIM_IT_Update) != SET);
  TIM_ClearFlag(TIM3, TIM_IT_Update);
  TIM_Cmd(TIM3, DISABLE);
}
#endif
void open_DS1()
{
  GPIO_ResetBits(GPIOE,GPIO_Pin_5);
}
void close_DS1()
{
  GPIO_SetBits(GPIOE,GPIO_Pin_5);
}
void open_DS0()
{
  GPIO_ResetBits(GPIOB,GPIO_Pin_5);
}
void close_DS0()
{
  GPIO_SetBits(GPIOB,GPIO_Pin_5);
}
void twinkle_led1()
{
    if(TIM_GetFlagStatus(TIM3, TIM_IT_Update) == SET){
      TIM_ClearFlag(TIM3, TIM_IT_Update);
      GPIO_SetBits(GPIOE,GPIO_Pin_5);
    }
    if(TIM_GetFlagStatus(TIM3, TIM_IT_Update) == SET){
      TIM_ClearFlag(TIM3, TIM_IT_Update);
      GPIO_ResetBits(GPIOE,GPIO_Pin_5);
    }
}

void init_lightsensor()
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE );	  //ê1?üADC3í¨μàê±?ó	
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE);//ADC?′??	
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE);//?′???áê?	    
  ADC_DeInit(ADC3);  //?′??ADC3,??íaéè ADC3μ?è?2???′??÷??éè?aè±ê??μ
  ADC_InitTypeDef ADC_InitStructure; 
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; 
  ADC_InitStructure.ADC_ScanConvMode = DISABLE; //单次转换
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; 
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; 
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 
  ADC_InitStructure.ADC_NbrOfChannel = 1; //顺序进行规则转换的数目
  ADC_Init(ADC3, &ADC_InitStructure);
  ADC_Cmd(ADC3, ENABLE);
  ADC_ResetCalibration(ADC3);	//复位校准
  while(ADC_GetResetCalibrationStatus(ADC3));	//复位校准结束
  ADC_StartCalibration(ADC3);	 //开始校准
  while(ADC_GetCalibrationStatus(ADC3));	 //校准结束
   ADC_Cmd(ADC3, DISABLE);
}
u16 get_adc3()
{
  ADC_RegularChannelConfig(ADC3, ADC_Channel_6, 1, ADC_SampleTime_28Cycles5);//一个序列，采样时间239.5周期
  //ADC_ClearFlag(ADC3, ADC_FLAG_EOC);
  ADC_SoftwareStartConvCmd(ADC3, ENABLE);		//使能软件启动
  while(ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC)==RESET);//等待转换结束
  //ADC_SoftwareStartConvCmd(ADC3, ENABLE);
  return ADC_GetConversionValue(ADC3);	//返回最后一次转换结果
}

 int main(void)
{
  RCC_Configuration_t();
  initled();
  init_usart1(115200);
  delay_init();
#ifdef TIME_DELAY
  init_timer3_ms(100);
#endif
  
#ifdef LIGHT_SENSOR
  u16 light=0;
  int percent=100;
  char s[8];
  int len=0;
  init_lightsensor();
  ADC_Cmd(ADC3, ENABLE);
#endif
  
#ifdef BEEP
  initbeep();
#endif
  
#ifdef KEY
  initkey();
#endif
  
#ifdef SPI_FLASH
  u8 readdata[10];
  u8 writedata[10];
  init_flash();
#endif
  
#ifdef TIM_INPUT_CAPTURE
  init_inputcapture();
  reset_tim5count();
#endif
  
#ifdef  RED_CONTROL
  init_redcontrol();
#endif
  
#ifdef OLED
  init_oled();
#endif
  
#ifdef LCD
  LCD_Init();
#endif
  
#ifdef I2C
  at24cxx_i2c_init();
#endif
  
#ifdef BLUETOOTH
  init_bluetooth(115200);
#endif
  
#ifdef USB
  u16 t;
  u16 times=0;    
  u8 usbstatus=0;
  u16 len_usb;	
  
  delay_ms(1800);
  LCD_Init();
  USB_Port_Set(0); 	//USB断开
  delay_ms(700);
  USB_Port_Set(1);	//USB连接
  Set_USBClock(); 
  USB_Interrupts_Config();
  USB_Init();
  
#endif
  while(1)
  {
#ifdef BEEP
    soft_pwm(80,100,1000,10); 
#elif LIGHT_SENSOR
    
    light=get_adc3();
    percent=light*100/65536;
    len=u16_to_string(percent,s);
    uartsend_string(s,len);
    uartsend_string("\r\n",sizeof("\r\n"));
    msleep(500);
#elif KEY    
    if(GPIO_ReadInputDataBit(GPIOE,KEY2)==Bit_RESET){
      uartsend_string("key2 press down",sizeof("key2 press down"));
      uartsend_string("\r\n",sizeof("\r\n"));
      msleep(100);
    }
    if(GPIO_ReadInputDataBit(GPIOA,KEY_UP)==Bit_SET){
      uartsend_string("KEY_UP press up",sizeof("KEY_UP press up"));
      uartsend_string("\r\n",sizeof("\r\n"));
      msleep(100);
    }
#elif SPI_FLASH
   
    for(int i=0;i<10;i++){
      writedata[i]=i+16;
    }
    W25_sectorerase(0x000000);
    W25_read(readdata,256,10);
    uartsend_string((char*)readdata,10);
    uartsend_string("\r\n",sizeof("\r\n"));
    W25_write_page(writedata,256,10);
    W25_read(readdata,256,10);
    uartsend_string((char*)readdata,10);
    uartsend_string("\r\n",sizeof("\r\n"));
    msleep(500);
    GPIO_SetBits(GPIOE,GPIO_Pin_5);
    msleep(500);
    GPIO_ResetBits(GPIOE,GPIO_Pin_5);
#elif  TIM_INPUT_CAPTURE
    /*if(get_tim5count()>1000){
      uartsend_string("1s",sizeof("1s"));
      uartsend_string("\r\n",sizeof("\r\n"));
      reset_tim5count();
    }*/
#elif RED_CONTROL  
    msleep(2000);
    scan_redkey();
    //uartsend_string("wait red_control",sizeof("wait red_control"));
    //uartsend_string("\r\n",sizeof("\r\n"));
    //redcontrol_read_command();
     //uartsend_char('I');
#elif OLED
    oled_test();
    /*open_DS0();
    msleep(500);
    close_DS0();
    msleep(500);*/
#elif LCD
    LCD_ShowString(30,70,200,16,16,"TFTLCD TEST");
    msleep(1000);
#elif I2C 
    char aa;
    i2c_write(0x10,0x30);
    delay_us(1000);
    while(1){
      aa=i2c_read(0x10);
      uartsend_string("i2c test\r\n",sizeof("i2c test\r\n"));
      uartsend_char(aa);
      uartsend_string("\r\n",sizeof("\r\n"));
      delay_us(500000);
      delay_us(500000);
    }
#elif BLUETOOTH
   
#elif USB
    if(usbstatus!=bDeviceState)//USB连接状态发生变化.
    {
            usbstatus=bDeviceState;//记录新的状态
            if(usbstatus==CONFIGURED)
            {
                    LCD_ShowString(30,130,200,16,16,"USB Connected    ");//ìáê?USBá??ó3é1|
                    open_DS1();
            }else
            {
                    LCD_ShowString(30,130,200,16,16,"USB disConnected");//ìáê?USB???a
                    close_DS1();
            }
    }
    if(USB_USART_RX_STA&0x8000){					   
            len_usb=USB_USART_RX_STA&0x3FFF;//得到此次接收到的数据长度
            usb_printf("\r\n你发送的消息为:%d\r\n\r\n",len_usb);
            for(t=0;t<len_usb;t++)
            {
                    USB_USART_SendData(USB_USART_RX_BUF[t]);//以字节方式，发送给USB 
            }
            usb_printf("\r\n\r\n");//插入换行
            USB_USART_RX_STA=0;
    }
    else{
            times++;
            if(times%5000==0)
            {
                    usb_printf("\r\nSTM32 USB串口实验\r\n");
                    usb_printf("开发板ALIENTEK\r\n\r\n");
            }
            if(times%200==0)usb_printf("输入数据，以回车结束\r\n");    
            delay_ms(10);   
    }
	
#else
      open_DS1();
      delay_us(500000);//msleep(200);
      uartsend_string("recv str is\r\n",sizeof("recv str is\r\n")-1);
      close_DS1();
      delay_us(500000);//msleep(200);  
      /*
       int str_len=0;
    delay_us(1000000);
    char uartrecv_data[128]={0};
     str_len=uartrecv_string_int(uartrecv_data, 500);
    uartsend_string("recv str is\r\n",sizeof("recv str is\r\n")-1);
    uartsend_string(uartrecv_data,strlen(uartrecv_data));
      */ 
#endif
  }
}

