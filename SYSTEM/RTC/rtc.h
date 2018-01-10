/**
 ****************************************************************************
 * @Warning :Without permission from the author,Not for commercial use
 * @File    :rtc.h
 * @Author  :seblee
 * @date    :2017/10/23
 * @version :V 1.0.0
 *************************************************
 * @brief   :
 ****************************************************************************
**/
#ifndef __RTC_H_
#define __RTC_H_
/* Private include -----------------------------------------------------------*/
#include "syscfg.h"
/* Private typedef -----------------------------------------------------------*/
typedef struct
{
    char year[4];       /**< Year         */
    char dash1;         /**< Dash1        */
    char month[2];      /**< Month        */
    char dash2;         /**< Dash2        */
    char day[2];        /**< Day          */
    char T;             /**< T            */
    char hour[2];       /**< Hour         */
    char colon1;        /**< Colon1       */
    char minute[2];     /**< Minute       */
    char colon2;        /**< Colon2       */
    char second[2];     /**< Second       */
    char decimal;       /**< Decimal      */
    char sub_second[6]; /**< Sub-second   */
    char Z;             /**< UTC timezone */
} iso8601_time_t;

typedef struct
{
    u16 yea;
    RTC_Month_TypeDef month;
    u8 day;
    u8 hour;
    u8 minute;
    u8 second;
    RTC_Weekday_TypeDef week;
} _eland_date_time_t;

/**
 * RTC time
 */
typedef struct
{
    uint8_t sec;
    uint8_t min;
    uint8_t hr;
    uint8_t weekday; /* 1-sunday... 7-saturday */
    uint8_t date;
    uint8_t month;
    uint8_t year;
} mico_rtc_time_t;

typedef enum {
    MICO_2_MCU,
    MCU_2_MICO,
} __mico2mcu_t;
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern _eland_date_time_t CurrentMCUTime;
extern mico_rtc_time_t CurrentMicoTime;
extern __IO bool AlarmOccurred;
extern __IO bool WakeupOccurred;
extern uint8_t DayOfMon[12][2];
/* Private function prototypes -----------------------------------------------*/
void ELAND_RTC_Init(void);
void ELAND_RTC_ALARM_ISR(void);
void ELAND_RTC_WAKEUP_ISR(void);
void RTC_Time_Set(_eland_date_time_t time);

void ELAND_RTC_Read(mico_rtc_time_t *time);
void ELAND_Time_Convert(mico_rtc_time_t *mico_time, _eland_date_time_t *mcu_time, __mico2mcu_t mico2mcu);
RTC_Weekday_TypeDef CaculateWeekDay(int y, int m, int d);
/* Private functions ---------------------------------------------------------*/

#endif /*__RTC_H_*/
