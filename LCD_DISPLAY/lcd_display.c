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
#include "rgbled.h"
#include "key.h"
#include "rtc.h"
#include "eland_usart.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
_eland_data_t eland_data;
_alarm_mcu_data_t alarm_data;
_alarm_mcu_data_t alarm_data_eland;
bool Alarm_need_Refresh = TRUE;
_ELAND_MODE_t Eland_mode = ELAND_MODE_NONE;

const LCD_Digital_Serial_t Clock_number_table[8][2] = {
    /*          */
    {Serial_MAX, Serial_MAX},
    {Serial_17, Serial_18}, //alarm hour
    {Serial_19, Serial_20}, //alarm minute
    {Serial_01, Serial_02}, //date year
    {Serial_03, Serial_04}, //date month
    {Serial_05, Serial_06}, //date day
    {Serial_07, Serial_08}, //time hour
    {Serial_09, Serial_10}, //time minute
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

    memset(&alarm_data, 0, sizeof(_alarm_mcu_data_t));
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
    case APStatusStart:
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

    HT162x_LCD_TCP_STATE_Set((state > CONNECTED_NET) ? RESET : SET);

    if ((state == APStatusStart) || (state == APServerStart))
    {
        if (rssi_value == LEVEL0)
            rssi_value = LEVEL4;
        else
            rssi_value = LEVEL0;
        HT162x_LCD_RSSI_Set(rssi_value);
    }
    else if (state < WifyConnected)
        HT162x_LCD_RSSI_Set(LEVEL0);
    else
        HT162x_LCD_RSSI_Set(RSSI_Value);
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
    static uint8_t time_set_mode = 0, number_flash_flag = 0, number_flash_cache;
    static uint8_t key_delay = 0;
    static mico_rtc_time_t Time_cache;
    _eland_date_time_t mcutimeCache;
    static _ELAND_MODE_t Eland_modeBak = ELAND_MODE_MAX;
    /***** bit0 time bit1 alarm*************/
    uint8_t changeflag = 0;
    if (Eland_mode != Eland_modeBak)
    {
        /**refresh wifi**/
        HT162x_LCD_RSSI_Set(LEVEL0);
        HT162x_LCD_Change_Pixel(COM7, SEG21, RESET);
        /**refresh time**/
        HT162x_LCD_Time_Display(TIME_PART, CurrentMicoTime);
        /**refresh date**/
        HT162x_LCD_Date_Display(TIME_PART, CurrentMicoTime);
        /*clear alarm -- next alarm*/
        if (Key_Count & KEY_MON)
            HT162x_LCD_Change_Pixel(COM7, SEG11, RESET);
        else if (Key_Count & KEY_AlarmMode)
            HT162x_LCD_Change_Pixel(COM7, SEG11, SET);
        /*clear alarm -- snooze*/
        HT162x_LCD_Change_Pixel(COM7, SEG13, RESET);
        /*clear alarm -- alarm date */
        for (i = (uint8_t)Serial_11; i < (uint8_t)Serial_17; i++)
            HT162x_LCD_Num_Set((LCD_Digital_Serial_t)i, 10);
        /**refresh alarm time**/
        HT162x_LCD_Time_Display(ALARM_PART, alarm_data.moment_time);
        /*clear alarm -- alarm week*/
        HT162x_LCD_Week_Set(ALARM_PART, WEEKDAYMAX);
        /**SHOW the line**/
        HT162x_LCD_Change_Pixel(COM7, SEG33, SET);

        Eland_modeBak = Eland_mode;
        time_set_mode = 0;
    }
    /******************/
    if (Key_Count & KEY_MON)
        Eland_mode = ELAND_CLOCK_MON;
    else if (Key_Count & KEY_AlarmMode)
        Eland_mode = ELAND_CLOCK_ALARM;
    /******key menue***********/
    switch (time_set_mode)
    {
    case 0:
        if (Key_Restain_Trg & KEY_Set)
        {
            time_set_mode = 3;
            memcpy(&Time_cache, &CurrentMicoTime, sizeof(mico_rtc_time_t));
            Time_cache.sec = 0;
            number_flash_cache = Time_cache.year;
        }
        if (Key_Up_Trg & KEY_Set)
        {
            time_set_mode = 1;
            number_flash_cache = alarm_data.moment_time.hr;
        }
        break;
    case 1: //alarm hour
        if ((Key_Up_Trg & KEY_Add) || ((key_delay >= ChangeSpeed) && (Key_Restain & KEY_Add)))
        {
            key_delay = 0;
            if (alarm_data.moment_time.hr < 24)
                alarm_data.moment_time.hr++;
            else
                alarm_data.moment_time.hr = 0;

            if ((eland_data.time_display_format == 1) && (alarm_data.moment_time.hr > 12))
                number_flash_cache = alarm_data.moment_time.hr - 12;
            else
                number_flash_cache = alarm_data.moment_time.hr;
            if (eland_data.time_display_format == 1)
            {
                if (alarm_data.moment_time.hr >= 12)
                    HT162x_LCD_AMPM_Set(ALARM_PART, PM);
                else
                    HT162x_LCD_AMPM_Set(ALARM_PART, AM);
            }
            else
                HT162x_LCD_AMPM_Set(ALARM_PART, AMPMMAX);
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][0], ((number_flash_cache / 10) % 10));
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][1], (number_flash_cache % 10));
        }
        else if ((Key_Up_Trg & KEY_Minus) || ((key_delay >= ChangeSpeed) && (Key_Restain & KEY_Minus)))
        {
            key_delay = 0;
            if (alarm_data.moment_time.hr > 0)
                alarm_data.moment_time.hr--;
            else
                alarm_data.moment_time.hr = 23;
            if ((eland_data.time_display_format == 1) && (alarm_data.moment_time.hr > 12))
                number_flash_cache = alarm_data.moment_time.hr - 12;
            else
                number_flash_cache = alarm_data.moment_time.hr;
            if (eland_data.time_display_format == 1)
            {
                if (alarm_data.moment_time.hr >= 12)
                    HT162x_LCD_AMPM_Set(ALARM_PART, PM);
                else
                    HT162x_LCD_AMPM_Set(ALARM_PART, AM);
            }
            else
                HT162x_LCD_AMPM_Set(ALARM_PART, AMPMMAX);
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][0], ((number_flash_cache / 10) % 10));
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][1], (number_flash_cache % 10));
        }
        else
        {
            if (key_delay < ChangeSpeed)
                key_delay++;
        }
        if (Key_Up_Trg & KEY_Set)
        {
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][0], ((number_flash_cache / 10) % 10));
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][1], (number_flash_cache % 10));
            time_set_mode = 2;
            number_flash_cache = alarm_data.moment_time.min;
        }
        break;
    case 2: //alarm minute
        if ((Key_Up_Trg & KEY_Add) || ((key_delay >= ChangeSpeed) && (Key_Restain & KEY_Add)))
        {
            key_delay = 0;
            if (alarm_data.moment_time.min < 59)
                alarm_data.moment_time.min++;
            else
                alarm_data.moment_time.min = 0;
            number_flash_cache = alarm_data.moment_time.min;
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][0], ((number_flash_cache / 10) % 10));
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][1], (number_flash_cache % 10));
        }
        else if ((Key_Up_Trg & KEY_Minus) || ((key_delay >= ChangeSpeed) && (Key_Restain & KEY_Minus)))
        {
            key_delay = 0;
            if (alarm_data.moment_time.min > 0)
                alarm_data.moment_time.min--;
            else
                alarm_data.moment_time.min = 59;
            number_flash_cache = alarm_data.moment_time.min;
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][0], ((number_flash_cache / 10) % 10));
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][1], (number_flash_cache % 10));
        }
        else
        {
            if (key_delay < ChangeSpeed)
                key_delay++;
        }
        if (Key_Up_Trg & KEY_Set)
        {
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][0], ((number_flash_cache / 10) % 10));
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][1], (number_flash_cache % 10));
            changeflag |= 2;
            time_set_mode = 0;
            number_flash_cache = 0;
        }
        break;
    case 3: //date year

        if ((Key_Up_Trg & KEY_Add) || ((key_delay >= ChangeSpeed) && (Key_Restain & KEY_Add)))
        {
            key_delay = 0;
            number_flash_cache = ++Time_cache.year;
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][0], ((number_flash_cache / 10) % 10));
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][1], (number_flash_cache % 10));
        }
        else if ((Key_Up_Trg & KEY_Minus) || ((key_delay >= ChangeSpeed) && (Key_Restain & KEY_Minus)))
        {
            key_delay = 0;
            if (Time_cache.year > 0)
                Time_cache.year--;
            else
                Time_cache.year = 99;
            number_flash_cache = Time_cache.year;
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][0], ((number_flash_cache / 10) % 10));
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][1], (number_flash_cache % 10));
        }
        else
        {
            if (key_delay < ChangeSpeed)
                key_delay++;
        }
        if (Key_Up_Trg & KEY_Set)
        {
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][0], ((number_flash_cache / 10) % 10));
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][1], (number_flash_cache % 10));
            number_flash_cache = Time_cache.month;
            time_set_mode = 4;
        }
        break;
    case 4: //date month
        if ((Key_Up_Trg & KEY_Add) || ((key_delay >= ChangeSpeed) && (Key_Restain & KEY_Add)))
        {
            key_delay = 0;
            if (Time_cache.month < 12)
                Time_cache.month++;
            else
                Time_cache.month = 1;
            number_flash_cache = Time_cache.month;
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][0], ((number_flash_cache / 10) % 10));
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][1], (number_flash_cache % 10));
        }
        else if ((Key_Up_Trg & KEY_Minus) || ((key_delay >= ChangeSpeed) && (Key_Restain & KEY_Minus)))
        {
            key_delay = 0;
            if (Time_cache.month > 1)
                Time_cache.month--;
            else
                Time_cache.month = 12;
            number_flash_cache = Time_cache.month;
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][0], ((number_flash_cache / 10) % 10));
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][1], (number_flash_cache % 10));
        }
        else
        {
            if (key_delay < ChangeSpeed)
                key_delay++;
        }
        if (Key_Up_Trg & KEY_Set)
        {
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][0], ((number_flash_cache / 10) % 10));
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][1], (number_flash_cache % 10));
            number_flash_cache = Time_cache.date;
            time_set_mode = 5;
        }
        break;
    case 5: //date date
        if ((Key_Up_Trg & KEY_Add) || ((key_delay >= ChangeSpeed) && (Key_Restain & KEY_Add)))
        {
            key_delay = 0;
            if (Time_cache.date < DayOfMon[Time_cache.month - 1][(Time_cache.year % 4 == 0) ? 1 : 0])
                Time_cache.date++;
            else
                Time_cache.date = 1;
            number_flash_cache = Time_cache.date;
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][0], ((number_flash_cache / 10) % 10));
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][1], (number_flash_cache % 10));
        }
        else if ((Key_Up_Trg & KEY_Minus) || ((key_delay >= ChangeSpeed) && (Key_Restain & KEY_Minus)))
        {
            key_delay = 0;
            if (Time_cache.date > 1)
                Time_cache.date--;
            else
                Time_cache.date = DayOfMon[Time_cache.month - 1][(Time_cache.year % 4 == 0) ? 1 : 0];
            number_flash_cache = Time_cache.date;
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][0], ((number_flash_cache / 10) % 10));
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][1], (number_flash_cache % 10));
        }
        else
        {
            if (key_delay < ChangeSpeed)
                key_delay++;
        }
        if (Key_Up_Trg & KEY_Set)
        {
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][0], ((number_flash_cache / 10) % 10));
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][1], (number_flash_cache % 10));
            time_set_mode = 6;
            if ((eland_data.time_display_format == 1) && (Time_cache.hr > 12))
                number_flash_cache = Time_cache.hr - 12;
            else
                number_flash_cache = Time_cache.hr;
        }
        break;
    case 6: //time hour
        if ((Key_Up_Trg & KEY_Add) || ((key_delay >= ChangeSpeed) && (Key_Restain & KEY_Add)))
        {
            key_delay = 0;
            if (Time_cache.hr < 23)
                Time_cache.hr++;
            else
                Time_cache.hr = 0;
            if ((eland_data.time_display_format == 1) && (Time_cache.hr > 12))
                number_flash_cache = Time_cache.hr - 12;
            else
                number_flash_cache = Time_cache.hr;
            if (eland_data.time_display_format == 1)
            {
                if (Time_cache.hr >= 12)
                    HT162x_LCD_AMPM_Set(TIME_PART, PM);
                else
                    HT162x_LCD_AMPM_Set(TIME_PART, AM);
            }
            else
                HT162x_LCD_AMPM_Set(TIME_PART, AMPMMAX);
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][0], ((number_flash_cache / 10) % 10));
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][1], (number_flash_cache % 10));
        }
        else if ((Key_Up_Trg & KEY_Minus) || ((key_delay >= ChangeSpeed) && (Key_Restain & KEY_Minus)))
        {
            key_delay = 0;
            if (Time_cache.hr > 0)
                Time_cache.hr--;
            else
                Time_cache.hr = 23;
            if ((eland_data.time_display_format == 1) && (Time_cache.hr > 12))
                number_flash_cache = Time_cache.hr - 12;
            else
                number_flash_cache = Time_cache.hr;
            if (eland_data.time_display_format == 1)
            {
                if (Time_cache.hr >= 12)
                    HT162x_LCD_AMPM_Set(TIME_PART, PM);
                else
                    HT162x_LCD_AMPM_Set(TIME_PART, AM);
            }
            else
                HT162x_LCD_AMPM_Set(TIME_PART, AMPMMAX);

            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][0], ((number_flash_cache / 10) % 10));
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][1], (number_flash_cache % 10));
        }
        else
        {
            if (key_delay < ChangeSpeed)
                key_delay++;
        }
        if (Key_Up_Trg & KEY_Set)
        {
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][0], ((number_flash_cache / 10) % 10));
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][1], (number_flash_cache % 10));
            number_flash_cache = Time_cache.min;
            time_set_mode = 7;
        }
        break;
    case 7: //time minute
        if ((Key_Up_Trg & KEY_Add) || ((key_delay >= ChangeSpeed) && (Key_Restain & KEY_Add)))
        {
            key_delay = 0;
            if (Time_cache.min < 59)
                Time_cache.min++;
            else
                Time_cache.min = 0;

            number_flash_cache = Time_cache.min;
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][0], ((number_flash_cache / 10) % 10));
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][1], (number_flash_cache % 10));
        }
        else if ((Key_Up_Trg & KEY_Minus) || ((key_delay >= ChangeSpeed) && (Key_Restain & KEY_Minus)))
        {
            key_delay = 0;
            if (Time_cache.min > 0)
                Time_cache.min--;
            else
                Time_cache.min = 59;

            number_flash_cache = Time_cache.min;
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][0], ((number_flash_cache / 10) % 10));
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][1], (number_flash_cache % 10));
        }
        else
        {
            if (key_delay < ChangeSpeed)
                key_delay++;
        }
        if (Key_Up_Trg & KEY_Set)
        {
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][0], ((number_flash_cache / 10) % 10));
            HT162x_LCD_Num_Set(Clock_number_table[time_set_mode][1], (number_flash_cache % 10));
            time_set_mode = 0;
            memset(&mcutimeCache, 0, sizeof(_eland_date_time_t));
            ELAND_Time_Convert(&Time_cache, &mcutimeCache, MICO_2_MCU);
            mcutimeCache.week = CaculateWeekDay((int)(Time_cache.year + 2000), Time_cache.month, Time_cache.date);
            RTC_Time_Set(mcutimeCache);
            memcpy(&CurrentMicoTime, &Time_cache, sizeof(mico_rtc_time_t));
            changeflag |= 1;
        }
        break;
    default:
        break;
    }
    if (changeflag != 0) //syncchronize
    {
        if (MCU_Refreshed == REFRESH_NONE)
        {
            if (changeflag & 1)
                MCU_Refreshed = REFRESH_TIME;
            else if (changeflag & 2)
                MCU_Refreshed = REFRESH_ALARM;
        }
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
    if ((AlarmOccurred == TRUE) && (time_set_mode < 3)) //1s update time
    {
        HT162x_LCD_Time_Display(TIME_PART, CurrentMicoTime);
        HT162x_LCD_Date_Display(TIME_PART, CurrentMicoTime);
        /* back light turn brightest */
        if ((Today_Second > eland_data.night_mode_begin_time) || (Today_Second < eland_data.night_mode_end_time))
            RGBLED_Set_Brightness(eland_data.brightness_night);
        else
            RGBLED_Set_Brightness(eland_data.brightness_normal);

        AlarmOccurred = FALSE;
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
    static _ELAND_MODE_t Eland_modeBak = ELAND_MODE_MAX;
    if (Eland_mode != Eland_modeBak)
    {
        /**refresh wifi**/
        LCD_Display_Rssi_State(eland_state);
        /**refresh time**/
        HT162x_LCD_Time_Display(TIME_PART, CurrentMicoTime);
        /**refresh date**/
        HT162x_LCD_Date_Display(TIME_PART, CurrentMicoTime);
        /*clear alarm -- next alarm*/
        if (Key_Count & KEY_MON)
            HT162x_LCD_Change_Pixel(COM7, SEG11, RESET);
        else if (Key_Count & KEY_AlarmMode)
            HT162x_LCD_Change_Pixel(COM7, SEG11, SET);
        /*clear alarm -- snooze*/
        HT162x_LCD_Change_Pixel(COM7, SEG13, RESET);
        /*clear alarm -- alarm date and time */
        // for (i = (uint8_t)Serial_11; i < (uint8_t)Serial_17; i++)
        //     HT162x_LCD_Num_Set((LCD_Digital_Serial_t)i, 10);
        /**refresh alarm time**/
        //HT162x_LCD_Time_Display(ALARM_PART, alarm_data.moment_time);
        /*clear alarm -- alarm week*/
        HT162x_LCD_Week_Set(ALARM_PART, WEEKDAYMAX);
        /**SHOW the line**/
        HT162x_LCD_Change_Pixel(COM7, SEG33, SET);

        Eland_modeBak = Eland_mode;
    }
    if (WakeupOccurred == TRUE) //500ms point flash
    {
        WakeupOccurred = FALSE;
        HT162x_LCD_Toggle_Pixel(COM0, SEG32);
        HT162x_LCD_Toggle_Pixel(COM0, SEG33);
        LCD_Display_Rssi_State(eland_state);
    }
    if (AlarmOccurred == TRUE) //1s update time
    {
        HT162x_LCD_Time_Display(TIME_PART, CurrentMicoTime);
        HT162x_LCD_Date_Display(TIME_PART, CurrentMicoTime);
        /* back light turn brightest */
        if ((Today_Second > eland_data.night_mode_begin_time) || (Today_Second < eland_data.night_mode_end_time))
            RGBLED_Set_Brightness(eland_data.brightness_night);
        else
            RGBLED_Set_Brightness(eland_data.brightness_normal);
        AlarmOccurred = FALSE;
    }
    ALARM_Alarm_Refresh();
}
/**
 ****************************************************************************
 * @Function : void ALARM_Alarm_Refresh(void)
 * @File     : lcd_display.c
 * @Program  : none
 * @Created  : 2018/1/11 by seblee
 * @Brief    : refresh alarm data
 * @Version  : V1.0
**/
void ALARM_Alarm_Refresh(void)
{
    uint8_t i;
    if (!Alarm_need_Refresh)
        return;
    Alarm_need_Refresh = FALSE;
    /*refresh alarm date*/
    if (alarm_data_eland.alarm_repeat == 5)
        HT162x_LCD_Date_Display(ALARM_PART, alarm_data_eland.moment_time);
    else
    {
        /*refresh alarm date*/
        for (i = (uint8_t)Serial_11; i < (uint8_t)Serial_17; i++)
            HT162x_LCD_Num_Set((LCD_Digital_Serial_t)i, 10);
        /**refresh week**/
        for (i = 0; i < 7; i++)
            HT162x_LCD_Change_Pixel(COM7, ALARM_PART_Week_seg[i], (FlagStatus)(((1 << i) & alarm_data_eland.alarm_on_days_of_week)));
    }
    /*refresh alarm time*/
    HT162x_LCD_Time_Display(ALARM_PART, alarm_data_eland.moment_time);
    /*refresh snooze point*/
    HT162x_LCD_Change_Pixel(COM7, SEG13, (FlagStatus)alarm_data_eland.snooze_count);
}
