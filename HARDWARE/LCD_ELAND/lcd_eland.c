/**
 ****************************************************************************
 * @Warning :Without permission from the author,Not for commercial use
 * @File    :lcd_eland.c
 * @Author  :Xiaowine
 * @date    :2017/10/23
 * @version :V 1.0.0
 *************************************************
 * @brief   :
 ****************************************************************************
**/
/* Private include -----------------------------------------------------------*/
#include "lcd_eland.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
 ****************************************************************************
 * @Function : void LCD_ELAND_Init(void)
 * @File     : lcd_eland.c
 * @Program  : none
 * @Created  : 2017/10/23 by Xiaowine
 * @Brief    : init lcd driver
 * @Version  : V1.0
**/
void LCD_ELAND_Init(void)
{ /*
    The LCD is configured as follow:
     - clock source = LSE (32.768 KHz)
     - Voltage source = Internal
     - Prescaler = 2
     - Divider = 18 (16 + 2)
     - Mode = 1/8 Duty, 1/4 Bias
     - LCD frequency = (clock source * Duty) / (Prescaler * Divider)
                     = 114 Hz ==> Frame frequency = 28,5 Hz*/

    /* Enable LCD clock */
    CLK_PeripheralClockConfig(CLK_Peripheral_LCD, ENABLE);
    CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1);

    /* Initialize the LCD */
    LCD_Init(LCD_Prescaler_2, LCD_Divider_18, LCD_Duty_1_8,
             LCD_Bias_1_4, LCD_VoltageSource_Internal);

    /* Mask register*/
    LCD_PortMaskConfig(LCD_PortMaskRegister_0, 0xFF);
    LCD_PortMaskConfig(LCD_PortMaskRegister_1, 0xFF);
    LCD_PortMaskConfig(LCD_PortMaskRegister_2, 0xFF);
    LCD_PortMaskConfig(LCD_PortMaskRegister_3, 0xFF);
    LCD_PortMaskConfig(LCD_PortMaskRegister_4, 0xFF);

    LCD_ContrastConfig(LCD_Contrast_Level_7);

    LCD_PulseOnDurationConfig(LCD_PulseOnDuration_7);

    LCD_Cmd(ENABLE); /*!< Enable LCD peripheral */
}
/**
 ****************************************************************************
 * @Function : void ELAND_LCD_Clear(void)
 * @File     : lcd_eland.c
 * @Program  : none
 * @Created  : 2017/10/24 by Xiaowine
 * @Brief    : clear lcd
 * @Version  : V1.0
**/
void ELAND_LCD_Clear(void)
{
    uint8_t counter = 0x00;

    /* Enable the write access on the LCD RAM First banck */
    LCD->CR4 &= (uint8_t)(~LCD_CR4_PAGECOM);

    for (counter = 0x0; counter < 0x16; counter++)
    {
        LCD->RAM[counter] = LCD_RAM_RESET_VALUE;
    }

    /* Enable the write access on the LCD RAM second banck */
    LCD->CR4 |= LCD_CR4_PAGECOM;

    for (counter = 0x0; counter < 0x16; counter++)
    {
        LCD->RAM[counter] = LCD_RAM_RESET_VALUE;
    }
}
/**
 ****************************************************************************
 * @Function : void LCD_ELAND_Display_All(void)
 * @File     : lcd_eland.c
 * @Program  : none
 * @Created  : 2017/10/24 by Xiaowine
 * @Brief    : dispaly all
 * @Version  : V1.0
**/
void LCD_ELAND_Display_All(void)
{
    uint8_t counter = 0x00;

    /* Enable the write access on the LCD RAM First banck */
    LCD->CR4 &= (uint8_t)(~LCD_CR4_PAGECOM);

    for (counter = 0x0; counter < 0x10; counter++)
    {
        LCD->RAM[counter] = 0xff;
    }

    /* Enable the write access on the LCD RAM second banck */
    LCD->CR4 |= LCD_CR4_PAGECOM;

    for (counter = 0x0; counter < 0x10; counter++)
    {
        LCD->RAM[counter] = 0xff;
    }
}
/**
 ****************************************************************************
 * @Function : void LCD_Eland_Pixel_Write(u8 COMx,u8 SEGn)
 * @File     : lcd_eland.c
 * @Program  : COMx：witch com  0~7
 *             SEGn：witch segial pin 0~39
 * @Created  : 2017/10/26 by Xiaowine
 * @Brief    : pixel write
 * @Version  : V1.0
**/
void LCD_Eland_Pixel_Write(u8 COMx, u8 SEGn)
{
    u16 Cache = 0;

    if (SEGn < 28)
        Cache = 28 * (COMx % 4) + SEGn;
    else
        Cache = 112 + (COMx % 4) * 16 + (SEGn - 28);

    if (COMx < 4)
        LCD->CR4 &= (uint8_t)(~LCD_CR4_PAGECOM);
    else
        LCD->CR4 |= LCD_CR4_PAGECOM;

    LCD->RAM[(Cache / 8)] |= (1 << (Cache % 8));
}
/**
 ****************************************************************************
 * @Function : void LCD_Eland_Pixel_Clear(u8 COMx, u8 SEGn)
 * @File     : lcd_eland.c
 * @Program  : COMx：witch com  0~7
 *             SEGn：witch segial pin 0~39
 * @Created  : 2017/10/26 by Xiaowine
 * @Brief    : pixel clear
 * @Version  : V1.0
**/
void LCD_Eland_Pixel_Clear(u8 COMx, u8 SEGn)
{
    u16 Cache = 0;

    if (SEGn < 28)
        Cache = 28 * (COMx % 4) + SEGn;
    else
        Cache = 112 + (COMx % 4) * 16 + (SEGn - 28);

    if (COMx < 4)
        LCD->CR4 &= (uint8_t)(~LCD_CR4_PAGECOM);
    else
        LCD->CR4 |= LCD_CR4_PAGECOM;

    LCD->RAM[(Cache / 8)] &= (~(1 << (Cache % 8)));
}
