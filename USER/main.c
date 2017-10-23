/**
 ****************************************************************************
 * @Warning :Without permission from the author,Not for commercial use
 * @File    :main.c
 * @Author  :Xiaowine
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
 * @Created  : 2017/10/20 by Xiaowine
 * @Brief    : main function
 * @Version  : V1.0
**/
void main(void)
{
    disableInterrupts();
    SysClock_Init();
    //TIM4_Init();
    enableInterrupts();
    GPIO_SetBits(LED_BACK_PORT, LED_BACK_PIN);
    /* Infinite loop */
    while (1)
    {
        /* Toggle LEDs LD1..LD4 */
        GPIO_SetBits(LED_RED_PORT, LED_RED_PIN);
        Delay_By_nop(0xFFFFF);
        GPIO_ResetBits(LED_RED_PORT, LED_RED_PIN);

        GPIO_SetBits(LED_GREEN_PORT, LED_GREEN_PIN);
        Delay_By_nop(0xFFFFF);
        GPIO_ResetBits(LED_GREEN_PORT, LED_GREEN_PIN);

        GPIO_SetBits(LED_BLUE_PORT, LED_BLUE_PIN);
        Delay_By_nop(0xFFFFF);
        GPIO_ResetBits(LED_BLUE_PORT, LED_BLUE_PIN);

        Delay_By_nop(0xFFFFF);
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
