/**
 ****************************************************************************
 * @Warning :Without permission from the author,Not for commercial use
 * @File    :syscfg.c
 * @Author  :seblee
 * @date    :2017/10/23
 * @version :V 1.0.0
 *************************************************
 * @brief   :
 ****************************************************************************
 **/
/* Private include -----------------------------------------------------------*/
#include "syscfg.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void LSE_StabTime(void);
/* Private functions ---------------------------------------------------------*/

/**
 ****************************************************************************
 * @Function : void SysClock_Init(void)
 * @File     : syscfg.c
 * @Program  :
 * @Created  : 2017/10/19 by seblee
 * @Brief    : 初始化系統時鐘
 * @Version  : V1.0
**/
void SysClock_Init(void)
{
    /* Infinite loop */
    CLK_DeInit(); //时钟恢复默认
#ifdef CLK_SUE_HSI
    CLK_HSICmd(ENABLE);
    while (CLK_GetFlagStatus(CLK_FLAG_HSIRDY) == RESET)
        ; //等待直到HSI稳定

    CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI); // 指定HSI为主时钟
    CLK_SYSCLKSourceSwitchCmd(ENABLE);
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1); //設置分頻
#endif

#ifndef RTC_LSE
    CLK_LSICmd(ENABLE); // 使能内部LSI OSC（38KHz）
    while (CLK_GetFlagStatus(CLK_FLAG_LSIRDY) == RESET)
        ; //等待直到LSI稳定
#else
    CLK_LSEConfig(CLK_LSE_ON); // 使能外部LSE OSC（32.768KHz）
    while (CLK_GetFlagStatus(CLK_FLAG_LSERDY) == RESET)
        ; //等待直到LSE稳定
    /* wait for 1 second for the LSE Stabilisation */
    LSE_StabTime();
#endif
}
/**
  * @brief  Wait 1 sec for LSE stabilization .
  * @param  None.
  * @retval None.
  * Note : TIM4 is configured for a system clock = 2MHz
  */
void LSE_StabTime(void)
{

    CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);

    /* Configure TIM4 to generate an update event each 1 s */
    TIM4_TimeBaseInit(TIM4_Prescaler_16384, 123);
    /* Clear update flag */
    TIM4_ClearFlag(TIM4_FLAG_Update);

    /* Enable TIM4 */
    TIM4_Cmd(ENABLE);

    /* Wait 1 sec */
    while (TIM4_GetFlagStatus(TIM4_FLAG_Update) == RESET)
        ;

    TIM4_ClearFlag(TIM4_FLAG_Update);

    /* Disable TIM4 */
    TIM4_Cmd(DISABLE);

    CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, DISABLE);
}
