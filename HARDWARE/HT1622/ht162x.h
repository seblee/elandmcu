/**
 ****************************************************************************
 * @Warning :Without permission from the author,Not for commercial use
 * @File    :HT162x.h
 * @Author  :seblee
 * @date    :2017/12/1
 * @version :V 1.0.0
 *************************************************
 * @brief   :
 ****************************************************************************
**/
#ifndef __HT162x_H_
#define __HT162x_H_
/* Private include -----------------------------------------------------------*/
#include "syscfg.h"
#include "rtc.h"
/* Private typedef -----------------------------------------------------------*/
typedef enum _HT162x_CMD {
    SYS_DIS = 0X00,   //Turn off both system oscillator and LCD bias generator ------------ Def.
    SYS_EN = 0X01,    //Turn on system oscillator
    LCD_OFF = 0X02,   //Turn off LCD display ------------ Def.
    LCD_ON = 0X03,    //Turn on LCD display
    TIMER_DIS = 0X04, //Disable time base output ------------ Def.
    WDT_DIS = 0X05,   //Disable WDT time-out flag output ------------ Def.
    TIMER_EN = 0X06,  //Enable time base output
    WDT_EN = 0X07,    //Enable WDT time - out flag output
    TONE_OFF = 0X08,  //Turn off tone outputs ------------ Def.
    CLR_TIMER = 0X0D, //Clear the contents of the time base generator
    CLR_WDT = 0X0F,   //Clear the contents of the WDT stage
    RC_32K = 0x18,    //System clock source,on - chip RC oscillator ------------ Def.
    EXT_32K = 0X1C,   //System clock source, external 32kHz clock source or crystal oscillator 32.768kHz
    TONE_4K = 0X40,   //Tone frequency output : 4kHz
    TONE_2K = 0X60,   //Tone frequency output : 2kHz
    IRQ_DIS = 0X80,   //Disable IRQ output ------------ Def.
    IRQ_EN = 0X88,    //Enable IRQ output
    F1 = 0XA0,        //Time base clock output : 1Hz The WDT time - out flag after : 4s
    F2 = 0XA1,        //Time base clock output : 2Hz The WDT time - out flag after : 2s
    F4 = 0XA2,        //Time base clock output : 4Hz The WDT time - out flag after : 1s
    F8 = 0XA3,        //Time base clock output : 8Hz The WDT time - out flag after : 1 / 2s
    F16 = 0X4A,       //Time base clock output : 16Hz The WDT time - out flag after : 1 / 4s
    F32 = 0XA5,       //Time base clock output : 32Hz The WDT time - out flag after : 1 / 8s
    F64 = 0XA6,       //Time base clock output : 64Hz The WDT time - out flag after : 1 / 16s
    F128 = 0XA7,      //Time base clock output : 128Hz The WDT time - out flag after : 1 / 32s ------------ Def.
    TEST = 0XE0,      //Test mode,user don t use.
    NORMAL = 0XE3,    //Normal mod ------------ Def.
} _HT162x_CMD_t;
typedef enum _SEQUENT_ {
    MSB = 0X00,
    LSB
} _SEQUENT_t;

typedef enum {
    COM0 = (uint8_t)0x00, /*!< COM 0  */
    COM1 = (uint8_t)0x01, /*!< COM 1  */
    COM2 = (uint8_t)0x02, /*!< COM 2  */
    COM3 = (uint8_t)0x03, /*!< COM 3  */
    COM4 = (uint8_t)0x04, /*!< COM 4  */
    COM5 = (uint8_t)0x05, /*!< COM 5  */
    COM6 = (uint8_t)0x06, /*!< COM 6  */
    COM7 = (uint8_t)0x07, /*!< COM 7  */
} LCD_COMx_TypeDef;

