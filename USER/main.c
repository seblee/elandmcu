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
    __eland_color_t color = ELAND_RED;
    LCD_COMx_TypeDef COMx = COM_0;
    disableInterrupts();
    /* System clock */
    SysClock_Init();
    TIM4_Init();
    UART1_Init();
    ELAND_RTC_Init();
    LCD_ELAND_Init();
    RGBLED_CFG();
    ElandKeyInit();
    IWDG_Config();
    enableInterrupts();
    LCD_ELAND_Write_All();
    //LCD_Eland_COMx_Write(COM_0);
    IWDG_ReloadCounter();
    TIM5_SetCompare1(TIM5_PERIOD); //LCD_Eland_Time_Display(ElandCurrentTime);
    /* Infinite loop */
    while (1)
    {
        /* Reload IWDG counter */
        IWDG_ReloadCounter();
        Eland_KeyState_Read();
        if (AlarmOccurred == TRUE)
        {
            AlarmOccurred = FALSE;
            //if (Key_Count & KEY_MON)
            LCD_Eland_Time_Display(ElandCurrentTime);
            eland_state_display();
        }
        if (Key_Trg & KEY_Minus)
        {
            //LCD_Eland_COMx_Clear(COMx);
            if (COMx == COM_7)
                COMx = COM_0;
            else
                COMx++;
            //LCD_Eland_COMx_Write(COMx);
        }
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
