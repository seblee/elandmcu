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
__no_init __ELAND_DATA_2_MCU_t eland_data;
__no_init _alarm_mcu_data_t alarm_data_simple;
__no_init _alarm_mcu_data_t alarm_data_display;
__no_init bool Alarm_is_empty;
uint8_t Alarm_led_brigh = 100;
bool Alarm_need_Refresh = TRUE;
bool ELAND_DATA_Refreshed = TRUE;
__no_init _ELAND_MODE_t Eland_mode;
uint8_t alarm_skip_flag = 0;
uint8_t alarm_skip_flash_count = 0;
uint8_t alarm_snooze_flash_count = 0; //20ms
__eland_error_t eland_flash_state = EL_ERROR_NONE;

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
const uint8_t Position_alarm_simple[7] = {8, 9, 0, 1, 2, 3, 4};

const LCD_Wifi_Rssi_t Rssi_array[5] = {LEVEL0, LEVEL1, LEVEL2, LEVEL3, LEVEL4};
/* Private function prototypes -----------------------------------------------*/
static void test_display(void);
static void check_key(void);
static void LCD_Check_LCD(void);
static void LCD_Check_Clock(void);
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
    eland_data.led_normal = 80;
    eland_data.led_night = 20;
    eland_data.night_mode_enabled = 0;
    eland_data.night_mode_begin_time = 79200;
    eland_data.night_mode_end_time = 21600;

    Alarm_is_empty = TRUE;
    AlarmOccurred = FALSE;
    Today_Second = 0;
    WakeupOccurred = FALSE;
    Key_Light_counter = 0;
    /***init display alarm***/
    memset(&alarm_data_simple, 0, sizeof(_alarm_mcu_data_t));
    memset(&alarm_data_display, 0, sizeof(_alarm_mcu_data_t));
    /***init display time***/
    CurrentMicoTime.sec = 0;
    CurrentMicoTime.min = 0;
    CurrentMicoTime.hr = 0;
    CurrentMicoTime.weekday = 7;
    CurrentMicoTime.date = 1;
    CurrentMicoTime.month = 1;
    CurrentMicoTime.year = 0;

    eland_state = ElandNone;
    RSSI_Value = LEVELNUM;
    Eland_mode = ELAND_MODE_NONE;
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

    switch (Eland_mode)
    {
    case ELAND_CLOCK_MON:
    case ELAND_CLOCK_ALARM:
        HT162x_LCD_TCP_STATE_Set(RESET);
        HT162x_LCD_RSSI_Set(LEVEL0);
        break;
    case ELAND_NC:
    case ELAND_NA:
        HT162x_LCD_TCP_STATE_Set((state > CONNECTED_NET) ? RESET : SET);
        if (state < WifyConnected)
            HT162x_LCD_RSSI_Set(LEVEL0);
        else
            HT162x_LCD_RSSI_Set(RSSI_Value);
        break;
    case ELAND_AP:
        HT162x_LCD_TCP_STATE_Set(RESET);
        if (rssi_value == LEVEL0)
            rssi_value = LEVEL4;
        else
            rssi_value = LEVEL0;
        HT162x_LCD_RSSI_Set(rssi_value);
        break;
    case ELAND_OTA:
        HT162x_LCD_TCP_STATE_Set(RESET);
        HT162x_LCD_RSSI_Set(LEVEL0);
        break;
    case ELAND_TEST:
        break;
    default:
        break;
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
    static uint8_t time_set_mode = 0, number_flash_flag = 0, number_flash_cache;
    static uint8_t key_delay = 0;
    static mico_rtc_time_t Time_cache;
    static _alarm_mcu_data_t alarm_cache;
    _eland_date_time_t mcutimeCache;
    static _ELAND_MODE_t Eland_modeBak = ELAND_MODE_MAX;
    static LCD_AMPM_Distinguish_t ampm_cache;
    /***** bit0 time bit1 alarm*************/
    static uint8_t changeflag = 0;
    if (Eland_mode != Eland_modeBak)
    {
        if (Eland_mode != ELAND_TEST)
        { /**refresh wifi**/
            HT162x_LCD_RSSI_Set(LEVEL0);
            HT162x_LCD_Change_Pixel(COM7, SEG21, RESET);
            /**refresh time**/
            HT162x_LCD_Time_Display(TIME_PART, CurrentMicoTime);
            /**refresh date**/
            HT162x_LCD_Date_Display(TIME_PART, CurrentMicoTime);
            /**SHOW the line**/
            HT162x_LCD_Change_Pixel(COM7, SEG33, SET);
            /**backup mode**/
            if ((time_set_mode != 0) && (number_flash_flag == 0))
            {
                HT162x_LCD_Double_Digits_Write(Position_alarm_simple[time_set_mode - 1], number_flash_cache, ((time_set_mode == 2) || (time_set_mode == 3) || (time_set_mode == 7)) ? 2 : 1);
                number_flash_flag = 1;
            }
            time_set_mode = 0;
        }
        Eland_modeBak = Eland_mode;
    }
    if (Eland_mode == ELAND_TEST)
    {
        test_display();
        return;
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
        if ((Key_Up_Trg & KEY_Set) && (Eland_mode == ELAND_CLOCK_ALARM))
        {
            time_set_mode = 1;
            memcpy(&alarm_cache, &alarm_data_simple, sizeof(_alarm_mcu_data_t));
            if ((eland_data.time_display_format == 1) && (alarm_cache.moment_time.hr > 12))
                number_flash_cache = alarm_cache.moment_time.hr - 12;
            else
                number_flash_cache = alarm_cache.moment_time.hr;
        }
        break;
    case 1: //alarm hour
        if ((Key_Up_Trg & KEY_Add) || ((key_delay >= ChangeSpeed) && (Key_Short_Restain & KEY_Add)))
        {
            key_delay = 0;
            if (alarm_cache.moment_time.hr < 24)
                alarm_cache.moment_time.hr++;
            else
                alarm_cache.moment_time.hr = 0;

            if ((eland_data.time_display_format == 1) && (alarm_cache.moment_time.hr > 12))
                number_flash_cache = alarm_cache.moment_time.hr - 12;
            else
                number_flash_cache = alarm_cache.moment_time.hr;
            if (eland_data.time_display_format == 1)
            {
                if (alarm_cache.moment_time.hr >= 12)
                    HT162x_LCD_AMPM_Set(ALARM_PART, PM);
                else
                    HT162x_LCD_AMPM_Set(ALARM_PART, AM);
            }
            else
                HT162x_LCD_AMPM_Set(ALARM_PART, AMPMMAX);
            HT162x_LCD_Double_Digits_Write(Position_alarm_simple[0], number_flash_cache, 1);
        }
        else if ((Key_Up_Trg & KEY_Minus) || ((key_delay >= ChangeSpeed) && (Key_Short_Restain & KEY_Minus)))
        {
            key_delay = 0;
            if (alarm_cache.moment_time.hr > 0)
                alarm_cache.moment_time.hr--;
            else
                alarm_cache.moment_time.hr = 23;
            if ((eland_data.time_display_format == 1) && (alarm_cache.moment_time.hr > 12))
                number_flash_cache = alarm_cache.moment_time.hr - 12;
            else
                number_flash_cache = alarm_cache.moment_time.hr;
            if (eland_data.time_display_format == 1)
            {
                if (alarm_cache.moment_time.hr >= 12)
                    HT162x_LCD_AMPM_Set(ALARM_PART, PM);
                else
                    HT162x_LCD_AMPM_Set(ALARM_PART, AM);
            }
            else
                HT162x_LCD_AMPM_Set(ALARM_PART, AMPMMAX);

            HT162x_LCD_Double_Digits_Write(Position_alarm_simple[0], number_flash_cache, 1);
        }
        else
        {
            if (key_delay < ChangeSpeed)
                key_delay++;
        }
        if (Key_Up_Trg & KEY_Set)
        {
            HT162x_LCD_Double_Digits_Write(Position_alarm_simple[time_set_mode - 1], number_flash_cache, 1);
            time_set_mode = 2;
            number_flash_cache = alarm_cache.moment_time.min;
        }
        break;
    case 2: //alarm minute
        if ((Key_Up_Trg & KEY_Add) || ((key_delay >= ChangeSpeed) && (Key_Short_Restain & KEY_Add)))
        {
            key_delay = 0;
            if (alarm_cache.moment_time.min < 59)
                alarm_cache.moment_time.min++;
            else
                alarm_cache.moment_time.min = 0;
            number_flash_cache = alarm_cache.moment_time.min;
            HT162x_LCD_Double_Digits_Write(Position_alarm_simple[time_set_mode - 1], number_flash_cache, 2);
        }
        else if ((Key_Up_Trg & KEY_Minus) || ((key_delay >= ChangeSpeed) && (Key_Short_Restain & KEY_Minus)))
        {
            key_delay = 0;
            if (alarm_cache.moment_time.min > 0)
                alarm_cache.moment_time.min--;
            else
                alarm_cache.moment_time.min = 59;
            number_flash_cache = alarm_cache.moment_time.min;
            HT162x_LCD_Double_Digits_Write(Position_alarm_simple[time_set_mode - 1], number_flash_cache, 2);
        }
        else
        {
            if (key_delay < ChangeSpeed)
                key_delay++;
        }
        if (Key_Up_Trg & KEY_Set)
        {
            memcpy(&alarm_data_simple, &alarm_cache, sizeof(_alarm_mcu_data_t));
            HT162x_LCD_Double_Digits_Write(Position_alarm_simple[time_set_mode - 1], number_flash_cache, 2);
            changeflag |= 2;
            time_set_mode = 0;
            number_flash_cache = 0;
        }
        break;
    case 3: //date year
        if ((Key_Up_Trg & KEY_Add) || ((key_delay >= ChangeSpeed) && (Key_Short_Restain & KEY_Add)))
        {
            key_delay = 0;
            if (Time_cache.year < 37)
                Time_cache.year++;
            else
                Time_cache.year = 0;
            number_flash_cache = Time_cache.year;
            HT162x_LCD_Double_Digits_Write(Position_alarm_simple[time_set_mode - 1], number_flash_cache, 2);
        }
        else if ((Key_Up_Trg & KEY_Minus) || ((key_delay >= ChangeSpeed) && (Key_Short_Restain & KEY_Minus)))
        {
            key_delay = 0;
            if (Time_cache.year > 0)
                Time_cache.year--;
            else
                Time_cache.year = 37;
            number_flash_cache = Time_cache.year;
            HT162x_LCD_Double_Digits_Write(Position_alarm_simple[time_set_mode - 1], number_flash_cache, 2);
        }
        else
        {
            if (key_delay < ChangeSpeed)
                key_delay++;
        }
        if (Key_Up_Trg & KEY_Set)
        {
            HT162x_LCD_Double_Digits_Write(Position_alarm_simple[time_set_mode - 1], number_flash_cache, 2);
            number_flash_cache = Time_cache.month;
            time_set_mode = 4;
        }
        break;
    case 4: //date month
        if ((Key_Up_Trg & KEY_Add) || ((key_delay >= ChangeSpeed) && (Key_Short_Restain & KEY_Add)))
        {
            key_delay = 0;
            if (Time_cache.month < 12)
                Time_cache.month++;
            else
                Time_cache.month = 1;
            number_flash_cache = Time_cache.month;
            HT162x_LCD_Double_Digits_Write(Position_alarm_simple[time_set_mode - 1], number_flash_cache, 1);
        }
        else if ((Key_Up_Trg & KEY_Minus) || ((key_delay >= ChangeSpeed) && (Key_Short_Restain & KEY_Minus)))
        {
            key_delay = 0;
            if (Time_cache.month > 1)
                Time_cache.month--;
            else
                Time_cache.month = 12;
            number_flash_cache = Time_cache.month;
            HT162x_LCD_Double_Digits_Write(Position_alarm_simple[time_set_mode - 1], number_flash_cache, 1);
        }
        else
        {
            if (key_delay < ChangeSpeed)
                key_delay++;
        }
        if (Key_Up_Trg & KEY_Set)
        {
            HT162x_LCD_Double_Digits_Write(Position_alarm_simple[time_set_mode - 1], number_flash_cache, 1);
            if (Time_cache.date > DayOfMon[Time_cache.month - 1][(Time_cache.year % 4 == 0) ? 1 : 0])
                Time_cache.date = DayOfMon[Time_cache.month - 1][(Time_cache.year % 4 == 0) ? 1 : 0];
            number_flash_cache = Time_cache.date;
            time_set_mode = 5;
        }
        break;
    case 5: //date date
        if ((Key_Up_Trg & KEY_Add) || ((key_delay >= ChangeSpeed) && (Key_Short_Restain & KEY_Add)))
        {
            key_delay = 0;
            if (Time_cache.date < DayOfMon[Time_cache.month - 1][(Time_cache.year % 4 == 0) ? 1 : 0])
                Time_cache.date++;
            else
                Time_cache.date = 1;
            number_flash_cache = Time_cache.date;
            HT162x_LCD_Double_Digits_Write(Position_alarm_simple[time_set_mode - 1], number_flash_cache, 1);
        }
        else if ((Key_Up_Trg & KEY_Minus) || ((key_delay >= ChangeSpeed) && (Key_Short_Restain & KEY_Minus)))
        {
            key_delay = 0;
            if (Time_cache.date > 1)
                Time_cache.date--;
            else
                Time_cache.date = DayOfMon[Time_cache.month - 1][(Time_cache.year % 4 == 0) ? 1 : 0];
            number_flash_cache = Time_cache.date;
            HT162x_LCD_Double_Digits_Write(Position_alarm_simple[time_set_mode - 1], number_flash_cache, 1);
        }
        else
        {
            if (key_delay < ChangeSpeed)
                key_delay++;
        }
        if (Key_Up_Trg & KEY_Set)
        {
            HT162x_LCD_Double_Digits_Write(Position_alarm_simple[time_set_mode - 1], number_flash_cache, 1);
            time_set_mode = 6;
            if ((eland_data.time_display_format == 1) && (Time_cache.hr > 12))
                number_flash_cache = Time_cache.hr - 12;
            else
                number_flash_cache = Time_cache.hr;
        }
        break;
    case 6: //time hour
        if ((Key_Up_Trg & KEY_Add) || ((key_delay >= ChangeSpeed) && (Key_Short_Restain & KEY_Add)))
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
            HT162x_LCD_Double_Digits_Write(Position_alarm_simple[time_set_mode - 1], number_flash_cache, 1);
        }
        else if ((Key_Up_Trg & KEY_Minus) || ((key_delay >= ChangeSpeed) && (Key_Short_Restain & KEY_Minus)))
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
                HT162x_LCD_AMPM_Set(TIME_PART, (Time_cache.hr >= 12) ? PM : AM);
                ampm_cache = (Time_cache.hr >= 12) ? PM : AM;
            }
            else
            {
                HT162x_LCD_AMPM_Set(TIME_PART, AMPMMAX);
                ampm_cache = AMPMMAX;
            }

            HT162x_LCD_Double_Digits_Write(Position_alarm_simple[time_set_mode - 1], number_flash_cache, 1);
        }
        else
        {
            if (key_delay < ChangeSpeed)
                key_delay++;
        }
        if (Key_Up_Trg & KEY_Set)
        {
            HT162x_LCD_Double_Digits_Write(Position_alarm_simple[time_set_mode - 1], number_flash_cache, 1);
            number_flash_cache = Time_cache.min;
            time_set_mode = 7;
        }
        break;
    case 7: //time minute
        if ((Key_Up_Trg & KEY_Add) || ((key_delay >= ChangeSpeed) && (Key_Short_Restain & KEY_Add)))
        {
            key_delay = 0;
            if (Time_cache.min < 59)
                Time_cache.min++;
            else
                Time_cache.min = 0;

            number_flash_cache = Time_cache.min;
            HT162x_LCD_Double_Digits_Write(Position_alarm_simple[time_set_mode - 1], number_flash_cache, 2);
        }
        else if ((Key_Up_Trg & KEY_Minus) || ((key_delay >= ChangeSpeed) && (Key_Short_Restain & KEY_Minus)))
        {
            key_delay = 0;
            if (Time_cache.min > 0)
                Time_cache.min--;
            else
                Time_cache.min = 59;

            number_flash_cache = Time_cache.min;
            HT162x_LCD_Double_Digits_Write(Position_alarm_simple[time_set_mode - 1], number_flash_cache, 2);
        }
        else
        {
            if (key_delay < ChangeSpeed)
                key_delay++;
        }
        if (Key_Up_Trg & KEY_Set)
        {
            HT162x_LCD_Double_Digits_Write(Position_alarm_simple[time_set_mode - 1], number_flash_cache, 2);
            time_set_mode = 8;
        }
        break;
    case 8: //AM PM
        if ((Key_Up_Trg & KEY_Minus) || (Key_Up_Trg & KEY_Add))
        {
            if (eland_data.time_display_format == 1)
            {
                eland_data.time_display_format = 2;
                ampm_cache = AMPMMAX;
            }
            else
            {
                eland_data.time_display_format = 1;
                if (Time_cache.hr >= 12)
                    ampm_cache = PM;
                else
                    ampm_cache = AM;
            }
            HT162x_LCD_AMPM_Set(TIME_PART, ampm_cache);
        }

        if (Key_Up_Trg & KEY_Set)
        {
            HT162x_LCD_Double_Digits_Write(Position_alarm_simple[time_set_mode - 1], number_flash_cache, 2);
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
    if (WakeupOccurred == TRUE) //500ms point flash
    {
        WakeupOccurred = FALSE;
        HT162x_LCD_FLASH_Point_Toggle();
        alarm_snooze_flash_count++;
        if ((time_set_mode != 0) && (time_set_mode != 8))
        {
            if (number_flash_flag == 0)
            {
                HT162x_LCD_Double_Digits_Write(Position_alarm_simple[time_set_mode - 1], number_flash_cache, ((time_set_mode == 2) || (time_set_mode == 3) || (time_set_mode == 7)) ? 2 : 1);
                number_flash_flag = 1;
            }
            else
            {
                HT162x_LCD_Double_Digits_Write(Position_alarm_simple[time_set_mode - 1], number_flash_cache, 0);
                number_flash_flag = 0;
            }
        }
        else if (time_set_mode == 8)
        {
            if (number_flash_flag == 0)
            {
                number_flash_flag = 1;
                HT162x_LCD_AMPM_Set(TIME_PART, ampm_cache);
            }
            else
            {
                number_flash_flag = 0;
                HT162x_LCD_AMPM_Set(TIME_PART, AMPMMAX);
            }
        }
    }
    if (AlarmOccurred == TRUE) //1s update time
    {
        alarm_skip_flash_count++;
        if (time_set_mode < 3)
        {
            HT162x_LCD_Time_Display(TIME_PART, CurrentMicoTime);
            HT162x_LCD_Date_Display(TIME_PART, CurrentMicoTime);
        }
        AlarmOccurred = FALSE;
        /*refresh brightness*/
        Brightness_refresh();
    }
    if (changeflag & 3) //syncchronize
    {
        if (MCU_Refreshed == REFRESH_NONE)
        {
            if (changeflag & 1)
            {
                MCU_Refreshed = REFRESH_TIME;
                changeflag &= 0xfe;
            }
            else if (changeflag & 2)
            {
                MCU_Refreshed = REFRESH_ALARM;
                changeflag &= 0xfd;
            }
        }
    }
    ALARM_Alarm_Refresh();
    Eland_data_Refresh();
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
    /***** bit0 time bit1 alarm*************/
    static uint8_t changeflag = 5;
    static uint8_t second_count = 0;
    if (Eland_mode != Eland_modeBak)
    {
        if (Eland_mode == ELAND_OTA)
        {
            /**refresh date**/
            HT162x_LCD_Date_Display(TIME_CLEAR, CurrentMicoTime);
            /**refresh time**/
            HT162x_LCD_Time_Display(TIME_CLEAR, CurrentMicoTime);
            /**SHOW the line**/
            HT162x_LCD_Change_Pixel(COM7, SEG33, RESET);

            HT162x_LCD_Change_Pixel(COM0, SEG32, RESET);
            HT162x_LCD_Change_Pixel(COM0, SEG33, RESET);
            /**refresh alarm**/
            Eland_alarm_display(RESET);
        }
        else if (Eland_mode == ELAND_TEST)
            HT162x_LCD_Clear(SET);
        else
        {
            /**refresh time**/
            HT162x_LCD_Time_Display(TIME_PART, CurrentMicoTime);
            /**refresh date**/
            HT162x_LCD_Date_Display(TIME_PART, CurrentMicoTime);
            /**SHOW the line**/
            HT162x_LCD_Change_Pixel(COM7, SEG33, SET);
        }
        if (Eland_modeBak == ELAND_MODE_MAX)
            changeflag = 4;
        /**refresh wifi**/
        LCD_Display_Rssi_State(eland_state);
        Eland_modeBak = Eland_mode;
    }
    if (Eland_mode == ELAND_OTA)
    {
        if (WakeupOccurred == TRUE) //500ms point flash
        {
            WakeupOccurred = FALSE;
            ota_display();
        }
        return;
    }
    else if (Eland_mode == ELAND_TEST)
    {
        test_display();
        return;
    }
    if (WakeupOccurred == TRUE) //500ms point flash
    {
        WakeupOccurred = FALSE;
        alarm_snooze_flash_count++;
        HT162x_LCD_FLASH_Point_Toggle();
        LCD_Display_Rssi_State(eland_state);
    }
    if (AlarmOccurred == TRUE) //1s update time
    {
        alarm_skip_flash_count++;
        AlarmOccurred = FALSE;
        if (eland_state < CONNECTED_NET)
        {
            if (second_count++ >= 60)
            {
                second_count = 0;
                changeflag |= 1;
            }
        }
        HT162x_LCD_Time_Display(TIME_PART, CurrentMicoTime);
        HT162x_LCD_Date_Display(TIME_PART, CurrentMicoTime);

        /*refresh brightness*/
        Brightness_refresh();
    }
    ALARM_Alarm_Refresh();
    Eland_data_Refresh();

    if (changeflag & 7) // && (eland_state >= ElandBegin)) //syncchronize
    {
        if (MCU_Refreshed == REFRESH_NONE)
        {
            if (changeflag & 1)
            {
                MCU_Refreshed = REFRESH_TIME;
                changeflag &= ~1;
            }
            else if (changeflag & 2)
            {
                MCU_Refreshed = REFRESH_ALARM;
                changeflag &= ~2;
            }
            else if (changeflag & 4)
            {
                MCU_Refreshed = REFRESH_ELAND_DATA;
                changeflag &= ~4;
            }
        }
    }
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
    static uint8_t alarm_NA_flash_count = 0; //20ms

    if (alarm_data_display.mode == ELAND_NA)
    {
        if (Alarm_need_Refresh)
        {
            alarm_NA_flash_count = 0;
            Alarm_need_Refresh = FALSE;
        }
        if (alarm_NA_flash_count == 0)
            Eland_alarm_display(SET);
        else if (alarm_NA_flash_count == 35)
            Eland_alarm_display(RESET);
        if (alarm_NA_flash_count++ >= 50)
            alarm_NA_flash_count = 0;
        return;
    }
    if (alarm_data_display.alarm_skip > 0)
    {
        if (alarm_skip_flash_count == 1)
            Eland_alarm_display(SET);
        else if (alarm_skip_flash_count >= 3)
        {
            Eland_alarm_display(RESET);
            alarm_skip_flash_count = 0;
        }
        return;
    }
    if (alarm_data_display.alarm_state == ALARM_SNOOZ_STOP)
    {
        if (alarm_snooze_flash_count == 1)
            HT162x_LCD_Change_Pixel(COM7, SEG13, (FlagStatus)(alarm_data_display.snooze_enabled));
        else if (alarm_snooze_flash_count >= 2)
        {
            HT162x_LCD_Change_Pixel(COM7, SEG13, RESET);
            alarm_snooze_flash_count = 0;
        }
    }

    if (!Alarm_need_Refresh)
        return;
    alarm_NA_flash_count = 0;
    alarm_skip_flash_count = 10;
    Alarm_need_Refresh = FALSE;
    Eland_alarm_display(SET);
    /*set alarm color*/
}
/**
 ****************************************************************************
 * @Function : void Eland_data_Refresh(void)
 * @File     : lcd_display.c
 * @Program  : none
 * @Created  : 2018/1/11 by seblee
 * @Brief    : refresh data refresh
 * @Version  : V1.0
**/
void Eland_data_Refresh(void)
{
    static __ELAND_DATA_2_MCU_t eland_data_bak;
    if (!ELAND_DATA_Refreshed)
        return;
    ELAND_DATA_Refreshed = FALSE;
    /*refresh brightness*/
    Brightness_refresh();
    if (eland_data.time_display_format != eland_data_bak.time_display_format)
        Alarm_need_Refresh = TRUE;

    memcpy(&eland_data_bak, &eland_data, sizeof(__ELAND_DATA_2_MCU_t));
}