typedef enum {
    SEG00 = (uint8_t)0x00, /*!< SEG00  */
    SEG01 = (uint8_t)0x01, /*!< SEG01  */
    SEG02 = (uint8_t)0x02, /*!< SEG02  */
    SEG03 = (uint8_t)0x03, /*!< SEG03  */
    SEG04 = (uint8_t)0x04, /*!< SEG04  */
    SEG05 = (uint8_t)0x05, /*!< SEG05  */
    SEG06 = (uint8_t)0x06, /*!< SEG06  */
    SEG07 = (uint8_t)0x07, /*!< SEG07  */
    SEG08 = (uint8_t)0x08, /*!< SEG08  */
    SEG09 = (uint8_t)0x09, /*!< SEG09  */
    SEG10 = (uint8_t)0x0A, /*!< SEG10  */
    SEG11 = (uint8_t)0x0B, /*!< SEG11  */
    SEG12 = (uint8_t)0x0C, /*!< SEG12  */
    SEG13 = (uint8_t)0x0D, /*!< SEG13  */
    SEG14 = (uint8_t)0x0E, /*!< SEG14  */
    SEG15 = (uint8_t)0x0F, /*!< SEG15  */
    SEG16 = (uint8_t)0x10, /*!< SEG16  */
    SEG17 = (uint8_t)0x11, /*!< SEG17  */
    SEG18 = (uint8_t)0x12, /*!< SEG18  */
    SEG19 = (uint8_t)0x13, /*!< SEG19  */
    SEG20 = (uint8_t)0x14, /*!< SEG20  */
    SEG21 = (uint8_t)0x15, /*!< SEG21  */
    SEG22 = (uint8_t)0x16, /*!< SEG22  */
    SEG23 = (uint8_t)0x17, /*!< SEG23  */
    SEG24 = (uint8_t)0x18, /*!< SEG24  */
    SEG25 = (uint8_t)0x19, /*!< SEG25  */
    SEG26 = (uint8_t)0x1A, /*!< SEG26  */
    SEG27 = (uint8_t)0x1B, /*!< SEG27  */
    SEG28 = (uint8_t)0x1C, /*!< SEG28  */
    SEG29 = (uint8_t)0x1D, /*!< SEG29  */
    SEG30 = (uint8_t)0x1E, /*!< SEG30  */
    SEG31 = (uint8_t)0x1F, /*!< SEG31  */
    SEG32 = (uint8_t)0x20, /*!< SEG32  */
    SEG33 = (uint8_t)0x21, /*!< SEG33  */
    SEG34 = (uint8_t)0x22, /*!< SEG34  */
    SEG35 = (uint8_t)0x23, /*!< SEG35  */
    SEG36 = (uint8_t)0x24, /*!< SEG36  */
    SEG37 = (uint8_t)0x25, /*!< SEG37  */
    SEG38 = (uint8_t)0x26, /*!< SEG38  */
    SEG39 = (uint8_t)0x27, /*!< SEG39  */
    SEG40 = (uint8_t)0x28, /*!< SEG40  */
    SEG41 = (uint8_t)0x29, /*!< SEG41  */
    SEG42 = (uint8_t)0x2A, /*!< SEG42  */
    SEG43 = (uint8_t)0x2B, /*!< SEG43  */
    SEG44 = (uint8_t)0x2C, /*!< SEG44  */
    SEG45 = (uint8_t)0x2D, /*!< SEG45  */
    SEG46 = (uint8_t)0x2E, /*!< SEG46  */
    SEG47 = (uint8_t)0x2F, /*!< SEG47  */
} LCD_SEGx_TypeDef;

typedef enum {
    Serial_01 = 0X00,
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
    Serial_MAX,
} LCD_Digital_Serial_t;

typedef union {
    uint16_t WORD;
    uint8_t BYTE[2];
    struct
    {
        unsigned char Digital_GDA : 4;  //word 8-11  Data[0] 4-7
        unsigned char Digital_FECB : 4; //word 12-15  Data[0] 4-7
        unsigned char Digital_IJK : 4;  //word 0-3    Data[0] 0-3
        unsigned char Digital_HML : 4;  //word 8-11   Data[1] 0-3
    };
} __Digital_Coding_t;

typedef enum {
    POSITIVE,
    NEGATIVE,
} LCD_Coding_Dirtction_t;

typedef enum {
    TIME_PART = 0,
    ALARM_PART,
    WEEKTYPEMAX,
} LCD_Time_Type_t;

typedef enum {
    SUNDAY = 0,
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    WEEKDAYMAX,
} LCD_Week_Day_t;

typedef enum {
    AM = 0,
    PM,
    AMPMMAX,
    AMPMNUM,
} LCD_AMPM_Distinguish_t;

