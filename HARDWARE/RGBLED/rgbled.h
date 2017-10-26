/**
 ****************************************************************************
 * @Warning :Without permission from the author,Not for commercial use
 * @File    :rgbled.h
 * @Author  :seblee
 * @date    :2017/10/20
 * @version :V 1.0.0
 *************************************************
 * @brief   :
 ****************************************************************************
**/
#ifndef __RGBLED_H_
#define __RGBLED_H_
/* Private include -----------------------------------------------------------*/
#include "syscfg.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define TIM3_PERIOD 1000
#define TIM5_PERIOD 1000

#define LED_RED_PORT GPIOI
#define LED_GREEN_PORT GPIOI
#define LED_BLUE_PORT GPIOH
#define LED_BACK_PORT GPIOH

#define LED_RED_PIN GPIO_Pin_0
#define LED_GREEN_PIN GPIO_Pin_3
#define LED_BLUE_PIN GPIO_Pin_7
#define LED_BACK_PIN GPIO_Pin_6

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern __IO uint16_t TIM3_CCR1_Val;
extern __IO uint16_t TIM3_CCR2_Val;
extern __IO uint16_t TIM5_CCR1_Val;
extern __IO uint16_t TIM5_CCR2_Val;
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void RGBLED_CFG(void);

#endif /*__RGBLED_H_*/
