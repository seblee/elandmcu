/**
 ****************************************************************************
 * @Warning :Without permission from the author,Not for commercial use
 * @File    :rgbled.c
 * @Author  :Xiaowine
 * @date    :2017/10/20
 * @version :V 1.0.0
 *************************************************
 * @brief   :
 ****************************************************************************
**/
/* Private include -----------------------------------------------------------*/
#include "rgbled.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define TIM3_PERIOD
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void RGBLED_GPIO_Cfg(void);
/* Private functions ---------------------------------------------------------*/

/**
 ****************************************************************************
 * @Function : void RGBLED_CFG(void)
 * @File     : rgbled.c
 * @Program  : NONE
 * @Created  : 2017/10/20 by Xiaowine
 * @Program  : PI0 TIM3---channel 1
 *             PI3 TIM3---channel 2
 *             PH7 TIM5---channel 2
 *             PH6 TIM5---channel 1
 * @Version  : V1.0
**/
void RGBLED_CFG(void)
{
    RGBLED_GPIO_Cfg();
}

/**
 ****************************************************************************
 * @Function : static void RGBLED_GPIO_Cfg(void)
 * @File     : rgbled.c
 * @Program  : none
 * @Created  : 2017/10/20 by Xiaowine
 * @Program  : PI0 TIM3---channel 1
 *             PI3 TIM3---channel 2
 *             PH7 TIM5---channel 2
 *             PH6 TIM5---channel 1
 * @Version  : V1.0
**/
static void RGBLED_GPIO_Cfg(void)
{
    GPIO_Init(GPIOI, GPIO_Pin_0 | GPIO_Pin_3, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_Init(GPIOH, GPIO_Pin_6 | GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Fast);
}
static void TIM3_Config(void)
{
    /* Enable TIM3 clock */
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM3, ENABLE);

    TIM3_TimeBaseInit(TIM3_Prescaler_128, TIM3_CounterMode_Up, );
}