typedef enum {
    LEVEL0 = 0x00,
    LEVEL1 = 0x08,
    LEVEL2 = 0x0C,
    LEVEL3 = 0x0E,
    LEVEL4 = 0x0F,
} LCD_Wifi_Rssi_t;

/* Private define ------------------------------------------------------------*/
//HT162x_CS
#define HT162x_CS_PORT GPIOD
#define HT162x_CS_PIN GPIO_Pin_4
#define HT162x_CS_SET GPIO_WriteBit(HT162x_CS_PORT, HT162x_CS_PIN, SET)
#define HT162x_CS_RESET GPIO_WriteBit(HT162x_CS_PORT, HT162x_CS_PIN, RESET)
//HT162x_RD
#define HT162x_RD_PORT GPIOB
#define HT162x_RD_PIN GPIO_Pin_7
#define HT162x_RD_SET GPIO_WriteBit(HT162x_RD_PORT, HT162x_RD_PIN, SET)
#define HT162x_RD_RESET GPIO_WriteBit(HT162x_RD_PORT, HT162x_RD_PIN, RESET)
//HT162x_RD
#define HT162x_WR_PORT GPIOB
#define HT162x_WR_PIN GPIO_Pin_6
#define HT162x_WR_SET GPIO_WriteBit(HT162x_WR_PORT, HT162x_WR_PIN, SET)
#define HT162x_WR_RESET GPIO_WriteBit(HT162x_WR_PORT, HT162x_WR_PIN, RESET)
//HT162x_DATA
#define HT162x_DATA_PORT GPIOB
#define HT162x_DATA_PIN GPIO_Pin_5
#define HT162x_DATA_SET GPIO_WriteBit(HT162x_DATA_PORT, HT162x_DATA_PIN, SET)
#define HT162x_DATA_RESET GPIO_WriteBit(HT162x_DATA_PORT, HT162x_DATA_PIN, RESET)
#define HT162x_DATA_SET_OUT GPIO_Init(HT162x_DATA_PORT, HT162x_DATA_PIN, GPIO_Mode_Out_PP_High_Fast)
#define HT162x_DATA_SET_IN GPIO_Init(HT162x_DATA_PORT, HT162x_DATA_PIN, GPIO_Mode_In_PU_No_IT)

#define HT162x_DATA_IN GPIO_ReadInputDataBit(HT162x_DATA_PORT, HT162x_DATA_PIN)

#define CMD_ID_READ 0XC0
#define CMD_ID_WRITE 0XA0
#define CMD_ID_WR_MODIFY_RD 0XA0
#define CMD_ID_COMMAND 0X80

#define LEN_CMD_ID 3
#define LEN_ADDRESS 7
#define LEN_DATA 4
#define LEN_COMMMAND 9
#define HT1623_MEM_NUM 96
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void HT162x_init(void);
void HT162x_LCD_Clear(FlagStatus value);
void HT162x_LCD_Change_COMx(LCD_COMx_TypeDef comx, FlagStatus value);
void HT162x_LCD_Wtrtie_SEGxData(LCD_SEGx_TypeDef segx, uint8_t data);
void HT162x_LCD_Change_Pixel(LCD_COMx_TypeDef comx, LCD_SEGx_TypeDef segx, FlagStatus value);
void HT162x_LCD_Toggle_Pixel(LCD_COMx_TypeDef comx, LCD_SEGx_TypeDef segx);
uint8_t HT162x_Read_Data(uint8_t address);
void HT162x_Write_Data(uint8_t addrass, uint8_t data);
void HT162x_LCD_Num_Set(LCD_Digital_Serial_t Serial, u8 data);
void HT162x_LCD_Week_Set(LCD_Time_Type_t type, LCD_Week_Day_t day);
void HT162x_LCD_RSSI_Set(LCD_Wifi_Rssi_t value);
void HT162x_LCD_AMPM_Set(LCD_Time_Type_t type, LCD_AMPM_Distinguish_t value);
void HT162x_LCD_Time_Display(LCD_Time_Type_t type, mico_rtc_time_t time);
void HT162x_LCD_Date_Display(LCD_Time_Type_t type, mico_rtc_time_t time);
/* Private functions ---------------------------------------------------------*/

#endif /*__HT162x_H_*/
