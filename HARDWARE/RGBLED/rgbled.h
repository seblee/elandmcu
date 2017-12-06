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
/* Private typedef -----------------------------------------------------------*/
typedef enum {
    ELAND_BLACK,
    ELAND_BLUE,
    ELAND_WHITE_BLUE,
    ELAND_PURPLE,
    ELAND_RED,
    ELAND_PINK,
    ELAND_ORANGE,
    ELAND_YELLOW,
    ELAND_YELLOW_GREEN,
    ELAND_GREEN,
    ELAND_WHITE,
} __eland_color_t;

/* Private define ------------------------------------------------------------*/
#define TIM3_PERIOD 1000
#define TIM2_PERIOD 1000
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
extern __IO uint16_t TIM2_CCR1_Val;
extern __IO uint16_t TIM2_CCR2_Val;
extern __IO uint16_t TIM3_CCR1_Val;
extern __IO uint16_t TIM3_CCR2_Val;
extern __IO uint16_t TIM5_CCR1_Val;
extern __IO uint16_t TIM5_CCR2_Val;
/* Private function prototypes -----------------------------------------------*/
void RGBLED_CFG(void);
void RGBLED_Input_RGB(u8 Red, u8 Green, u8 Blue);
void RGBLED_Color_Set(__eland_color_t color);
void RGBLED_RGBCode_Set(u32 ColorCode);

/* Private functions ---------------------------------------------------------*/

#endif /*__RGBLED_H_*/
