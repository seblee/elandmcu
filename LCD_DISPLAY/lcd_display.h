/**
 ****************************************************************************
 * @Warning :Without permission from the author,Not for commercial use
 * @File    :lcd_display.h
 * @Author  :seblee
 * @date    :2017/12/12
 * @version :V 1.0.0
 *************************************************
 * @brief   :
 ****************************************************************************
**/
#ifndef __LCD_DISPLAY_H_
#define __LCD_DISPLAY_H_
/* Private include -----------------------------------------------------------*/
#include "syscfg.h"
#include "eland_usart.h"
#include "ht162x.h"
/* Private typedef -----------------------------------------------------------*/
typedef struct
{
    int8_t time_display_format;
    int8_t brightness_normal;
    int8_t brightness_night;
    int8_t night_mode_enable;
    uint32_t night_mode_begin_time;
    uint32_t night_mode_end_time;
} _eland_data_t;

typedef struct
{
    mico_rtc_time_t alarm_moment; //
    int8_t alarm_color;
    int8_t snooze_count;
    uint8_t alarm_on_days_of_week;
} _alarm_MCU_data_t;

typedef enum _eland_mode {
    ELAND_MODE_NONE,
    ELAND_CLOCK_MON,
    ELAND_CLOCK_ALARM,
    ELAND_NC,
    ELAND_NA,
    ELAND_MODE_MAX,
} _ELAND_MODE_t;

/* Private define ------------------------------------------------------------*/
#define ChangeSpeed 5 //speed of continued +/- =
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern _eland_data_t eland_data;
extern _alarm_MCU_data_t alarm_data;
extern _ELAND_MODE_t Eland_mode;
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void LCD_Display_Rssi_State(Eland_Status_type_t state);
void LCD_NetMode(void);
void LCD_Clock_MON(void);
void LCD_data_init(void);

#endif /*__LCD_DISPLAY_H_*/
