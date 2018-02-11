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
#include "timing_delay.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
int Firmware_Version_Major = 0;
int Firmware_Version_Minor = 0;
/* Private function prototypes -----------------------------------------------*/

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
    // Delay_By_nop(50000);
    // Delay_By_nop(50000);
    Delay_By_nop(10000);
#endif
    //CLK_CCOConfig(CLK_CCOSource_LSE, CLK_CCODiv_1);
}
