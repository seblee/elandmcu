/**
 ****************************************************************************
 * @Warning :Without permission from the author,Not for commercial use
 * @File    :gpio.h
 * @Author  :Xiaowine
 * @date    :2017/10/20
 * @version :V 1.0.0
 *************************************************
 * @brief   :
 ****************************************************************************
**/
#ifndef __GPIO_H_
#define __GPIO_H_
#include "stm8l15x_conf.h"

#define LED_RED_PORT GPIOI
#define LED_GREEN_PORT GPIOI
#define LED_BLUE_PORT GPIOH
#define LED_BACK_PORT GPIOH

#define LED_RED_PIN GPIO_Pin_0
#define LED_GREEN_PIN GPIO_Pin_3
#define LED_BLUE_PIN GPIO_Pin_7
#define LED_BACK_PIN GPIO_Pin_6

void GPIO_Config(void);
/******__GPIO_H_******/
#endif
