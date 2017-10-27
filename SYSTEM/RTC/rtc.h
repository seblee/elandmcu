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
    u16 year;
    RTC_Month_TypeDef month;
    u8 day;
    u8 hour;
    u8 minute;
    u8 second;
    RTC_Weekday_TypeDef week;
} _eland_date_time;

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void ELAND_RTC_Init(void); 
void ELAND_RTC_ALARM_ISR(void);
/* Private functions ---------------------------------------------------------*/

#endif /*__RTC_H_*/
