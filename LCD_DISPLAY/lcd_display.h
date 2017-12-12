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

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void LCD_Display_Rssi_State(Eland_Status_type_t state);

#endif /*__LCD_DISPLAY_H_*/
