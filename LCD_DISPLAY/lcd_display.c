/**
 ****************************************************************************
 * @Warning :Without permission from the author,Not for commercial use
 * @File    :lcd_display.c
 * @Author  :seblee
 * @date    :2017/12/12
 * @version :V 1.0.0
 *************************************************
 * @brief   :
 ****************************************************************************
**/
/* Private include -----------------------------------------------------------*/
#include "lcd_display.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
 ****************************************************************************
 * @Function : void LCD_Display_State(Eland_Status_type_t state)
 * @File     : lcd_display.c
 * @Program  : state:the state get from eland
 * @Created  : 2017/12/12 by seblee
 * @Brief    : display state
 * @Version  : V1.0
**/
void LCD_Display_State(Eland_Status_type_t state)
{
    switch (state)
    {
    case APStatus:
        break;
    default:
        break;
    }
}
/**
 ****************************************************************************
 * @Function : void LCD_Display_Rssi_State(Eland_Status_type_t state)
 * @File     : lcd_display.c
 * @Program  : state:the state get from eland
 * @Created  : 2017/12/12 by seblee
 * @Brief    : display RSSI state
 * @Version  : V1.0
**/
void LCD_Display_Rssi_State(Eland_Status_type_t state)
{
    static LCD_Wifi_Rssi_t rssi_value = LEVEL0;
    if ((state == APStatus) || (state == HttpServerStatus))
    {
        if (rssi_value == LEVEL0)
            rssi_value = LEVEL4;
        else
            rssi_value = LEVEL0;
        HT162x_LCD_RSSI_Set(rssi_value);
    }
    else
    {
        if (RSSI_Value >= RSSI_STATE_STAGE4)
            rssi_value = LEVEL4;
        else if (RSSI_Value >= RSSI_STATE_STAGE3)
            rssi_value = LEVEL3;
        else if (RSSI_Value >= RSSI_STATE_STAGE2)
            rssi_value = LEVEL2;
        else if (RSSI_Value >= RSSI_STATE_STAGE1)
            rssi_value = LEVEL1;
        else
            rssi_value = LEVEL0;
        HT162x_LCD_RSSI_Set(rssi_value);
    }
}
