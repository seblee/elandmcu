/**
 ****************************************************************************
 * @Warning :Without permission from the author,Not for commercial use
 * @File    :lcd_display.c
 * @Author  :seblee
 * @date    :2017/12/12
 * @version :V 1.0.0
 *************************************************
 * @brief   :
 ****************************************************************************
**/
/* Private include -----------------------------------------------------------*/
#include "lcd_display.h"
#include "key.h"
#include "rtc.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
_eland_data_t eland_data;
_alarm_data_t alarm_data;
_ELAND_MODE_t Eland_mode = ELAND_MODE_NONE;

const LCD_Digital_Serial_t Clock_number_table[4][2] = {
    /*          */
    {Serial_MAX, Serial_MAX},
    {Serial_17, Serial_18},
    {Serial_19, Serial_20},
    {Serial_01, Serial_02},
};

/* Private functions ---------------------------------------------------------*/
/**
 ****************************************************************************
 * @Function : void LCD_data_init(void)
 * @File     : lcd_display.c
 * @Program  : none
 * @Created  : 2018/1/9 by seblee
 * @Brief    : data init
 * @Version  : V1.0
**/
void LCD_data_init(void)
{
    eland_data.time_display_format = 1;
    eland_data.brightness_normal = 80;
    eland_data.brightness_night = 20;
    eland_data.night_mode_enable = 0;
    eland_data.night_mode_begin_time = 79200;
    eland_data.night_mode_end_time = 21600;

    memset(&alarm_data, 0, sizeof(_alarm_data_t));
}

