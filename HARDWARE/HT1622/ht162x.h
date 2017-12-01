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
    MSB,
    LSB
} __SEQUENT_t;
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
#define HT162x_WR_PIN GPIO_Pin_5
#define HT162x_WR_SET GPIO_WriteBit(HT162x_WR_PORT, HT162x_WR_PIN, SET)
#define HT162x_WR_RESET GPIO_WriteBit(HT162x_WR_PORT, HT162x_WR_PIN, RESET)
//HT162x_DATA
#define HT162x_DATA_PORT GPIOB
#define HT162x_DATA_PIN GPIO_Pin_6
#define HT162x_DATA_SET GPIO_WriteBit(HT162x_DATA_PORT, HT162x_DATA_PIN, SET)
#define HT162x_DATA_RESET GPIO_WriteBit(HT162x_DATA_PORT, HT162x_DATA_PIN, RESET)
#define HT162x_DATA_SET_OUT GPIO_Init(HT162x_DATA_PORT, HT162x_DATA_PIN, GPIO_Mode_Out_PP_High_Fast)
#define HT162x_DATA_SET_IN GPIO_Init(HT162x_DATA_PORT, HT162x_DATA_PIN, GPIO_Mode_In_FL_No_IT)

#define HT162x_DATA_IN GPIO_ReadInputDataBit(HT162x_DATA_PORT, HT162x_DATA_PIN)

#define CMD_ID_READ 0XC0
#define CMD_ID_WRITE 0XA0
#define CMD_ID_COMMAND 0X80

#define LEN_CMD_ID 3
#define LEN_ADDRESS 7
#define LEN_DATA 4
#define LEN_COMMMAND 9

#define HT1623_MEM_NUM 96
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

#endif /*__HT162x_H_*/