void LCD_OtherMode(void)
{
    static bool display_flag = TRUE;

    if (Eland_mode == ELAND_TEST)
    {
        test_display();
        return;
    }
    Eland_mode = ELAND_MODE_NONE;
    if (WakeupOccurred == TRUE) //500ms point flash
    {
        WakeupOccurred = FALSE;
        HT162x_LCD_FLASH_Point_Toggle();
        LCD_Display_Rssi_State(eland_state);

        if (display_flag)
        {
            display_flag = FALSE;
            /**refresh date**/
            HT162x_LCD_Date_Display(TIME_PART, CurrentMicoTime);
        }
        else
        {
            display_flag = TRUE;
            /**refresh date**/
            HT162x_LCD_Date_Display(TIME_CLEAR, CurrentMicoTime);
        }
    }
    if (AlarmOccurred == TRUE) //1s update time
    {
        HT162x_LCD_Time_Display(TIME_PART, CurrentMicoTime);
        // HT162x_LCD_Date_Display(TIME_PART, CurrentMicoTime);
        AlarmOccurred = FALSE;
    }
    ALARM_Alarm_Refresh();
    Eland_data_Refresh();
}
/**
 ****************************************************************************
 * @Function : void Brightness_refresh(void)
 * @File     : lcd_display.c
 * @Program  : none
 * @Created  : 2018/3/26 by seblee
 * @Brief    : refresh brightness
 * @Version  : V1.0
**/
void Brightness_refresh(void)
{
    bool now_is_night = FALSE;

    if ((eland_data.night_mode_enabled) &&
        (Eland_mode != ELAND_CLOCK_MON) &&
        (Eland_mode != ELAND_CLOCK_ALARM))
    {
        if (eland_data.night_mode_end_time < eland_data.night_mode_begin_time)
        {
            if (Today_Second < eland_data.night_mode_end_time)
                now_is_night = TRUE;
            else if (Today_Second < eland_data.night_mode_begin_time)
                now_is_night = FALSE;
            else
                now_is_night = TRUE;
        }
        else
        {
            if (Today_Second < eland_data.night_mode_begin_time)
                now_is_night = FALSE;
            else if (Today_Second < eland_data.night_mode_end_time)
                now_is_night = TRUE;
            else
                now_is_night = FALSE;
        }
    }
    else
        now_is_night = FALSE;
    /* back light turn brightest */
    if ((Key_Light_counter < SW_LIGHT_TIMES) || (Eland_mode == ELAND_AP))
    {
        RGBLED_Set_Brightness(100);
    }
    else
    {
        if (now_is_night)
            RGBLED_Set_Brightness(eland_data.brightness_night);
        else
            RGBLED_Set_Brightness(eland_data.brightness_normal);
    }
    if (now_is_night)
    {
        if (Alarm_led_brigh != eland_data.led_night)
        {
            Alarm_need_Refresh = TRUE;
            Alarm_led_brigh = eland_data.led_night;
        }
    }
    else
    {
        if (Alarm_led_brigh != eland_data.led_normal)
        {
            Alarm_need_Refresh = TRUE;
            Alarm_led_brigh = eland_data.led_normal;
        }
    }
}
/**
 ****************************************************************************
 * @Function : void Eland_alarm_display(FlagStatus status)
 * @File     : lcd_display.c
 * @Program  : status  SET:display RESET:clear
 * @Created  : 2018/4/8 by seblee
 * @Brief    : display alarm
 * @Version  : V1.0
**/
void Eland_alarm_display(FlagStatus status)
{
    if ((status == RESET) || Alarm_is_empty || (Eland_mode == ELAND_OTA) || (Eland_mode == ELAND_CLOCK_MON))
    {
        /*Clear alarm date*/
        HT162x_LCD_Date_Display(ALARM_CLEAR, alarm_data_display.moment_time);
        /*Clear alarm time*/
        HT162x_LCD_Time_Display(ALARM_CLEAR, alarm_data_display.moment_time);
        /*refresh snooze point*/
        HT162x_LCD_Change_Pixel(COM7, SEG13, RESET);
        /*next alarm display*/
        HT162x_LCD_Change_Pixel(COM7, SEG11, RESET);
    }
    else
    {
        /*refresh alarm time*/
        if (Eland_mode == ELAND_CLOCK_ALARM)
            HT162x_LCD_Date_Display(ALARM_CLEAR, alarm_data_display.moment_time);
        else
            HT162x_LCD_Date_Display(ALARM_PART, alarm_data_display.moment_time);
        /*refresh alarm time*/
        HT162x_LCD_Time_Display(ALARM_PART, alarm_data_display.moment_time);
        /*refresh snooze point*/
        HT162x_LCD_Change_Pixel(COM7, SEG13, (FlagStatus)(alarm_data_display.snooze_enabled));
        /*next alarm display*/
        HT162x_LCD_Change_Pixel(COM7, SEG11, (FlagStatus)(alarm_data_display.next_alarm));
    }
    RGBLED_Color_Set((__eland_color_t)alarm_data_display.color, Alarm_led_brigh);
}

