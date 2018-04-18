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
typedef struct eland_data_2_mcu
{
    int8_t time_display_format;
    int8_t night_mode_enabled;
    int8_t brightness_normal;
    int8_t brightness_night;
    int8_t led_normal;
    int8_t led_night;
    uint32_t night_mode_begin_time;
    uint32_t night_mode_end_time;
} __ELAND_DATA_2_MCU_t;

typedef enum _eland_mode {
    ELAND_MODE_NONE = (uint8_t)0x00,
    ELAND_CLOCK_MON,
    ELAND_CLOCK_ALARM,
    ELAND_NC,
    ELAND_NA,
    ELAND_AP,
    ELAND_OTA,
    ELAND_MODE_MAX,
} _ELAND_MODE_t;

typedef enum {
    ALARM_IDEL,
    ALARM_ADD,
    ALARM_MINUS,
    ALARM_SORT,
    ALARM_ING,
    ALARM_SNOOZ_STOP,
    ALARM_STOP,
    ALARM_SKIP,
} _alarm_list_state_t;
typedef struct
{
    mico_rtc_time_t moment_time;
    int8_t color;
    int8_t snooze_enabled;
    int8_t next_alarm;
    int8_t alarm_skip;
    _ELAND_MODE_t mode;
    _alarm_list_state_t alarm_state;
} _alarm_mcu_data_t;

/* Private define ------------------------------------------------------------*/
#define ChangeSpeed 5 //speed of continued +/- =
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern bool Alarm_need_Refresh;
extern bool ELAND_DATA_Refreshed;
extern bool Alarm_is_empty;
extern __ELAND_DATA_2_MCU_t eland_data;
extern _alarm_mcu_data_t alarm_data_simple;
extern _alarm_mcu_data_t alarm_data_display;
extern _ELAND_MODE_t Eland_mode;
extern uint8_t alarm_skip_flag;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void LCD_Display_Rssi_State(Eland_Status_type_t state);
void LCD_NetMode(void);
void LCD_Clock_MON(void);
void LCD_data_init(void);
void ALARM_Alarm_Refresh(void);
void Eland_data_Refresh(void);
void LCD_OtherMode(void);
void Brightness_refresh(void);
void Eland_alarm_display(FlagStatus status);
void ota_display(void);
#endif /*__LCD_DISPLAY_H_*/
