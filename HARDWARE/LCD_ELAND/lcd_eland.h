/**
 ****************************************************************************
 * @Warning :Without permission from the author,Not for commercial use
 * @File    :lcd_eland.h
 * @Author  :seblee
 * @date    :2017/10/23
 * @version :V 1.0.0
 *************************************************
 * @brief   :
 ****************************************************************************
**/
#ifndef __LCD_ELAND_H_
#define __LCD_ELAND_H_
/* Private include -----------------------------------------------------------*/
#include "syscfg.h"
#include "rtc.h"
/* Private typedef -----------------------------------------------------------*/
typedef enum {
    COM_0 = (uint8_t)0x00, /*!< COM 0  */
    COM_1 = (uint8_t)0x01, /*!< COM 1  */
    COM_2 = (uint8_t)0x02, /*!< COM 2  */
    COM_3 = (uint8_t)0x03, /*!< COM 3  */
    COM_4 = (uint8_t)0x04, /*!< COM 4  */
    COM_5 = (uint8_t)0x05, /*!< COM 5  */
    COM_6 = (uint8_t)0x06, /*!< COM 6  */
    COM_7 = (uint8_t)0x07, /*!< COM 7  */
} LCD_COMx_TypeDef;

typedef enum {
    Serial_01,
    Serial_02,
    Serial_03,
    Serial_04,
    Serial_05,
    Serial_06,
    Serial_07,
    Serial_08,
    Serial_09,
    Serial_10,
    Serial_11,
    Serial_12,
    Serial_13,
    Serial_14,
    Serial_15,
    Serial_16,
    Serial_17,
    Serial_18,
    Serial_19,
    Serial_20,
} LCD_Digital_Serial_t;

typedef union {
    uint16_t WORD;
    uint8_t BYTE[2];
    struct
    {
        unsigned char Digital_FGHI : 4; //word 8-11   Data[0] 0-3
        unsigned char Digital_B : 4;    //word 12-15  Data[0] 4-7
        unsigned char Digital_CALK : 4; //word 0-3    Data[1] 0-3
        unsigned char Digital_EDMJ : 4; //word 4-7    Data[1] 4-7
    };
} __Digital_Coding_t;

typedef enum {
    POSITIVE,
    NEGATIVE,
} LCD_Coding_Dirtction_t;

typedef enum {
    TIME_PART,
    ALARM_PART,
} LCD_Time_Type_t;

typedef enum {
    SUNDAY,
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
} LCD_Week_Day_t;

typedef enum {
    LEVEL0,
    LEVEL1,
    LEVEL2,
    LEVEL3,
    LEVEL4,
} LCD_Wifi_Rssi_t;

/* Private define ------------------------------------------------------------*/
#define testlcdIO
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void LCD_ELAND_Init(void);
void LCD_ELAND_Write_All(void);
void LCD_ELAND_Clear_All(void);
void LCD_Eland_Pixel_Write(LCD_COMx_TypeDef COMx, u8 SEGn);
void LCD_Eland_Pixel_Clear(LCD_COMx_TypeDef COMx, u8 SEGn);
void LCD_Eland_COMx_Write(LCD_COMx_TypeDef COMx);
void LCD_Eland_COMx_Clear(LCD_COMx_TypeDef COMx);
void LCD_Eland_Num_Set(LCD_Digital_Serial_t Serial, u8 data);
void LCD_Eland_Week_Set(LCD_Time_Type_t type, LCD_Week_Day_t day);
void LCD_Eland_Wifi_RSSI_Set(LCD_Wifi_Rssi_t level);
void LCD_Eland_Time_Display(_eland_date_time_t time);
void LCD_Eland_Time_Upgrade(_eland_date_time_t time);
void LCD_Eland_COM_SCAN(void);
/* Private functions ---------------------------------------------------------*/

#endif /*__LCD_ELAND_H_*/
