/**
  ******************************************************************************
  * @file    main.h 
  * @author  ShenWenting
  * @version V1.0
  * @date    17-Aug-2015
  * @brief   Initialize program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2015 SAIC</center></h2>
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
#include<string.h>
#include "stm32f10x.h"
#define KEY0 GPIO_Pin_4
#define KEY1 GPIO_Pin_3
#define KEY2 GPIO_Pin_2
#define KEY_UP GPIO_Pin_0
    
#define W25_ERASE_SECTOR_INSTRUC     0x20
#define W25_WRITE_ENABLE_INSTRUC     0x06
#define W25_WRITE_DISENABLE_INSTRUC  0x04
#define W25_READ_DATA_INSTRUC        0x03
#define W25_WRITE_PAGE_INSTRUC       0x02
extern void open_DS1();
extern void close_DS1();
extern void open_DS0();
extern void close_DS0();
extern u8 spi2_writeread(u8 data);
extern void init_flash();
extern void enable_w25();
extern void disenable_w25();
extern u8 W25_readSR();
extern void W25_wait_busy();
extern void W25_writenable();
extern void W25_write_page(u8 *buffer,u32 start_address,u32 write_num);
extern void W25_sectorerase(u32 erase_address);
extern void W25_read(u8 *buffer,u32 start_address,u32 read_num);
extern void init_inputcapture();
extern void init_usart1(int rate);
extern char int_to_char(int a);
extern void uartsend_string(char *p,int n);
extern void uartsend_char(char a);
extern void msleep(int num);
extern void initbeep();
extern void soft_pwm();
extern void reset_tim5count();
extern u32 get_tim5count();
extern int u16_to_string(int a,char *p);
extern void init_redcontrol();
extern int redcontrol_read_command();
extern int read_start_bit();
extern void init_oled();
extern void oled_test();
extern void init_lcd();
extern void lcd_test();
extern void at24cxx_i2c_init();
extern void delay_init();
extern void  delay_us(u32 nus);
extern void i2c_write(u8 addr,u8 data);
extern u8 i2c_read(u8 nAddr);
extern void set_ble_baud(int baud);
extern void init_bluetooth(int baud);
extern void send_data_blue(char *str);
extern char uartrecv_char(void);
extern void uartrecv_string_forever(char *p);
extern int usart_receive_frame(); 
extern int uartrecv_string_int(char *recvdata,int timeout_us);
extern __IO uint32_t bDeviceState;
extern u8 USB_USART_RX_BUF[USB_USART_REC_LEN]; 
extern u16 USB_USART_RX_STA; 
//extern CONFIGURED;
//extern enum _DEVICE_STATE;
extern void usb_printf(char* fmt,...);  
extern void LCD_Init(void);
extern void delay_ms(u16 nms);
extern void USB_Port_Set(u8 enable);
extern void Set_USBClock(void);
extern void USB_Interrupts_Config(void);
extern void USB_Init(void);

#endif
