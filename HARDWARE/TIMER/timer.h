/**
 ****************************************************************************
 * @Warning :Without permission from the author,Not for commercial use
 * @File    :timer.h
 * @Author  :Xiaowine
 * @date    :2017/10/25
 * @version :V 1.0.0
 *************************************************
 * @brief   :
 ****************************************************************************
**/
#ifndef __TIMER_H_
#define __TIMER_H_
/* Private include -----------------------------------------------------------*/
#include "syscfg.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern __IO u32 Timer_Counter_1ms;
/* Private function prototypes -----------------------------------------------*/
void TIM4_Init(void);
void TIM4_UPD_OVF(void);

/* Private functions ---------------------------------------------------------*/

#endif /*__TIMER_H_*/
