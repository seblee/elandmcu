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
#include "usart.h"
#include "rtc.h"
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
    u8 temp;
    u8 data = 0;
    __eland_color_t color = ELAND_BLACK;
    disableInterrupts();
    /* System clock */
    SysClock_Init();
    TIM4_Init();
    UART1_Init();
    ELAND_RTC_Init();
    LCD_ELAND_Init();
    RGBLED_CFG();
    ElandKeyInit();
    RGBLED_Color_Set(ELAND_WHITE);
    IWDG_Config();
    enableInterrupts();
    LCD_ELAND_Write_All();
    IWDG_ReloadCounter();
    TIM5_SetCompare1(TIM5_PERIOD);
    /* Infinite loop */
    // LCD_Eland_Week_Set(TIME_WEEK, SATURDAY);
    // LCD_Eland_Week_Set(ALARM_WEEK, SATURDAY);
    while (1)
    {
        /* Reload IWDG counter */
        IWDG_ReloadCounter();
        Eland_KeyState_Read();
        if (AlarmOccurred == TRUE)
        {
            AlarmOccurred = FALSE;
            // LCD_Eland_Time_Display(ElandCurrentTime);
        }
        temp++;
        if (temp > 60)
        {
            temp = 0;
            if (color == ELAND_WHITE)
                color = ELAND_BLACK;
            else
                color++;
            RGBLED_Color_Set(color);

            LCD_Eland_Num_Set(Serial_01, data % 10);
            LCD_Eland_Num_Set(Serial_02, data % 10);
            LCD_Eland_Num_Set(Serial_03, data % 10);
            LCD_Eland_Num_Set(Serial_04, data % 10);
            LCD_Eland_Num_Set(Serial_05, data % 10);
            LCD_Eland_Num_Set(Serial_06, data % 10);
            LCD_Eland_Num_Set(Serial_07, data % 10);
            LCD_Eland_Num_Set(Serial_08, data % 10);
            LCD_Eland_Num_Set(Serial_09, data % 10);
            LCD_Eland_Num_Set(Serial_10, data % 10);
            LCD_Eland_Num_Set(Serial_11, data % 10);
            LCD_Eland_Num_Set(Serial_12, data % 10);
            LCD_Eland_Num_Set(Serial_13, data % 10);
            LCD_Eland_Num_Set(Serial_14, data % 10);
            LCD_Eland_Num_Set(Serial_15, data % 10);
            LCD_Eland_Num_Set(Serial_16, data % 10);
            LCD_Eland_Num_Set(Serial_17, data % 10);
            LCD_Eland_Num_Set(Serial_18, data % 10);
            LCD_Eland_Num_Set(Serial_19, data % 10);
            LCD_Eland_Num_Set(Serial_20, data % 10);
            LCD_Eland_Week_Set(TIME_WEEK, (LCD_Week_Day_t)(data % 7));
            LCD_Eland_Week_Set(ALARM_WEEK, (LCD_Week_Day_t)(data % 7));
            LCD_Eland_Wifi_RSSI_Set((LCD_Wifi_Rssi_t)(data % 5));
            data++;
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
