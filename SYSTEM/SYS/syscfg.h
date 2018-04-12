/**
 ****************************************************************************
 * @Warning :Without permission from the author,Not for commercial use
 * @File    :syscfg.h
 * @Author  :seblee
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
#include <stdlib.h>
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define MCU_VERSION_MAJOR (uint8_t)1
#define MCU_VERSION_MINOR (uint8_t)11
#define RTC_LSE
#define CLK_SUE_HSI
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern int Firmware_Version_Major;
extern int Firmware_Version_Minor;
/* Private function prototypes -----------------------------------------------*/
void SysClock_Init(void);

/* Private functions ---------------------------------------------------------*/

#endif /*__SYSCFG_H_*/
