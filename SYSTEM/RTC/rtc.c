/**
 ****************************************************************************
 * @Warning :Without permission from the author,Not for commercial use
 * @File    :rtc.c
 * @Author  :seblee
 * @date    :2017/10/23
 * @version :V 1.0.0
 *************************************************
 * @brief   :
 ****************************************************************************
**/
/* Private include -----------------------------------------------------------*/
#include "rtc.h"
//#include "lcd_eland.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
RTC_InitTypeDef RTC_InitStr;
RTC_TimeTypeDef RTC_TimeStr;
RTC_DateTypeDef RTC_DateStr;
RTC_AlarmTypeDef RTC_AlarmStr;

__IO bool AlarmOccurred = FALSE;
_eland_date_time_t ElandCurrentTime = {2017, RTC_Month_November, 15, 13, 20, 00, RTC_Weekday_Wednesday};
/* Private function prototypes -----------------------------------------------*/
void Calendar_Init(void);
/* Private functions ---------------------------------------------------------*/
/**
 ****************************************************************************
 * @Function : void RTC_Init(void)
 * @File     : rtc.c
 * @Program  : none
 * @Created  : 2017/10/23 by seblee
 * @Brief    : init rtc
 * @Version  : V1.0
**/
void ELAND_RTC_Init(void)
{
    /* Select LSE (32.768 KHz) as RTC clock source */
    CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1);

    CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);

    /* Calendar Configuration */
    Calendar_Init();
}

/**
  * @brief  Calendar Configuration.
  * @param  None
  * @retval None
  */
