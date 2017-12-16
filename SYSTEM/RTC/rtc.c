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
__IO bool WakeupOccurred = FALSE;
_eland_date_time_t ElandCurrentTime = {2017, RTC_Month_December, 5, 16, 20, 00, RTC_Weekday_Tuesday};
const char MonthStr[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
RTC_Month_TypeDef MonthValue[12] = {RTC_Month_January, RTC_Month_February, RTC_Month_March, RTC_Month_April, RTC_Month_May, RTC_Month_June, RTC_Month_July, RTC_Month_August, RTC_Month_September, RTC_Month_October, RTC_Month_November, RTC_Month_December};
/* Private function prototypes -----------------------------------------------*/
void Calendar_Init(void);
static void Get_built_DateTime(_eland_date_time_t *time);
static RTC_Weekday_TypeDef CaculateWeekDay(int y, int m, int d);
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
    /* Configures the RTC wakeup timer_step = RTCCLK/16 = LSE/16 = 488.28125 us */
    RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);

    /* Enable wake up unit Interrupt */
    RTC_ITConfig(RTC_IT_WUT, ENABLE);

    RTC_SetWakeUpCounter(1023);
    RTC_WakeUpCmd(ENABLE);
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
    Get_built_DateTime(&ElandCurrentTime);

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
 * @Brief    : RTC ALARM Interrupt Service
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
 * @Function : void ELAND_RTC_WAKEUP_ISR(void)
 * @File     : rtc.c
 * @Program  : none
 * @Created  : 2017/12/5 by seblee
 * @Brief    : RTC WAKEUP Interrupt
 * @Version  : V1.0
**/
void ELAND_RTC_WAKEUP_ISR(void)
{
    RTC_ClearITPendingBit(RTC_IT_WUT);
    WakeupOccurred = TRUE;
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

        mcu_time->month = MonthValue[mico_time->month - 1];

        // if (mico_time->month <= RTC_Month_September)
        //     mcu_time->month = (RTC_Month_TypeDef)mico_time->month;
        // else if (mico_time->month == 10)
        //     mcu_time->month = RTC_Month_October;
        // else if (mico_time->month == 11)
        //     mcu_time->month = RTC_Month_November;
        // else if (mico_time->month == 12)
        //     mcu_time->month = RTC_Month_December;

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
/**
 ****************************************************************************
 * @Function : static void Get_built_DateTime(_eland_date_time_t *time)
 * @File     : rtc.c
 * @Program  : time:back current time
 * @Created  : 2017/12/6 by seblee
 * @Brief    : set current time
 * @Version  : V1.0
**/
static void Get_built_DateTime(_eland_date_time_t *time)
{
    uint16_t da, ho, mi, se;
    char temp_str[4] = {0, 0, 0, 0}, i;
    sscanf(__DATE__, "%s %2d %4d", temp_str, &da, &(time->year));
    sscanf(__TIME__, "%2d:%2d:%2d", &ho, &mi, &se);

    time->day = (uint8_t)da;
    time->hour = (uint8_t)ho;
    time->minute = (uint8_t)mi;
    time->second = (uint8_t)se;
    for (i = 0; i < 12; i++)
    {
        if (strncmp(temp_str, MonthStr[i], 3) == 0)
        {
            time->month = MonthValue[i];
            break;
        }
    }
    time->week = CaculateWeekDay(time->year, i + 1, time->day);
}
/**
 ****************************************************************************
 * @Function : RTC_Weekday_TypeDef CaculateWeekDay(int y, int m, int d)
 * @File     : rtc.c
 * @Program  : y:year m:month d:day
 * @Created  : 2017/12/16 by seblee
 * @Brief    : Caculat get week
 * @Version  : V1.0
**/
RTC_Weekday_TypeDef CaculateWeekDay(int y, int m, int d)
{
    int iWeek;
    if (m == 1 || m == 2)
    {
        m += 12;
        y--;
    }
    iWeek = (d + 2 * m + 3 * (m + 1) / 5 + y + y / 4 - y / 100 + y / 400) % 7;
    return (RTC_Weekday_TypeDef)(iWeek + 1);
}
