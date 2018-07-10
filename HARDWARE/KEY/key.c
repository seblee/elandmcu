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
#include "lcd_display.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint16_t Key_Down_Trg = 0;          //按鍵單次狀態
uint16_t Key_Up_Trg = 0;            //按鍵短按 放開
uint16_t Key_Count = 0;             //按鍵長按狀態 按下/放開狀態
uint16_t Key_Restain = 0;           //按鍵按捺狀態
uint16_t Key_Restain_Trg = 0;       //按鍵按捺狀態
uint16_t Key_Short_Restain = 0;     //按鍵按捺狀態
uint16_t Key_Short_Restain_Trg = 0; //按鍵按捺狀態
__no_init uint16_t Key_Light_counter;
/* Private function prototypes -----------------------------------------------*/
static void Eland_Key_Long_Press_State(void);
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
    GPIO_Init(ELAND_KEY_TEST_PORT, ELAND_KEY_TEST_PIN, GPIO_Mode_In_PU_No_IT);             //TEST

    Eland_KeyState_Read();
    Eland_KeyState_Read();
    Eland_KeyState_Read();
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
    uint16_t Cache = 0;
    if (GPIO_ReadInputDataBit(ELAND_KEY_SET_PORT, ELAND_KEY_SET_PIN))
        Cache |= KEY_Set; /*!< SET */
    if (GPIO_ReadInputDataBit(ELAND_KEY_SOFT_RESET_PORT, ELAND_KEY_SOFT_RESET_PIN))
        Cache |= KEY_Reset; /*!< RESET */
    if (GPIO_ReadInputDataBit(ELAND_KEY_ADD_PORT, ELAND_KEY_ADD_PIN))
        Cache |= KEY_Add; /*!< UP   */
    if (GPIO_ReadInputDataBit(ELAND_KEY_MINUS_PORT, ELAND_KEY_MINUS_PIN))
        Cache |= KEY_Minus; /*!< DOWN  */
    if (GPIO_ReadInputDataBit(ELAND_KEY_MON_PORT, ELAND_KEY_MON_PIN))
        Cache |= KEY_MON; /*!< offline clock alarm off*/
    if (GPIO_ReadInputDataBit(ELAND_KEY_ALARM_MODE_PORT, ELAND_KEY_ALARM_MODE_PIN))
        Cache |= KEY_AlarmMode; /*!< offline alarm on*/
    if (GPIO_ReadInputDataBit(ELAND_KEY_WIFI_PORT, ELAND_KEY_WIFI_PIN))
        Cache |= KEY_Wifi; /*!< ON line*/
    if (GPIO_ReadInputDataBit(ELAND_KEY_SNOOZE_PORT, ELAND_KEY_SNOOZE_PIN))
        Cache |= KEY_Snooze; /*!< SNOOZE     */
    if (GPIO_ReadInputDataBit(ELAND_KEY_ALARM_PORT, ELAND_KEY_ALARM_PIN))
        Cache |= KEY_Alarm; /*!< ALARM_OFF   */
    if (GPIO_ReadInputDataBit(ELAND_KEY_TEST_PORT, ELAND_KEY_TEST_PIN))
        Cache |= KEY_TEST; //TEST

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
        ReadData = KeyValue_present ^ KEY_CHECK_BASE;
        Key_Down_Trg = ReadData & (ReadData ^ Key_Count);
        Key_Count = ReadData;
    }
    if ((Key_Down_Trg & KEY_Snooze) ||
        (Key_Down_Trg & KEY_Alarm) ||
        (Key_Down_Trg & KEY_Set) ||
        (Key_Down_Trg & KEY_Add) ||
        (Key_Down_Trg & KEY_Minus))
    {
        Key_Light_counter = 0;
        ELAND_DATA_Refreshed = TRUE;
    }
    else if (Key_Light_counter < 0xffff)
    {
        if (Key_Light_counter++ == SW_LIGHT_TIMES)
            ELAND_DATA_Refreshed = TRUE;
    }
    KeyValue_last = KeyValue_present;
    Eland_Key_Long_Press_State();
}
/**
 ****************************************************************************
 * @Function : void Eland_Key_Long_Press_State(void)
 * @File     : key.c
 * @Program  : none
 * @Created  : 2017/11/2 by seblee
 * @Brief    : check long press state 20ms 訪問一次
 * @Version  : V1.0
**/
static void Eland_Key_Long_Press_State(void)
{
    static uint8_t KEY_Timer[9];
    uint8_t i;
    static uint16_t Key_Restain_Count = 0;
    static uint16_t Key_Short_Restain_Count = 0;
    uint8_t long_press_time_tem;
    for (i = 0; i < KEY_NUM; i++)
    {
        if (i == 8)
            long_press_time_tem = ALARM_OFF_LONG_PRESS_TIMES;
        else
            long_press_time_tem = LONG_PRESS_TIMES;
        if (Key_Count & (1 << i))
        {
            if (KEY_Timer[i] < long_press_time_tem)
                KEY_Timer[i]++;
            else
                Key_Restain |= (1 << i);

            if (KEY_Timer[i] > SHSORT_RESTAIN_TIMES)
                Key_Short_Restain |= (1 << i);
        }
        else
        {
            if ((KEY_Timer[i] > 0) && (KEY_Timer[i] < long_press_time_tem)) // short press
                Key_Up_Trg |= (1 << i);
            else
                Key_Up_Trg &= (~(1 << i));
            KEY_Timer[i] = 0;
            Key_Restain &= (~(1 << i));
            Key_Short_Restain &= (~(1 << i));
        }
    }
    Key_Restain_Trg = Key_Restain & (Key_Restain ^ Key_Restain_Count);
    Key_Restain_Count = Key_Restain;
    Key_Short_Restain_Trg = Key_Short_Restain & (Key_Short_Restain ^ Key_Short_Restain_Count);
    Key_Short_Restain_Count = Key_Short_Restain;
}
