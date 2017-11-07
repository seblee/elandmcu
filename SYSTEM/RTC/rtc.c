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
#include "lcd_eland.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
RTC_InitTypeDef RTC_InitStr;
RTC_TimeTypeDef RTC_TimeStr;
RTC_DateTypeDef RTC_DateStr;
RTC_AlarmTypeDef RTC_AlarmStr;

__IO bool AlarmOccurred = FALSE;
_eland_date_time ElandCurrentTime = {2017, RTC_Month_November, 7, 16, 00, 00, RTC_Weekday_Saturday};
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
 * @Function : void RTC_Time_Set(_eland_date_time time)
 * @File     : rtc.c
 * @Program  : time to set
 * @Created  : 2017/10/23 by seblee
 * @Brief    : set time
 * @Version  : V1.0
**/
void RTC_Time_Set(_eland_date_time time)
{
    RTC_DateStructInit(&RTC_DateStr);
    RTC_DateStr.RTC_WeekDay = time.week;
    RTC_DateStr.RTC_Date = time.day;
    RTC_DateStr.RTC_Month = time.month;
    RTC_DateStr.RTC_Year = time.year;
    RTC_SetDate(RTC_Format_BIN, &RTC_DateStr);

    RTC_TimeStructInit(&RTC_TimeStr);
    RTC_TimeStr.RTC_Hours = time.hour;
    RTC_TimeStr.RTC_Minutes = time.minute;
    RTC_TimeStr.RTC_Seconds = time.second;
    RTC_SetTime(RTC_Format_BIN, &RTC_TimeStr);
}
/**
 ****************************************************************************
 * @Function : void ELAND_RTC_Read(_eland_date_time *time)
 * @File     : rtc.c
 * @Program  : time register
 * @Created  : 2017/10/23 by seblee
 * @Brief    : read rtc time
 * @Version  : V1.0
**/
void ELAND_RTC_Read(_eland_date_time *time)
{
    /* Wait until the calendar is synchronized */
    // while (RTC_WaitForSynchro() != SUCCESS) ;//低功耗時候使用
    /* Get the current Time*/
    RTC_GetTime(RTC_Format_BIN, &RTC_TimeStr);
    /* Get the current Date */
    RTC_GetDate(RTC_Format_BIN, &RTC_DateStr);

    time->year = RTC_DateStr.RTC_Year;
    time->month = RTC_DateStr.RTC_Month;
    time->day = RTC_DateStr.RTC_Date;
    time->week = RTC_DateStr.RTC_WeekDay;
    time->hour = RTC_TimeStr.RTC_Hours;
    time->minute = RTC_TimeStr.RTC_Minutes;
    time->second = RTC_TimeStr.RTC_Seconds;
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