void ota_display(void)
{
    static uint8_t flag = 1;
    /**500ms once**/
    HT162x_LCD_Num_Set(Serial_07, (flag & 0x01) ? 10 : 0);
    HT162x_LCD_Num_Set(Serial_08, (flag & 0x02) ? 10 : 0);
    HT162x_LCD_Num_Set(Serial_09, (flag & 0x04) ? 10 : 0);
    HT162x_LCD_Num_Set(Serial_10, (flag & 0x08) ? 10 : 0);
    flag <<= 1;
    if (flag == 0x10)
        flag = 1;
}
static void test_display(void)
{
    if (Key_Count & KEY_Wifi) //NC/NA mode
        LCD_Check_LCD();
    else if (Key_Count & KEY_MON)
        LCD_Check_Clock();
    // else
    //     LCD_Check_LCD();
    check_key();
}
static void LCD_Check_LCD(void)
{
    static uint8_t check_step = 0;
    uint8_t step_changed = 0;
    uint8_t key_up = 10;
    uint8_t i;
    if (Key_Down_Trg & KEY_Add)
    {
        if (check_step < 37)
        {
            check_step++;
            step_changed = 1;
            key_up = 1;
        }
    }
    else if (Key_Down_Trg & KEY_Minus)
    {
        if (check_step > 0)
        {
            check_step--;
            step_changed = 1;
            key_up = 0;
        }
    }
    if (step_changed)
    {
        /****display all*******/
        if (check_step == 0)
            HT162x_LCD_Clear(SET);
        /****check 0-9*******/
        else if (check_step < 11)
        {
            if (check_step == 1)
                HT162x_LCD_Clear(RESET);
            for (i = Serial_01; i < Serial_MAX; i++)
                HT162x_LCD_Num_Set((LCD_Digital_Serial_t)i, check_step - 1);
            if ((key_up == 0) && (check_step == 10))
            {
                HT162x_LCD_Week_Set(TIME_PART, WEEKDAYMAX);  //week
                HT162x_LCD_Week_Set(ALARM_PART, WEEKDAYMAX); //week
            }
        }
        /****check week*******/
        else if (check_step < 18)
        {
            if (check_step == 11)
                for (i = Serial_01; i < Serial_MAX; i++)
                    HT162x_LCD_Num_Set((LCD_Digital_Serial_t)i, 10);
            HT162x_LCD_Week_Set(TIME_PART, (LCD_Week_Day_t)(check_step - 11));  //week
            HT162x_LCD_Week_Set(ALARM_PART, (LCD_Week_Day_t)(check_step - 11)); //week

            if ((key_up == 0) && (check_step == 17))
                HT162x_LCD_Change_Pixel(COM7, SEG22, RESET);
        }
        /****time am*******/
        else if (check_step == 18)
        {
            if (key_up == 0)
                HT162x_LCD_Change_Pixel(COM7, SEG23, RESET);
            HT162x_LCD_Change_Pixel(COM7, SEG22, SET);
        }
        /****time pm*******/
        else if (check_step == 19)
        {
            if (key_up == 0)
                HT162x_LCD_Change_Pixel(COM7, SEG38, RESET);
            HT162x_LCD_Change_Pixel(COM7, SEG23, SET);
        }
        /****alarm am*******/
        else if (check_step == 20)
        {
            if (key_up == 0)
                HT162x_LCD_Change_Pixel(COM7, SEG39, RESET);
            HT162x_LCD_Change_Pixel(COM7, SEG38, SET);
        }
        /****alarm pm*******/
        else if (check_step == 21)
        {
            HT162x_LCD_Change_Pixel(COM7, SEG39, SET);
        }
        /****check wifi*******/
        else if (check_step < 27)
        {
            HT162x_LCD_RSSI_Set(Rssi_array[check_step - 22]);
            if ((key_up == 0) && (check_step == 26))
                HT162x_LCD_Change_Pixel(COM7, SEG21, RESET);
        }
        /****red Exclamation*******/
        else if (check_step == 27)
        {
            if (key_up == 0)
                HT162x_LCD_Change_Pixel(COM7, SEG11, RESET);
            HT162x_LCD_Change_Pixel(COM7, SEG21, SET);
        }
        /****next alarm*******/
        else if (check_step == 28)
        {
            if (key_up == 0)
                HT162x_LCD_Change_Pixel(COM7, SEG13, RESET);
            HT162x_LCD_Change_Pixel(COM7, SEG11, SET);
        }
        /****snooze*******/
        else if (check_step == 29)
        {
            if (key_up == 0)
                HT162x_LCD_Change_Pixel(COM0, SEG08, RESET);
            HT162x_LCD_Change_Pixel(COM7, SEG13, SET);
        }
        /****point 1*******/
        else if (check_step == 30)
        {
            if (key_up == 0)
                HT162x_LCD_Change_Pixel(COM0, SEG16, RESET);
            HT162x_LCD_Change_Pixel(COM0, SEG08, SET);
        }
        /****point 2*******/
        else if (check_step == 31)
        {
            if (key_up == 0)
                HT162x_LCD_Change_Pixel(COM0, SEG33, RESET);
            HT162x_LCD_Change_Pixel(COM0, SEG16, SET);
        }
        /****point 3*******/
        else if (check_step == 32)
        {
            if (key_up == 0)
                HT162x_LCD_Change_Pixel(COM0, SEG32, RESET);
            HT162x_LCD_Change_Pixel(COM0, SEG33, SET);
        }
        /****point 4*******/
        else if (check_step == 33)
        {
            if (key_up == 0)
                HT162x_LCD_Change_Pixel(COM7, SEG15, RESET);
            HT162x_LCD_Change_Pixel(COM0, SEG32, SET);
        }
        /****point 5*******/
        else if (check_step == 34)
        {
            if (key_up == 0)
                HT162x_LCD_Change_Pixel(COM7, SEG07, RESET);
            HT162x_LCD_Change_Pixel(COM7, SEG15, SET);
        }
        /****point 6*******/
        else if (check_step == 35)
        {
            if (key_up == 0)
                HT162x_LCD_Change_Pixel(COM7, SEG31, RESET);
            HT162x_LCD_Change_Pixel(COM7, SEG07, SET);
        }
        /****point 7*******/
        else if (check_step == 36)
        {
            if (key_up == 0)
                HT162x_LCD_Change_Pixel(COM7, SEG30, RESET);
            HT162x_LCD_Change_Pixel(COM7, SEG31, SET);
        }
        /****point 8*******/
        else if (check_step == 37)
        {
            if (key_up == 0)
                HT162x_LCD_Change_Pixel(COM7, SEG30, RESET);
            HT162x_LCD_Change_Pixel(COM7, SEG30, SET);
        }
    }
}
static void LCD_Check_Clock(void)
{
    uint8_t i;
    static uint8_t brightness = 0;
    if (Key_Down_Trg & KEY_Minus)
    {
        if (brightness == 0)
            brightness = 10;
        else if (brightness == 10)
            brightness = 100;
        else
            brightness = 0;
        RGBLED_Set_Brightness(brightness);
    }
    if (AlarmOccurred == TRUE) //1s time flash
    {
        /****second******/
        HT162x_LCD_Double_Digits_Write(5, CurrentMicoTime.sec, 2);
        AlarmOccurred = FALSE;
    }
    if (eland_flash_state == EL_FLASH_OK)
    {
        eland_flash_state = EL_ERROR_NONE;
        for (i = Serial_07; i < Serial_11; i++)
            HT162x_LCD_Num_Set((LCD_Digital_Serial_t)i, 0);
    }
    else if (eland_flash_state == EL_FLASH_ERR)
    {
        eland_flash_state = EL_ERROR_NONE;
        for (i = Serial_07; i < Serial_11; i++)
            HT162x_LCD_Num_Set((LCD_Digital_Serial_t)i, 8);
    }
}

static void check_key(void)
{
    uint8_t i;
    uint8_t key_well_flag = 1;
    uint8_t key_press_number = 0;
    static uint8_t count = 0;
    static __eland_color_t color = ELAND_BLACK;
    key_press_number = 0;
    for (i = 0; i < (KEY_NUM - 1); i++)
    {
        if (Key_Count & (1 << i))
            key_press_number++;
    }
    if (key_press_number >= 3)
        key_well_flag = 0;
    key_press_number = 0;
    for (i = 4; i < 7; i++)
    {
        if (Key_Count & (1 << i))
            key_press_number++;
    }
    if (key_press_number >= 2)
        key_well_flag = 0;
    if (key_well_flag)
    {
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
            if (color == ELAND_RED)
                color = ELAND_GREEN;
            else if (color == ELAND_GREEN)
                color = ELAND_BLUE;
            else
                color = ELAND_RED;
            RGBLED_Color_Set(color, 100);
        }
    }
    else
    {
        if (count++ > 2)
        {
            count = 0;
            if (color >= ELAND_RED)
                color = ELAND_BLACK;
            else
                color = ELAND_RED;
            RGBLED_Color_Set(color, 100);
        }
    }
}
