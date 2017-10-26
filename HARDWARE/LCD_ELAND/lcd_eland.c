/**
 ****************************************************************************
 * @Warning :Without permission from the author,Not for commercial use
 * @File    :lcd_eland.c
 * @Author  :seblee
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
 * @Created  : 2017/10/23 by seblee
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
 * @Function : void LCD_ELAND_Clear(void)
 * @File     : lcd_eland.c
 * @Program  : none
 * @Created  : 2017/10/24 by seblee
 * @Brief    : clear lcd
 * @Version  : V1.0
**/
void LCD_ELAND_Clear(void)
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
 * @Function : void LCD_ELAND_Write_All(void)
 * @File     : lcd_eland.c
 * @Program  : none
 * @Created  : 2017/10/24 by seblee
 * @Brief    : dispaly all
 * @Version  : V1.0
**/
void LCD_ELAND_Write_All(void)
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
 * @Created  : 2017/10/26 by seblee
 * @Brief    : pixel write
 * @Version  : V1.0
**/
void LCD_Eland_Pixel_Write(LCD_COMx_TypeDef COMx, u8 SEGn)
{
    u16 Cache = 0;

    if (SEGn < 28)
        Cache = 28 * (COMx % 4) + SEGn;
    else
        Cache = 112 + (COMx % 4) * 16 + (SEGn - 28);

    if (COMx < COM_4)
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
 * @Created  : 2017/10/26 by seblee
 * @Brief    : pixel clear
 * @Version  : V1.0
**/
void LCD_Eland_Pixel_Clear(LCD_COMx_TypeDef COMx, u8 SEGn)
{
    u16 Cache = 0;

    if (SEGn < 28)
        Cache = 28 * (COMx % 4) + SEGn;
    else
        Cache = 112 + (COMx % 4) * 16 + (SEGn - 28);

    if (COMx < COM_4)
        LCD->CR4 &= (uint8_t)(~LCD_CR4_PAGECOM);
    else
        LCD->CR4 |= LCD_CR4_PAGECOM;

    LCD->RAM[(Cache / 8)] &= (~(1 << (Cache % 8)));
}
/**
 ****************************************************************************
 * @Function : void LCD_Eland_COMx_Write(LCD_COMx_TypeDef COMx)
 * @File     : lcd_eland.c
 * @Program  : COMx:witch com  0~7
 * @Created  : 2017/10/26 by seblee
 * @Brief    : write total line
 * @Version  : V1.0
**/
void LCD_Eland_COMx_Write(LCD_COMx_TypeDef COMx)
{
    switch (COMx)
    {
    case COM_0:
        LCD->CR4 &= (uint8_t)(~LCD_CR4_PAGECOM);
        LCD->RAM[LCD_RAMRegister_0] |= 0xff;
        LCD->RAM[LCD_RAMRegister_1] |= 0xff;
        LCD->RAM[LCD_RAMRegister_2] |= 0xff;
        LCD->RAM[LCD_RAMRegister_3] |= 0x0f;
        LCD->RAM[LCD_RAMRegister_14] |= 0xff;
        LCD->RAM[LCD_RAMRegister_15] |= 0x0f;

        break;
    case COM_1:
        LCD->CR4 &= (uint8_t)(~LCD_CR4_PAGECOM);
        LCD->RAM[LCD_RAMRegister_3] |= 0xf0;
        LCD->RAM[LCD_RAMRegister_4] |= 0xff;
        LCD->RAM[LCD_RAMRegister_5] |= 0xff;
        LCD->RAM[LCD_RAMRegister_6] |= 0xff;
        LCD->RAM[LCD_RAMRegister_16] |= 0xff;
        LCD->RAM[LCD_RAMRegister_17] |= 0x0f;
        break;
    case COM_2:
        LCD->CR4 &= (uint8_t)(~LCD_CR4_PAGECOM);
        LCD->RAM[LCD_RAMRegister_7] |= 0xff;
        LCD->RAM[LCD_RAMRegister_8] |= 0xff;
        LCD->RAM[LCD_RAMRegister_9] |= 0xff;
        LCD->RAM[LCD_RAMRegister_10] |= 0x0f;
        LCD->RAM[LCD_RAMRegister_18] |= 0xff;
        LCD->RAM[LCD_RAMRegister_19] |= 0x0f;
        break;
    case COM_3:
        LCD->CR4 &= (uint8_t)(~LCD_CR4_PAGECOM);
        LCD->RAM[LCD_RAMRegister_10] |= 0xf0;
        LCD->RAM[LCD_RAMRegister_11] |= 0xff;
        LCD->RAM[LCD_RAMRegister_12] |= 0xff;
        LCD->RAM[LCD_RAMRegister_13] |= 0xff;
        LCD->RAM[LCD_RAMRegister_20] |= 0xff;
        LCD->RAM[LCD_RAMRegister_21] |= 0x0f;
        break;
    case COM_4:
        LCD->CR4 |= LCD_CR4_PAGECOM;
        LCD->RAM[LCD_RAMRegister_0] |= 0xff;
        LCD->RAM[LCD_RAMRegister_1] |= 0xff;
        LCD->RAM[LCD_RAMRegister_2] |= 0xff;
        LCD->RAM[LCD_RAMRegister_3] |= 0x0f;
        LCD->RAM[LCD_RAMRegister_14] |= 0xff;
        LCD->RAM[LCD_RAMRegister_15] |= 0x0f;

        break;
    case COM_5:
        LCD->CR4 |= LCD_CR4_PAGECOM;
        LCD->RAM[LCD_RAMRegister_3] |= 0xf0;
        LCD->RAM[LCD_RAMRegister_4] |= 0xff;
        LCD->RAM[LCD_RAMRegister_5] |= 0xff;
        LCD->RAM[LCD_RAMRegister_6] |= 0xff;
        LCD->RAM[LCD_RAMRegister_16] |= 0xff;
        LCD->RAM[LCD_RAMRegister_17] |= 0x0f;
        break;
    case COM_6:
        LCD->CR4 |= LCD_CR4_PAGECOM;
        LCD->RAM[LCD_RAMRegister_7] |= 0xff;
        LCD->RAM[LCD_RAMRegister_8] |= 0xff;
        LCD->RAM[LCD_RAMRegister_9] |= 0xff;
        LCD->RAM[LCD_RAMRegister_10] |= 0x0f;
        LCD->RAM[LCD_RAMRegister_18] |= 0xff;
        LCD->RAM[LCD_RAMRegister_19] |= 0x0f;
        break;
    case COM_7:
        LCD->CR4 |= LCD_CR4_PAGECOM;
        LCD->RAM[LCD_RAMRegister_10] |= 0xf0;
        LCD->RAM[LCD_RAMRegister_11] |= 0xff;
        LCD->RAM[LCD_RAMRegister_12] |= 0xff;
        LCD->RAM[LCD_RAMRegister_13] |= 0xff;
        LCD->RAM[LCD_RAMRegister_20] |= 0xff;
        LCD->RAM[LCD_RAMRegister_21] |= 0x0f;
        break;
    default:
        break;
    }
}
/**
 ****************************************************************************
 * @Function : void LCD_Eland_COMx_Clear(LCD_COMx_TypeDef COMx)
 * @File     : lcd_eland.c
 * @Program  : COMx:witch com  0~7
 * @Created  : 2017/10/26 by seblee
 * @Brief    : clear total line
 * @Version  : V1.0
**/
void LCD_Eland_COMx_Clear(LCD_COMx_TypeDef COMx)
{
    switch (COMx)
    {
    case COM_0:
        LCD->CR4 &= (uint8_t)(~LCD_CR4_PAGECOM);
        LCD->RAM[LCD_RAMRegister_0] &= 0x00;
        LCD->RAM[LCD_RAMRegister_1] &= 0x00;
        LCD->RAM[LCD_RAMRegister_2] &= 0x00;
        LCD->RAM[LCD_RAMRegister_3] &= 0xf0;
        LCD->RAM[LCD_RAMRegister_14] &= 0x00;
        LCD->RAM[LCD_RAMRegister_15] &= 0xf0;

        break;
    case COM_1:
        LCD->CR4 &= (uint8_t)(~LCD_CR4_PAGECOM);
        LCD->RAM[LCD_RAMRegister_3] &= 0x0f;
        LCD->RAM[LCD_RAMRegister_4] &= 0x00;
        LCD->RAM[LCD_RAMRegister_5] &= 0x00;
        LCD->RAM[LCD_RAMRegister_6] &= 0x00;
        LCD->RAM[LCD_RAMRegister_16] &= 0x00;
        LCD->RAM[LCD_RAMRegister_17] &= 0xf0;
        break;
    case COM_2:
        LCD->CR4 &= (uint8_t)(~LCD_CR4_PAGECOM);
        LCD->RAM[LCD_RAMRegister_7] &= 0x00;
        LCD->RAM[LCD_RAMRegister_8] &= 0x00;
        LCD->RAM[LCD_RAMRegister_9] &= 0x00;
        LCD->RAM[LCD_RAMRegister_10] &= 0xf0;
        LCD->RAM[LCD_RAMRegister_18] &= 0x00;
        LCD->RAM[LCD_RAMRegister_19] &= 0xf0;
        break;
    case COM_3:
        LCD->CR4 &= (uint8_t)(~LCD_CR4_PAGECOM);
        LCD->RAM[LCD_RAMRegister_10] &= 0x0f;
        LCD->RAM[LCD_RAMRegister_11] &= 0x00;
        LCD->RAM[LCD_RAMRegister_12] &= 0x00;
        LCD->RAM[LCD_RAMRegister_13] &= 0x00;
        LCD->RAM[LCD_RAMRegister_20] &= 0x00;
        LCD->RAM[LCD_RAMRegister_21] &= 0xf0;
        break;
    case COM_4:
        LCD->CR4 |= LCD_CR4_PAGECOM;
        LCD->RAM[LCD_RAMRegister_0] &= 0x00;
        LCD->RAM[LCD_RAMRegister_1] &= 0x00;
        LCD->RAM[LCD_RAMRegister_2] &= 0x00;
        LCD->RAM[LCD_RAMRegister_3] &= 0xf0;
        LCD->RAM[LCD_RAMRegister_14] &= 0x00;
        LCD->RAM[LCD_RAMRegister_15] &= 0xf0;

        break;
    case COM_5:
        LCD->CR4 |= LCD_CR4_PAGECOM;
        LCD->RAM[LCD_RAMRegister_3] &= 0x0f;
        LCD->RAM[LCD_RAMRegister_4] &= 0x00;
        LCD->RAM[LCD_RAMRegister_5] &= 0x00;
        LCD->RAM[LCD_RAMRegister_6] &= 0x00;
        LCD->RAM[LCD_RAMRegister_16] &= 0x00;
        LCD->RAM[LCD_RAMRegister_17] &= 0xf0;
        break;
    case COM_6:
        LCD->CR4 |= LCD_CR4_PAGECOM;
        LCD->RAM[LCD_RAMRegister_7] &= 0x00;
        LCD->RAM[LCD_RAMRegister_8] &= 0x00;
        LCD->RAM[LCD_RAMRegister_9] &= 0x00;
        LCD->RAM[LCD_RAMRegister_10] &= 0xf0;
        LCD->RAM[LCD_RAMRegister_18] &= 0x00;
        LCD->RAM[LCD_RAMRegister_19] &= 0xf0;
        break;
    case COM_7:
        LCD->CR4 |= LCD_CR4_PAGECOM;
        LCD->RAM[LCD_RAMRegister_10] &= 0x0f;
        LCD->RAM[LCD_RAMRegister_11] &= 0x00;
        LCD->RAM[LCD_RAMRegister_12] &= 0x00;
        LCD->RAM[LCD_RAMRegister_13] &= 0x00;
        LCD->RAM[LCD_RAMRegister_20] &= 0x00;
        LCD->RAM[LCD_RAMRegister_21] &= 0xf0;
        break;
    default:
        break;
    }
}