void Calendar_Init(void)
{
    RTC_InitStr.RTC_HourFormat = RTC_HourFormat_24;
    RTC_InitStr.RTC_AsynchPrediv = 0x7f;
    RTC_InitStr.RTC_SynchPrediv = 0x00ff;
    RTC_Init(&RTC_InitStr);

    RTC_DateStructInit(&RTC_DateStr);
    RTC_DateStr.RTC_WeekDay = ElandCurrentTime.week;
    RTC_DateStr.RTC_Date = ElandCurrentTime.day;
    RTC_DateStr.RTC_Month = ElandCurrentTime.month;
    RTC_DateStr.RTC_Year = ElandCurrentTime.year % 100;
    RTC_SetDate(RTC_Format_BIN, &RTC_DateStr);

    RTC_TimeStructInit(&RTC_TimeStr);
    RTC_TimeStr.RTC_Hours = ElandCurrentTime.hour;
    RTC_TimeStr.RTC_Minutes = ElandCurrentTime.minute;
    RTC_TimeStr.RTC_Seconds = ElandCurrentTime.second;
    RTC_SetTime(RTC_Format_BIN, &RTC_TimeStr);

    RTC_AlarmStructInit(&RTC_AlarmStr);
    RTC_AlarmStr.RTC_AlarmTime.RTC_Hours = 01;
    RTC_AlarmStr.RTC_AlarmTime.RTC_Minutes = 01;
    RTC_AlarmStr.RTC_AlarmTime.RTC_Seconds = 00;
    RTC_AlarmStr.RTC_AlarmMask = RTC_AlarmMask_All; //屏蔽了所有时间,导致闹钟1秒中执行一次,进入一次闹钟中断
    RTC_SetAlarm(RTC_Format_BIN, &RTC_AlarmStr);

    RTC_ITConfig(RTC_IT_ALRA, ENABLE);
    RTC_AlarmCmd(ENABLE);
}
/**
 ****************************************************************************
 * @Function : void RTC_Time_Set(_eland_date_time_t time)
 * @File     : rtc.c
 * @Program  : time to set
 * @Created  : 2017/10/23 by seblee
 * @Brief    : set time
 * @Version  : V1.0
**/
void RTC_Time_Set(_eland_date_time_t time)
{
    RTC_TimeTypeDef TimeStr;
    RTC_DateTypeDef DateStr;

    RTC_DateStructInit(&DateStr);
    DateStr.RTC_WeekDay = time.week;
    DateStr.RTC_Date = time.day;
    DateStr.RTC_Month = time.month;
    DateStr.RTC_Year = time.year;
    RTC_SetDate(RTC_Format_BIN, &DateStr);

    RTC_TimeStructInit(&TimeStr);
    TimeStr.RTC_Hours = time.hour;
    TimeStr.RTC_Minutes = time.minute;
    TimeStr.RTC_Seconds = time.second;
    RTC_SetTime(RTC_Format_BIN, &TimeStr);
}
/**
 ****************************************************************************
 * @Function : void ELAND_RTC_Read(_eland_date_time_t *time)
 * @File     : rtc.c
 * @Program  : time register
 * @Created  : 2017/10/23 by seblee
 * @Brief    : read rtc time
 * @Version  : V1.0
**/
void ELAND_RTC_Read(_eland_date_time_t *time)
{
    RTC_TimeTypeDef TimeStr;
    RTC_DateTypeDef DateStr;
    /* Wait until the calendar is synchronized */
    // while (RTC_WaitForSynchro() != SUCCESS) ;//低功耗時候使用
    /* Get the current Time*/
    RTC_GetTime(RTC_Format_BIN, &TimeStr);
    /* Get the current Date */
    RTC_GetDate(RTC_Format_BIN, &DateStr);

    time->year = DateStr.RTC_Year;
    time->month = DateStr.RTC_Month;
    time->day = DateStr.RTC_Date;
    time->week = DateStr.RTC_WeekDay;
    time->hour = TimeStr.RTC_Hours;
    time->minute = TimeStr.RTC_Minutes;
    time->second = TimeStr.RTC_Seconds;
}
/**
 ****************************************************************************
 * @Function : void ELAND_RTC_ALARM_ISR(void)
 * @File     : rtc.c
 * @Program  : none
 * @Created  : 2017/10/23 by seblee
 * @Brief    : RTC ALARM Interupt Service
 * @Version  : V1.0
**/
void ELAND_RTC_ALARM_ISR(void)
{
    ELAND_RTC_Read(&ElandCurrentTime);
    RTC_ClearITPendingBit(RTC_IT_ALRA);
    AlarmOccurred = TRUE;
}
/**
 ****************************************************************************
 * @Function : _eland_date_time_t ELAND_Time_Convert(mico_rtc_time_t SCR_time)
 * @File     : rtc.c
 * @Program  : SCR_time
 * @Created  : 2017/11/13 by seblee
 * @Brief    : time formate converrt
 * @Version  : V1.0
**/
void ELAND_Time_Convert(mico_rtc_time_t *mico_time, _eland_date_time_t *mcu_time, __mico2mcu_t mico2mcu)
{
    if (mico2mcu == MICO_2_MCU)
    {
        mcu_time->year = mico_time->year;

        if (mico_time->month <= RTC_Month_September)
            mcu_time->month = (RTC_Month_TypeDef)mico_time->month;
        else if (mico_time->month == 10)
            mcu_time->month = RTC_Month_October;
        else if (mico_time->month == 11)
            mcu_time->month = RTC_Month_November;
        else if (mico_time->month == 12)
            mcu_time->month = RTC_Month_December;

        mcu_time->day = mico_time->date;
        mcu_time->hour = mico_time->hr;
        mcu_time->minute = mico_time->min;
        mcu_time->second = mico_time->sec;
        if (mico_time->weekday == 1)
            mcu_time->week = RTC_Weekday_Sunday;
        else
            mcu_time->week = (RTC_Weekday_TypeDef)(mico_time->weekday - 1);
    }
    else if (mico2mcu == MCU_2_MICO)
    {
        mico_time->year = mcu_time->year;
        mico_time->month = (uint8_t)mcu_time->month;

        mico_time->date = mcu_time->day;
        mico_time->hr = mcu_time->hour;
        mico_time->min = mcu_time->minute;
        mico_time->sec = mcu_time->second;
        if (mcu_time->week == RTC_Weekday_Sunday)
            mico_time->weekday = 1;
        else
            mico_time->weekday = (uint8_t)mcu_time->week + 1;
    }
}
