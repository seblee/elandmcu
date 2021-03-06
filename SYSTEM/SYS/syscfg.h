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

/** #if define **/
#define LCD_All_angle_view

#define RTC_LSE
#define CLK_SUE_HSI
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern int Firmware_Version_Major;
extern int Firmware_Version_Minor;
extern RST_FLAG_TypeDef rst_flag;
/* Private function prototypes -----------------------------------------------*/
void SysClock_Init(void);

/* Private functions ---------------------------------------------------------*/

#endif /*__SYSCFG_H_*/
