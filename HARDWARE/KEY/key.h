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
    KEY_Set = (uint8_t)0x01,   /*!< 時刻設置 */
    KEY_Reset = (uint8_t)0x02, /*!< 軟件復位 */
    KEY_Add = (uint8_t)0x04,   /*!< 時間＋   */
    KEY_Minus = (uint8_t)0x08, /*!< 時間－   */
    KEY_MON = (uint8_t)0x10,   /*!< mon時間  */
    KEY_Alarm = (uint8_t)0x20, /*!< 鬧鐘     */
    KEY_Wifi = (uint8_t)0x40,  /*!< wifi模式 */
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
#define ELAND_KEY_ALARM_PORT GPIOF
#define ELAND_KEY_ALARM_PIN GPIO_Pin_2
//wifi模式
#define ELAND_KEY_WIFI_PORT GPIOF
#define ELAND_KEY_WIFI_PIN GPIO_Pin_3

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern u8 Trg;   //按鍵單次狀態
extern u8 Count; //按鍵長按狀態
/* Private function prototypes -----------------------------------------------*/
void ElandKeyInit(void); //按键初始化
void Eland_KeyState_Read(void);

/* Private functions ---------------------------------------------------------*/

#endif /*__KEY_H_*/
