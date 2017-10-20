/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"
#include "syscfg.h"
#include "timer.h"
#include "gpio.h"
#include "timing_delay.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
    disableInterrupts();
    SysClock_Init();
    TIM4_Init();
    GPIO_Config();
    enableInterrupts();
    /* Infinite loop */
    while (1)
    {
        /* Toggle LEDs LD1..LD4 */
        GPIO_SetBits(LED_RED_PORT, LED_RED_PIN);
        Delay_By_nop(0xFFFF);
        Delay_By_nop(0xFFFF);
        Delay_By_nop(0xFFFF);
        Delay_By_nop(0xFFFF);
        Delay_By_nop(0xFFFF);
        Delay_By_nop(0xFFFF);
        Delay_By_nop(0xFFFF);
        Delay_By_nop(0xFFFF);
        GPIO_ResetBits(LED_RED_PORT, LED_RED_PIN);
        GPIO_SetBits(LED_GREEN_PORT, LED_GREEN_PIN);
        Delay_By_nop(0xFFFF);
        Delay_By_nop(0xFFFF);
        Delay_By_nop(0xFFFF);
        Delay_By_nop(0xFFFF);
        Delay_By_nop(0xFFFF);
        Delay_By_nop(0xFFFF);
        Delay_By_nop(0xFFFF);
        Delay_By_nop(0xFFFF);
        GPIO_ResetBits(LED_GREEN_PORT, LED_GREEN_PIN);
        GPIO_SetBits(LED_BLUE_PORT, LED_BLUE_PIN);
        Delay_By_nop(0xFFFF);
        Delay_By_nop(0xFFFF);
        Delay_By_nop(0xFFFF);
        Delay_By_nop(0xFFFF);
        Delay_By_nop(0xFFFF);
        Delay_By_nop(0xFFFF);
        Delay_By_nop(0xFFFF);
        Delay_By_nop(0xFFFF);
        GPIO_ResetBits(LED_BLUE_PORT, LED_BLUE_PIN);
        GPIO_SetBits(LED_BACK_PORT, LED_BACK_PIN);
        Delay_By_nop(0xFFFF);
        Delay_By_nop(0xFFFF);
        Delay_By_nop(0xFFFF);
        Delay_By_nop(0xFFFF);
        Delay_By_nop(0xFFFF);
        Delay_By_nop(0xFFFF);
        Delay_By_nop(0xFFFF);
        Delay_By_nop(0xFFFF);
        GPIO_ResetBits(LED_BACK_PORT, LED_BACK_PIN);
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

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
