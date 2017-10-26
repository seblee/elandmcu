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
/* Private define ------------------------------------------------------------*/

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
/* Private functions ---------------------------------------------------------*/

#endif /*__LCD_ELAND_H_*/
