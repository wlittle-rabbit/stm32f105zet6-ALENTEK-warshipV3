/**
  ******************************************************************************
  * @file    test_gpio.h 
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
#ifndef __TEST_GPIO_H
#define __TEST_GPIO_H

#include "stm32f10x.h"


extern void RCC_Configuration_t(void);
extern void GPIO_Configuration_t(void);
extern void test_gpio(void);
extern void gpio_reset(void);

#endif


/*************************** (C) COPYRIGHT 2015 SAIC ********END OF FILE*******/
