/**
 ****************************************************************************
 * @Warning :Without permission from the author,Not for commercial use
 * @File    :syscfg.h
 * @Author  :Xiaowine
 * @date    :2017/10/23
 * @version :V 1.0.0
 *************************************************
 * @brief   :
 ****************************************************************************
**/
#ifndef __SYSCFG_H_
#define __SYSCFG_H_
/* Private include -----------------------------------------------------------*/
#include "stm8l15x.h"
#include <string.h>
#include <stdio.h>
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define RTC_LSE
#define CLK_SUE_HSI

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void SysClock_Init(void);

/* Private functions ---------------------------------------------------------*/

#endif /*__SYSCFG_H_*/
