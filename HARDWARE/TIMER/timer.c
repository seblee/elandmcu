/**
 ****************************************************************************
 * @Warning :Without permission from the author,Not for commercial use
 * @File    :timer.c
 * @Author  :seblee
 * @date    :2017/10/19
 * @version :V 1.0.0
 *************************************************
 * @brief   :
 ****************************************************************************
**/

/* Private include -----------------------------------------------------------*/
#include "timer.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define TIM4_PERIOD 124

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
__IO u32 Timer_Counter_1ms = 0;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/**
 ****************************************************************************
 * @Function : void TIM4_Init(void)
 * @File     : timer.c
 * @Program  : none
 * @Created  : 2017/10/19 by seblee
 * @Brief    : system timer init
 * @Version  : V1.0
**/
void TIM4_Init(void)
{
    /* Enable TIM4 clock */
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);
    /* TIM4 Peripheral Configuration */
    /* Time Base configuration */
    TIM4_TimeBaseInit(TIM4_Prescaler_128, TIM4_PERIOD);
    /* TIM4 update interrupt enable */
    TIM4_ITConfig(TIM4_IT_Update, ENABLE);
    /* TIM4 counter enable */
    TIM4_Cmd(ENABLE);
}
/**
 ****************************************************************************
 * @Function : void TIM4_UPD_OVF(void)
 * @File     : timer.c
 * @Program  :
 * @Created  : 2017/10/19 by seblee
 * @Brief    : Tim4 interrupt service function
 * @Version  : V1.0
**/
void TIM4_UPD_OVF(void)
{
    Timer_Counter_1ms++;
}