/**
 ****************************************************************************
 * @Function : void LCD_Display_State(Eland_Status_type_t state)
 * @File     : lcd_display.c
 * @Program  : state:the state get from eland
 * @Created  : 2017/12/12 by seblee
 * @Brief    : display state
 * @Version  : V1.0
**/
void LCD_Display_State(Eland_Status_type_t state)
{
    switch (state)
    {
    case APStatus:
        break;
    default:
        break;
    }
}
/**
 ****************************************************************************
 * @Function : void LCD_Display_Rssi_State(Eland_Status_type_t state)
 * @File     : lcd_display.c
 * @Program  : state:the state get from eland
 * @Created  : 2017/12/12 by seblee
 * @Brief    : display RSSI state
 * @Version  : V1.0
**/
void LCD_Display_Rssi_State(Eland_Status_type_t state)
{
    static LCD_Wifi_Rssi_t rssi_value = LEVEL0;
    if ((state == APStatus) || (state == HttpServerStatus))
    {
        if (rssi_value == LEVEL0)
            rssi_value = LEVEL4;
        else
            rssi_value = LEVEL0;
        HT162x_LCD_RSSI_Set(rssi_value);
    }
    else
    {
        if (RSSI_Value >= RSSI_STATE_STAGE4)
            rssi_value = LEVEL4;
        else if (RSSI_Value >= RSSI_STATE_STAGE3)
            rssi_value = LEVEL3;
        else if (RSSI_Value >= RSSI_STATE_STAGE2)
            rssi_value = LEVEL2;
        else if (RSSI_Value >= RSSI_STATE_STAGE1)
            rssi_value = LEVEL1;
        else
            rssi_value = LEVEL0;
        HT162x_LCD_RSSI_Set(rssi_value);
    }
}
/**
 ****************************************************************************
 * @Function : void LCD_Clock_MON(void)
 * @File     : lcd_display.c
 * @Program  : none
 * @Created  : 2018/1/9 by seblee
 * @Brief    : display clock mode without time
 * @Version  : V1.0
**/
void LCD_Clock_MON(void)
{
    uint8_t i;
    static uint8_t time_set_mode = 0;
    static uint8_t number_flash_flag = 0;
    static uint8_t number_flash_cache;
    if (Eland_mode != ELAND_CLOCK_MON)
    {
        /**refresh wifi**/
        HT162x_LCD_RSSI_Set(LEVEL0);
        HT162x_LCD_Change_Pixel(COM7, SEG21, RESET);
        /**refresh time**/
        HT162x_LCD_Time_Display(TIME_PART, ElandCurrentTime);
        /**refresh date**/
        HT162x_LCD_Date_Display(TIME_PART, ElandCurrentTime);
        /*clear alarm -- next alarm*/
        HT162x_LCD_Change_Pixel(COM7, SEG11, RESET);
        /*clear alarm -- snooze*/
        HT162x_LCD_Change_Pixel(COM7, SEG13, RESET);
        /*clear alarm -- alarm date and time */
        for (i = (uint8_t)Serial_11; i < (uint8_t)Serial_17; i++)
            HT162x_LCD_Num_Set((LCD_Digital_Serial_t)i, 10);
        /**refresh alarm time**/
        HT162x_LCD_Time_Display(ALARM_PART, alarm_data.alarm_moment);
        /*clear alarm -- alarm week*/
        HT162x_LCD_Week_Set(ALARM_PART, WEEKDAYMAX);
        /**SHOW the line**/
        HT162x_LCD_Change_Pixel(COM7, SEG33, SET);
        Eland_mode = ELAND_CLOCK_MON;
        time_set_mode = 0;
    }
    /******key menue***********/
    switch (time_set_mode)
    {
    case 0:
        if (Key_Restain_Trg & KEY_Set)
        {
            time_set_mode = 3;
            number_flash_cache = ElandCurrentTime.year;
        }
        if (Key_Trg & KEY_Set)
        {
            time_set_mode = 1;
            number_flash_cache = alarm_data.alarm_moment.hour;
        }
        break;
    case 1: //alarm hour
        if (Key_Trg & KEY_Add)
        {
            if (alarm_data.alarm_moment.hour < 24)
                alarm_data.alarm_moment.hour++;
            else
                alarm_data.alarm_moment.hour = 0;
            number_flash_cache = alarm_data.alarm_moment.hour;
        }
        else if (Key_Trg & KEY_Minus)
        {
            if (alarm_data.alarm_moment.hour > 0)
                alarm_data.alarm_moment.hour--;
            else
                alarm_data.alarm_moment.hour = 23;
            number_flash_cache = alarm_data.alarm_moment.hour;
        }
        if (Key_Trg & KEY_Set)
        {
            time_set_mode = 2;
            number_flash_cache = alarm_data.alarm_moment.minute;
        }
        break;
    case 2: //alarm minute
        if (Key_Trg & KEY_Add)
        {
            if (alarm_data.alarm_moment.minute < 59)
                alarm_data.alarm_moment.minute++;
            else
                alarm_data.alarm_moment.minute = 0;
            number_flash_cache = alarm_data.alarm_moment.minute;
        }
        else if (Key_Trg & KEY_Minus)
        {
            if (alarm_data.alarm_moment.minute > 0)
                alarm_data.alarm_moment.minute--;
            else
                alarm_data.alarm_moment.minute = 59;
            number_flash_cache = alarm_data.alarm_moment.minute;
        }
        if (Key_Trg & KEY_Set)
        {
            time_set_mode = 0;
            number_flash_cache = 0;
            Eland_mode = ELAND_MODE_NONE;
        }
        break;
    case 3: //date year
        if (Key_Trg & KEY_Add)
        {
            number_flash_cache = ++ElandCurrentTime.year;
        }
        else if (Key_Trg & KEY_Minus)
        {
            if (ElandCurrentTime.year > 0)
                ElandCurrentTime.year--;
            else
                ElandCurrentTime.year = 59;
            number_flash_cache = --ElandCurrentTime.year;
        }
        if (Key_Trg & KEY_Set)
        {
            time_set_mode = 0;
            number_flash_cache = 0;
            Eland_mode = ELAND_MODE_NONE;
        }
        break;
    default:
        break;
    }

    if (WakeupOccurred == TRUE) //500ms point flash
    {
        WakeupOccurred = FALSE;
        HT162x_LCD_Toggle_Pixel(COM0, SEG32);
        HT162x_LCD_Toggle_Pixel(COM0, SEG33);
        if (time_set_mode != 0)
        {
            if (number_flash_flag == 0)
            {
                HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][0], ((number_flash_cache / 10) % 10)); //minute
                HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][1], (number_flash_cache % 10));        //minute
                number_flash_flag = 1;
            }
            else
            {
                HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][0], 10); //minute
                HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][1], 10); //minute
                number_flash_flag = 0;
            }
        }
    }
    if (AlarmOccurred == TRUE) //1s update time
    {
        HT162x_LCD_Time_Display(TIME_PART, ElandCurrentTime);
        HT162x_LCD_Date_Display(TIME_PART, ElandCurrentTime);
        AlarmOccurred = FALSE;
    }
}
/**
 ****************************************************************************
 * @Function : void LCD_Clock_Alarm(void)
 * @File     : lcd_display.c
 * @Program  : none
 * @Created  : 2018/1/9 by seblee
 * @Brief    : display clock mode with alarm
 * @Version  : V1.0
**/
void LCD_Clock_Alarm(void)
{
    if (Eland_mode != ELAND_CLOCK_MON)
    {

        Eland_mode = ELAND_CLOCK_ALARM;
    }
}
/**
 ****************************************************************************
 * @Function : void LCD_NetMode(void)
 * @File     : lcd_display.c
 * @Program  : none
 * @Created  : 2018/1/9 by seblee
 * @Brief    : display_netmode
 * @Version  : V1.0
**/
void LCD_NetMode(void)
{
}
