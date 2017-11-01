/**
 ****************************************************************************
 * @Warning :Without permission from the author,Not for commercial use
 * @File    :key.c
 * @Author  :seblee
 * @date    :2017/10/25
 * @version :V 1.0.0
 *************************************************
 * @brief   :
 ****************************************************************************
**/
/* Private include -----------------------------------------------------------*/
#include "key.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint16_t Key_Trg = 0;     //按鍵單次狀態
uint16_t Key_Count = 0;   //按鍵長按狀態
uint16_t Key_Restain = 0; //按鍵按捺狀態

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
 ****************************************************************************
 * @Function : void KeyInit(void)
 * @File     : key.c
 * @Program  : none
 * @Created  : 2017/10/25 by seblee
 * @Brief    : init key gpio
 * @Version  : V1.0
**/

void ElandKeyInit(void) //按键初始化
{
    GPIO_Init(ELAND_KEY_SET_PORT, ELAND_KEY_SET_PIN, GPIO_Mode_In_FL_No_IT);               //時刻設置
    GPIO_Init(ELAND_KEY_SOFT_RESET_PORT, ELAND_KEY_SOFT_RESET_PIN, GPIO_Mode_In_FL_No_IT); //軟件復位
    GPIO_Init(ELAND_KEY_ADD_PORT, ELAND_KEY_ADD_PIN, GPIO_Mode_In_FL_No_IT);               //時間＋
    GPIO_Init(ELAND_KEY_MINUS_PORT, ELAND_KEY_MINUS_PIN, GPIO_Mode_In_FL_No_IT);           //時間－
    GPIO_Init(ELAND_KEY_MON_PORT, ELAND_KEY_MON_PIN, GPIO_Mode_In_FL_No_IT);               //mon時間
    GPIO_Init(ELAND_KEY_ALARM_MODE_PORT, ELAND_KEY_ALARM_MODE_PIN, GPIO_Mode_In_FL_No_IT); //鬧鐘模式
    GPIO_Init(ELAND_KEY_WIFI_PORT, ELAND_KEY_WIFI_PIN, GPIO_Mode_In_FL_No_IT);             //wifi模式
    GPIO_Init(ELAND_KEY_SNOOZE_PORT, ELAND_KEY_SNOOZE_PIN, GPIO_Mode_In_FL_No_IT);         //貪睡
    GPIO_Init(ELAND_KEY_ALARM_PORT, ELAND_KEY_ALARM_PIN, GPIO_Mode_In_FL_No_IT);           //鬧鐘
}
/**
 ****************************************************************************
 * @Function : uint16_t Eland_PinState_Read(void)
 * @File     : key.c
 * @Program  : none
 * @Created  : 2017/10/25 by seblee
 * @Brief    : read pin state
 * @Version  : V1.0
**/
uint16_t Eland_PinState_Read(void)
{
    u8 Cache = 0;
    if (GPIO_ReadInputDataBit(ELAND_KEY_SET_PORT, ELAND_KEY_SET_PIN))
        Cache |= KEY_Set; //時刻設置
    if (GPIO_ReadInputDataBit(ELAND_KEY_SOFT_RESET_PORT, ELAND_KEY_SOFT_RESET_PIN))
        Cache |= KEY_Reset; //軟件復位
    if (GPIO_ReadInputDataBit(ELAND_KEY_ADD_PORT, ELAND_KEY_ADD_PIN))
        Cache |= KEY_Add; //時間＋
    if (GPIO_ReadInputDataBit(ELAND_KEY_MINUS_PORT, ELAND_KEY_MINUS_PIN))
        Cache |= KEY_Minus; //時間－
    if (GPIO_ReadInputDataBit(ELAND_KEY_MON_PORT, ELAND_KEY_MON_PIN))
        Cache |= KEY_MON; //mon時間
    if (GPIO_ReadInputDataBit(ELAND_KEY_ALARM_MODE_PORT, ELAND_KEY_ALARM_MODE_PIN))
        Cache |= KEY_AlarmMode; //鬧鐘模式
    if (GPIO_ReadInputDataBit(ELAND_KEY_WIFI_PORT, ELAND_KEY_WIFI_PIN))
        Cache |= KEY_Wifi; //wifi模式
    if (GPIO_ReadInputDataBit(ELAND_KEY_WIFI_PORT, ELAND_KEY_WIFI_PIN))
        Cache |= KEY_Snooze; //貪睡
    if (GPIO_ReadInputDataBit(ELAND_KEY_WIFI_PORT, ELAND_KEY_WIFI_PIN))
        Cache |= KEY_Alarm; //鬧鐘
    return Cache;
}
/**
 ****************************************************************************
 * @Function : void Eland_KeyState_Read(void)
 * @File     : key.c
 * @Program  : none
 * @Created  : 2017/10/25 by seblee
 * @Brief    : key state read
 * @Version  : V1.0
**/
void Eland_KeyState_Read(void)
{
    static uint16_t KeyValue_last = 0;
    uint16_t KeyValue_present, ReadData;
    KeyValue_present = Eland_PinState_Read(); //key当前值
    if (KeyValue_present == KeyValue_last)    //去抖20ms
    {
        ReadData = KeyValue_present ^ 0x7f;
        Key_Trg = ReadData & (ReadData ^ Key_Count);
        Key_Count = ReadData;
    }
    KeyValue_last = KeyValue_present;
}
