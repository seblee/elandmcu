/**
 ****************************************************************************
 * @Warning :Without permission from the author,Not for commercial use
 * @File    :main.c
 * @Author  :seblee
 * @date    :2017/10/20
 * @version :V 1.0.0
 *************************************************
 * @brief   :
 ****************************************************************************
**/

/* Private include -----------------------------------------------------------*/
#include "stm8l15x.h"
#include "syscfg.h"
#include "timer.h"
#include "timing_delay.h"
#include "iwdg.h"
#include "rtc.h"
#include "ht162x.h"
#include "key.h"
#include "rgbled.h"
#include "usart.h"
#include "lcd_display.h"
#include "eland_ota.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
 ****************************************************************************
 * @Function : void main(void)
 * @File     : main.c
 * @Program  : none
 * @Created  : 2017/10/20 by seblee
 * @Brief    : main function
 * @Version  : V1.0
**/
void main(void)
{
    uint16_t count = 0;
    disableInterrupts();
    OTA_bootloader_disable();
    enableInterrupts();
    /* System clock */
    SysClock_Init();
    /* System restart by iwdg */
    if ((RST_GetFlagStatus(RST_FLAG_IWDGF) != RESET) /* ||
        (RST_GetFlagStatus(RST_FLAG_SWIMF) != RESET)*/
    )
    {
        rst_flag = RST_FLAG_IWDGF;
        RST_ClearFlag(RST_FLAG_IWDGF);
        RST_ClearFlag(RST_FLAG_SWIMF);
    }
    /* System restart by poweron */
    else
    {
        rst_flag = (RST_FLAG_TypeDef)0;
        LCD_data_init();
        ELAND_RTC_Check();
    }
    HT162x_init();
    RGBLED_CFG();
    ElandKeyInit();
    TIM4_Init();
    UART1_Init();
    ELAND_RTC_Init();
    IWDG_Config();
    /* Reload IWDG counter */
    IWDG_ReloadCounter();
    /* Infinite loop */
    while (1)
    {
        if (count < 500)
        {
            count++;
        }
        /* Reload IWDG counter */
        IWDG_ReloadCounter();
        /* read key state */
        Eland_KeyState_Read();
        /***NC/NA mode**/
        if (Key_Count & KEY_Wifi)
            LCD_NetMode();
        /***offline mode**/
        else if (Key_Count & KEY_MON)
            LCD_Clock_MON();
        else
            LCD_Clock_MON();
        while (1)
        {
            if (Timer_Counter_1ms >= 20) //20ms
            {
                Timer_Counter_1ms = 0;
                break;
            }
        }
    }
}
#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}
#endif
