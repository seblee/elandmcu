/**
 ****************************************************************************
 * @Warning :Without permission from the author,Not for commercial use
 * @File    :key.h
 * @Author  :seblee
 * @date    :2017/10/25
 * @version :V 1.0.0
 *************************************************
 * @brief   :
 ****************************************************************************
**/
#ifndef __KEY_H_
#define __KEY_H_
/* Private include -----------------------------------------------------------*/
#include "stm8l15x.h"
#include "stm8l15x_gpio.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum {
    KEY_Set = (uint16_t)0x0001,       /*!< 時刻設置 */
    KEY_Reset = (uint16_t)0x0002,     /*!< 軟件復位 */
    KEY_Add = (uint16_t)0x0004,       /*!< 時間＋   */
    KEY_Minus = (uint16_t)0x0008,     /*!< 時間－   */
    KEY_MON = (uint16_t)0x0010,       /*!< mon時間  */
    KEY_AlarmMode = (uint16_t)0x0020, /*!< 鬧鐘模式 */
    KEY_Wifi = (uint16_t)0x0040,      /*!< wifi模式 */
    KEY_Snooze = (uint16_t)0x0080,    /*!< 貪睡     */
    KEY_Alarm = (uint16_t)0x0100,     /*!< 鬧鐘     */
} KEY_State_TypeDef;
/* Private define ------------------------------------------------------------*/
//時刻設置
#define ELAND_KEY_SET_PORT GPIOI
#define ELAND_KEY_SET_PIN GPIO_Pin_1
//軟件復位
#define ELAND_KEY_SOFT_RESET_PORT GPIOI
#define ELAND_KEY_SOFT_RESET_PIN GPIO_Pin_2
//時間＋
#define ELAND_KEY_ADD_PORT GPIOH
#define ELAND_KEY_ADD_PIN GPIO_Pin_4
//時間－
#define ELAND_KEY_MINUS_PORT GPIOH
#define ELAND_KEY_MINUS_PIN GPIO_Pin_5
//mon時間
#define ELAND_KEY_MON_PORT GPIOF
#define ELAND_KEY_MON_PIN GPIO_Pin_1
//鬧鐘
#define ELAND_KEY_ALARM_MODE_PORT GPIOF
#define ELAND_KEY_ALARM_MODE_PIN GPIO_Pin_2
//wifi模式
#define ELAND_KEY_WIFI_PORT GPIOF
#define ELAND_KEY_WIFI_PIN GPIO_Pin_3

//貪睡
#define ELAND_KEY_SNOOZE_PORT GPIOC
#define ELAND_KEY_SNOOZE_PIN GPIO_Pin_0

//Alarm
#define ELAND_KEY_ALARM_PORT GPIOC
#define ELAND_KEY_ALARM_PIN GPIO_Pin_1

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern uint16_t Key_Trg;     //按鍵單次狀態
extern uint16_t Key_Count;   //按鍵長按狀態
extern uint16_t Key_Restain; //按鍵按捺狀態
/* Private function prototypes -----------------------------------------------*/
void ElandKeyInit(void); //按键初始化
void Eland_KeyState_Read(void);

/* Private functions ---------------------------------------------------------*/

#endif /*__KEY_H_*/
