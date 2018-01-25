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
    disableInterrupts();
    OTA_bootloader_disable();
    enableInterrupts();
    /* System clock */
    LCD_data_init();
    SysClock_Init();
    HT162x_init();
    ELAND_RTC_Init();
    TIM4_Init();
    UART1_Init();
    ElandKeyInit();
    IWDG_Config();
    RGBLED_CFG();
    /* Reload IWDG counter */
    IWDG_ReloadCounter();
    RGBLED_Rainbow_Set(RAINBOW_RED);
    /* Infinite loop */
    while (1)
    {
        /* Reload IWDG counter */
        IWDG_ReloadCounter();
        Eland_KeyState_Read();
        if (Key_Count & KEY_Wifi) //NC/NA mode
            LCD_NetMode();
        else if ((Key_Count & KEY_MON) ||
                 (Key_Count & KEY_AlarmMode)) //clock MON mode
            LCD_Clock_MON();

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
