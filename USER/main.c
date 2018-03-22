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
    uint8_t number = 0;
    uint8_t i;
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
    HT162x_LCD_Clear(SET);
    /* Infinite loop */
    while (1)
    {
        /* Reload IWDG counter */
        IWDG_ReloadCounter();
        Eland_KeyState_Read();
        if ((Key_Down_Trg & KEY_Set) ||
            (Key_Down_Trg & KEY_Reset) ||
            (Key_Down_Trg & KEY_Add) ||
            (Key_Down_Trg & KEY_Minus) ||
            (Key_Down_Trg & KEY_MON) ||
            (Key_Down_Trg & KEY_AlarmMode) ||
            (Key_Down_Trg & KEY_Wifi) ||
            (Key_Down_Trg & KEY_Snooze) ||
            (Key_Down_Trg & KEY_Alarm))
            break;
        while (1)
        {
            if (Timer_Counter_1ms > 20) //20ms
            {
                Timer_Counter_1ms = 0;
                break;
            }
        }
    }

    while (1)
    {
        /* Reload IWDG counter */
        IWDG_ReloadCounter();
        Eland_KeyState_Read();
        if ((Key_Down_Trg & KEY_Set) ||
            (Key_Down_Trg & KEY_Reset) ||
            (Key_Down_Trg & KEY_Add) ||
            (Key_Down_Trg & KEY_Minus) ||
            (Key_Down_Trg & KEY_MON) ||
            (Key_Down_Trg & KEY_AlarmMode) ||
            (Key_Down_Trg & KEY_Wifi) ||
            (Key_Down_Trg & KEY_Snooze) ||
            (Key_Down_Trg & KEY_Alarm))
        {
            number++;
            for (i = Serial_01; i < Serial_MAX; i++)
                HT162x_LCD_Num_Set((LCD_Digital_Serial_t)i, number % 10);
            HT162x_LCD_Week_Set(TIME_PART, (LCD_Week_Day_t)(number % WEEKDAYMAX));
            HT162x_LCD_Week_Set(ALARM_PART, (LCD_Week_Day_t)(number % WEEKDAYMAX));
            HT162x_LCD_RSSI_Set(RSS_Array[number % 5]);
            HT162x_LCD_AMPM_Set(TIME_PART, (LCD_AMPM_Distinguish_t)(number % 2));
            HT162x_LCD_AMPM_Set(ALARM_PART, (LCD_AMPM_Distinguish_t)(number % 2));
            HT162x_LCD_TCP_STATE_Set((number % 2) ? RESET : SET);
            /*refresh snooze point*/
            HT162x_LCD_Change_Pixel(COM7, SEG13, (number % 2) ? RESET : SET);
            /*next alarm display*/
            HT162x_LCD_Change_Pixel(COM7, SEG11, (number % 2) ? RESET : SET);

            HT162x_LCD_Toggle_Pixel(COM0, SEG32);
            HT162x_LCD_Toggle_Pixel(COM0, SEG33);

            HT162x_LCD_Toggle_Pixel(COM0, SEG08);
            HT162x_LCD_Toggle_Pixel(COM0, SEG16);

            HT162x_LCD_Toggle_Pixel(COM7, SEG15);
            HT162x_LCD_Toggle_Pixel(COM7, SEG07);

            HT162x_LCD_Toggle_Pixel(COM7, SEG30);
            HT162x_LCD_Toggle_Pixel(COM7, SEG31);

            RGBLED_RGBCode_Set(RGB_Buff[number % 3]);
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
