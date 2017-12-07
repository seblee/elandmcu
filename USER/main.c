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
    uint16_t j = 0;
    uint16_t i = 0;
    LCD_Wifi_Rssi_t rssi_value[5] = {LEVEL0, LEVEL1, LEVEL2, LEVEL3, LEVEL4};
    __eland_color_t color;
    disableInterrupts();
    /* System clock */
    SysClock_Init();
    TIM4_Init();
    ElandKeyInit();
    ELAND_RTC_Init();
    IWDG_Config();
    enableInterrupts();
    HT162x_init();
    RGBLED_CFG();

    /* Reload IWDG counter */
    IWDG_ReloadCounter();
    HT162x_LCD_Clear(SET);
    HT162x_LCD_Change_Pixel(COM0, SEG08, SET);
    HT162x_LCD_Change_Pixel(COM0, SEG16, SET);
    HT162x_LCD_Change_Pixel(COM0, SEG32, SET);
    HT162x_LCD_Change_Pixel(COM0, SEG33, SET);
    HT162x_LCD_Change_Pixel(COM7, SEG15, SET);
    HT162x_LCD_Change_Pixel(COM7, SEG07, SET);
    HT162x_LCD_Change_Pixel(COM7, SEG30, SET);
    HT162x_LCD_Change_Pixel(COM7, SEG31, SET);

    /* Infinite loop */
    while (1)
    {
        /* Reload IWDG counter */
        IWDG_ReloadCounter();
        Eland_KeyState_Read();
        if ((Key_Trg & KEY_Set) ||
            (Key_Trg & KEY_Reset) ||
            (Key_Trg & KEY_Add) ||
            (Key_Trg & KEY_Minus) ||
            (Key_Trg & KEY_MON) ||
            (Key_Trg & KEY_AlarmMode) ||
            (Key_Trg & KEY_Wifi) ||
            (Key_Trg & KEY_Snooze) ||
            (Key_Trg & KEY_Alarm))
        {
            if (color == ELAND_RED)
                color = ELAND_GREEN;
            else if (color == ELAND_GREEN)
                color = ELAND_BLUE;
            else if (color == ELAND_BLUE)
                color = ELAND_RED;
            else
                color = ELAND_RED;
            RGBLED_Color_Set(color);
        }
        if (WakeupOccurred == TRUE) //500ms point flash
        {
            WakeupOccurred = FALSE;
            HT162x_LCD_Toggle_Pixel(COM0, SEG32);
            HT162x_LCD_Toggle_Pixel(COM0, SEG33);
        }
        if (AlarmOccurred == TRUE)
        {
            HT162x_LCD_Time_Display(ElandCurrentTime);
            AlarmOccurred = FALSE;
        }
        if ((i++ % 20) == 0)
        {
            HT162x_LCD_RSSI_Set(rssi_value[j % 5]);
            j++;
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
