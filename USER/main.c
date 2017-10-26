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
#include "rgbled.h"
#include "timing_delay.h"
#include "iwdg.h"
#include "lcd_eland.h"
#include "key.h"
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
    SysClock_Init();
    TIM4_Init();
    enableInterrupts();
    IWDG_Config();
    RGBLED_CFG();
    TIM3_SetCompare1(TIM3_PERIOD);
    TIM3_SetCompare2(TIM3_PERIOD);
    TIM5_SetCompare1(TIM5_PERIOD);
    TIM5_SetCompare2(TIM5_PERIOD);
    LCD_ELAND_Init();
    LCD_ELAND_Write_All();
    ElandKeyInit();

    /* Infinite loop */
    while (1)
    {
        /* Reload IWDG counter */
        IWDG_ReloadCounter();
        Eland_KeyState_Read();

        if (Count & 0x01)
        {
            if (TIM3_CCR1_Val < (TIM3_PERIOD + 1))
                TIM3_CCR1_Val += 1;
            else
                TIM3_CCR1_Val = 0;
            TIM3_SetCompare1(TIM3_CCR1_Val);
        }

        if (Count & 0x04)
        {
            if (TIM3_CCR2_Val < (TIM3_PERIOD + 1))
                TIM3_CCR2_Val += 1;
            else
                TIM3_CCR2_Val = 0;
            TIM3_SetCompare2(TIM3_CCR2_Val);
        }

        if (Count & 0x02)
        {
            if (TIM5_CCR1_Val < (TIM5_PERIOD + 1))
                TIM5_CCR1_Val += 1;
            else
                TIM5_CCR1_Val = 0;
            TIM5_SetCompare1(TIM5_CCR1_Val);
        }

        if (Count & 0x08)
        {
            if (TIM5_CCR2_Val < (TIM5_PERIOD + 1))
                TIM5_CCR2_Val += 1;
            else
                TIM5_CCR2_Val = 0;
            TIM5_SetCompare2(TIM5_CCR2_Val);
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
