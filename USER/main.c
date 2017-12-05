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
    disableInterrupts();
    /* System clock */
    SysClock_Init();
    TIM4_Init();
    ELAND_RTC_Init();
    //IWDG_Config();
    enableInterrupts();
    HT162x_init();
    IWDG_ReloadCounter();
    HT162x_LCD_Num_Set(Serial_07, 7);
    /* Infinite loop */

    while (1)
    {
        /* Reload IWDG counter */
        IWDG_ReloadCounter();
        if (AlarmOccurred == TRUE)
        {
            AlarmOccurred = FALSE;
        }

        while (1)
        {
            if (Timer_Counter_1ms > 20) //20ms
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
