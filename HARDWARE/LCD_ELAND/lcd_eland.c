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
#include "usart.h"
#include "key.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/**
================================================================================
                              GLASS LCD MAPPING
================================================================================

          L----A----B
          |         |
         K|         |C
          |         |
          J----M----D
          |         |
         I|         |E
          |         |
          H----G----F

A LCD __Digital_Coding_t is based on the following matrix:
             000B    CALK    EDMJ    FGHI
  SEG(n)    { 0 ,     K ,     J ,     I }
  SEG(n+1)  { 0 ,     L ,     M ,     H }
  SEG(n+2)  { 0 ,     A ,     D ,     G }
  SEG(n+3)  { B ,     C ,     E ,     F }

The character A for example is:
-----------------------------------------------------------
             COM0    COM1     COM2    COM3
  SEG(n)    { 0 ,     0 ,      0 ,     1 }
  SEG(n+1)  { 0 ,     0 ,      1 ,     0 }
  SEG(n+2)  { 0 ,     0 ,      0 ,     1 }
  SEG(n+3)  { 0 ,     0 ,      0 ,     1 }
   --------------------------------------------------------
           =  0       0        2       5  hex

  */
CONST uint16_t NumberMap[10] = {
    /* 0       1       2       3       4   */
    0X1FDF, 0X18C8, 0X1F7E, 0X1EFE, 0X18FB,
    /* 5       6       7       8       9   */
    0X1EF7, 0X1FF7, 0X18CE, 0XFFFF, 0X1EFF};
/* Private function prototypes -----------------------------------------------*/
static __Digital_Coding_t LCD_Eland_Digital_Convert(LCD_Coding_Dirtction_t direction, uint8_t Data);
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
{
#ifndef testlcdIO
    /*
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
    LCD_Init(LCD_Prescaler_2, LCD_Divider_16, LCD_Duty_1_8,
             LCD_Bias_1_4, LCD_VoltageSource_External);

    /* Mask register*/
    LCD_PortMaskConfig(LCD_PortMaskRegister_0, 0xFF);
    LCD_PortMaskConfig(LCD_PortMaskRegister_1, 0xFF);
    LCD_PortMaskConfig(LCD_PortMaskRegister_2, 0xFF);
    LCD_PortMaskConfig(LCD_PortMaskRegister_3, 0xFF);
    LCD_PortMaskConfig(LCD_PortMaskRegister_4, 0xFF);

    LCD_ContrastConfig(LCD_Contrast_Level_7);

    LCD_DeadTimeConfig(LCD_DeadTime_0);

    LCD_PulseOnDurationConfig(LCD_PulseOnDuration_1);

    LCD_HighDriveCmd(ENABLE);

    LCD_Cmd(ENABLE); /*!< Enable LCD peripheral */
#else
    GPIO_Init(GPIOA, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Fast); //COM0
    GPIO_Init(GPIOA, GPIO_Pin_5, GPIO_Mode_Out_PP_High_Fast); //COM1
    GPIO_Init(GPIOA, GPIO_Pin_6, GPIO_Mode_Out_PP_High_Fast); //COM2
    GPIO_Init(GPIOD, GPIO_Pin_1, GPIO_Mode_Out_PP_High_Fast); //COM3
    GPIO_Init(GPIOF, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Fast); //COM4
    GPIO_Init(GPIOF, GPIO_Pin_5, GPIO_Mode_Out_PP_High_Fast); //COM5
    GPIO_Init(GPIOF, GPIO_Pin_6, GPIO_Mode_Out_PP_High_Fast); //COM6
    GPIO_Init(GPIOF, GPIO_Pin_7, GPIO_Mode_Out_PP_High_Fast); //COM7

    GPIO_Init(GPIOA, GPIO_Pin_7, GPIO_Mode_Out_PP_High_Fast); //SEG0
    GPIO_Init(GPIOE, GPIO_Pin_0, GPIO_Mode_Out_PP_High_Fast); //SEG1
    GPIO_Init(GPIOE, GPIO_Pin_1, GPIO_Mode_Out_PP_High_Fast); //SEG2
    GPIO_Init(GPIOE, GPIO_Pin_2, GPIO_Mode_Out_PP_High_Fast); //SEG3
    GPIO_Init(GPIOE, GPIO_Pin_3, GPIO_Mode_Out_PP_High_Fast); //SEG4
    GPIO_Init(GPIOE, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Fast); //SEG5
    GPIO_Init(GPIOE, GPIO_Pin_5, GPIO_Mode_Out_PP_High_Fast); //SEG6
    GPIO_Init(GPIOD, GPIO_Pin_0, GPIO_Mode_Out_PP_High_Fast); //SEG7
    GPIO_Init(GPIOD, GPIO_Pin_2, GPIO_Mode_Out_PP_High_Fast); //SEG8
    GPIO_Init(GPIOD, GPIO_Pin_3, GPIO_Mode_Out_PP_High_Fast); //SEG9
    GPIO_Init(GPIOB, GPIO_Pin_0, GPIO_Mode_Out_PP_High_Fast); //SEG10
    GPIO_Init(GPIOB, GPIO_Pin_1, GPIO_Mode_Out_PP_High_Fast); //SEG11
    GPIO_Init(GPIOB, GPIO_Pin_2, GPIO_Mode_Out_PP_High_Fast); //SEG12
    GPIO_Init(GPIOB, GPIO_Pin_3, GPIO_Mode_Out_PP_High_Fast); //SEG13
    GPIO_Init(GPIOB, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Fast); //SEG14
    GPIO_Init(GPIOB, GPIO_Pin_5, GPIO_Mode_Out_PP_High_Fast); //SEG15
    GPIO_Init(GPIOB, GPIO_Pin_6, GPIO_Mode_Out_PP_High_Fast); //SEG16
    GPIO_Init(GPIOB, GPIO_Pin_7, GPIO_Mode_Out_PP_High_Fast); //SEG17
    GPIO_Init(GPIOD, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Fast); //SEG18
    GPIO_Init(GPIOD, GPIO_Pin_5, GPIO_Mode_Out_PP_High_Fast); //SEG19
    GPIO_Init(GPIOD, GPIO_Pin_6, GPIO_Mode_Out_PP_High_Fast); //SEG20
    GPIO_Init(GPIOD, GPIO_Pin_7, GPIO_Mode_Out_PP_High_Fast); //SEG21
    GPIO_Init(GPIOC, GPIO_Pin_2, GPIO_Mode_Out_PP_High_Fast); //SEG22
    GPIO_Init(GPIOC, GPIO_Pin_3, GPIO_Mode_Out_PP_High_Fast); //SEG23
    GPIO_Init(GPIOC, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Fast); //SEG24
    GPIO_Init(GPIOC, GPIO_Pin_7, GPIO_Mode_Out_PP_High_Fast); //SEG25
    GPIO_Init(GPIOE, GPIO_Pin_6, GPIO_Mode_Out_PP_High_Fast); //SEG26
    GPIO_Init(GPIOE, GPIO_Pin_7, GPIO_Mode_Out_PP_High_Fast); //SEG27
    GPIO_Init(GPIOG, GPIO_Pin_0, GPIO_Mode_Out_PP_High_Fast); //SEG28
    GPIO_Init(GPIOG, GPIO_Pin_1, GPIO_Mode_Out_PP_High_Fast); //SEG29
    GPIO_Init(GPIOG, GPIO_Pin_2, GPIO_Mode_Out_PP_High_Fast); //SEG30
    GPIO_Init(GPIOG, GPIO_Pin_3, GPIO_Mode_Out_PP_High_Fast); //SEG31
    GPIO_Init(GPIOG, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Fast); //SEG32
    GPIO_Init(GPIOG, GPIO_Pin_5, GPIO_Mode_Out_PP_High_Fast); //SEG33
    GPIO_Init(GPIOG, GPIO_Pin_6, GPIO_Mode_Out_PP_High_Fast); //SEG34
    GPIO_Init(GPIOG, GPIO_Pin_7, GPIO_Mode_Out_PP_High_Fast); //SEG35
    GPIO_Init(GPIOH, GPIO_Pin_0, GPIO_Mode_Out_PP_High_Fast); //SEG36
    GPIO_Init(GPIOH, GPIO_Pin_1, GPIO_Mode_Out_PP_High_Fast); //SEG37
    GPIO_Init(GPIOH, GPIO_Pin_2, GPIO_Mode_Out_PP_High_Fast); //SEG38
    GPIO_Init(GPIOH, GPIO_Pin_3, GPIO_Mode_Out_PP_High_Fast); //SEG39

    GPIO_WriteBit(GPIOA, GPIO_Pin_4, SET);   //COM0
    GPIO_WriteBit(GPIOA, GPIO_Pin_5, RESET); //COM1
    GPIO_WriteBit(GPIOA, GPIO_Pin_6, RESET); //COM2
    GPIO_WriteBit(GPIOD, GPIO_Pin_1, RESET); //COM3
    GPIO_WriteBit(GPIOF, GPIO_Pin_4, RESET); //COM4
    GPIO_WriteBit(GPIOF, GPIO_Pin_5, RESET); //COM5
    GPIO_WriteBit(GPIOF, GPIO_Pin_6, RESET); //COM6
    GPIO_WriteBit(GPIOF, GPIO_Pin_7, RESET); //COM7

    GPIO_WriteBit(GPIOA, GPIO_Pin_7, RESET); //SEG0
    GPIO_WriteBit(GPIOE, GPIO_Pin_0, RESET); //SEG1
    GPIO_WriteBit(GPIOE, GPIO_Pin_1, RESET); //SEG2
    GPIO_WriteBit(GPIOE, GPIO_Pin_2, RESET); //SEG3
    GPIO_WriteBit(GPIOE, GPIO_Pin_3, RESET); //SEG4
    GPIO_WriteBit(GPIOE, GPIO_Pin_4, RESET); //SEG5
    GPIO_WriteBit(GPIOE, GPIO_Pin_5, RESET); //SEG6
    GPIO_WriteBit(GPIOD, GPIO_Pin_0, RESET); //SEG7
    GPIO_WriteBit(GPIOD, GPIO_Pin_2, RESET); //SEG8
    GPIO_WriteBit(GPIOD, GPIO_Pin_3, RESET); //SEG9
    GPIO_WriteBit(GPIOB, GPIO_Pin_0, RESET); //SEG10
    GPIO_WriteBit(GPIOB, GPIO_Pin_1, RESET); //SEG11
    GPIO_WriteBit(GPIOB, GPIO_Pin_2, RESET); //SEG12
    GPIO_WriteBit(GPIOB, GPIO_Pin_3, RESET); //SEG13
    GPIO_WriteBit(GPIOB, GPIO_Pin_4, RESET); //SEG14
    GPIO_WriteBit(GPIOB, GPIO_Pin_5, RESET); //SEG15
    GPIO_WriteBit(GPIOB, GPIO_Pin_6, RESET); //SEG16
    GPIO_WriteBit(GPIOB, GPIO_Pin_7, RESET); //SEG17
    GPIO_WriteBit(GPIOD, GPIO_Pin_4, RESET); //SEG18
    GPIO_WriteBit(GPIOD, GPIO_Pin_5, RESET); //SEG19
    GPIO_WriteBit(GPIOD, GPIO_Pin_6, RESET); //SEG20
    GPIO_WriteBit(GPIOD, GPIO_Pin_7, RESET); //SEG21
    GPIO_WriteBit(GPIOC, GPIO_Pin_2, RESET); //SEG22
    GPIO_WriteBit(GPIOC, GPIO_Pin_3, RESET); //SEG23
    GPIO_WriteBit(GPIOC, GPIO_Pin_4, RESET); //SEG24
    GPIO_WriteBit(GPIOC, GPIO_Pin_7, RESET); //SEG25
    GPIO_WriteBit(GPIOE, GPIO_Pin_6, RESET); //SEG26
    GPIO_WriteBit(GPIOE, GPIO_Pin_7, RESET); //SEG27
    GPIO_WriteBit(GPIOG, GPIO_Pin_0, RESET); //SEG28
    GPIO_WriteBit(GPIOG, GPIO_Pin_1, RESET); //SEG29
    GPIO_WriteBit(GPIOG, GPIO_Pin_2, RESET); //SEG30
    GPIO_WriteBit(GPIOG, GPIO_Pin_3, RESET); //SEG31
    GPIO_WriteBit(GPIOG, GPIO_Pin_4, RESET); //SEG32
    GPIO_WriteBit(GPIOG, GPIO_Pin_5, RESET); //SEG33
    GPIO_WriteBit(GPIOG, GPIO_Pin_6, RESET); //SEG34
    GPIO_WriteBit(GPIOG, GPIO_Pin_7, RESET); //SEG35
    GPIO_WriteBit(GPIOH, GPIO_Pin_0, RESET); //SEG36
    GPIO_WriteBit(GPIOH, GPIO_Pin_1, RESET); //SEG37
    GPIO_WriteBit(GPIOH, GPIO_Pin_2, RESET); //SEG38
    GPIO_WriteBit(GPIOH, GPIO_Pin_3, RESET); //SEG39
#endif
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
    LCD_PageSelect(LCD_PageSelection_FirstPage);

    for (counter = 0x0; counter < 0x16; counter++)
    {
        LCD->RAM[counter] = LCD_RAM_RESET_VALUE;
    }

    /* Enable the write access on the LCD RAM second banck */
    LCD_PageSelect(LCD_PageSelection_SecondPage);

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
    LCD_PageSelect(LCD_PageSelection_FirstPage);

    for (counter = 0x0; counter < 0x16; counter++)
    {
        LCD->RAM[counter] = 0xff;
    }

    /* Enable the write access on the LCD RAM second banck */
    LCD_PageSelect(LCD_PageSelection_SecondPage);

    for (counter = 0x0; counter < 0x16; counter++)
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
        LCD_PageSelect(LCD_PageSelection_FirstPage);
    else
        LCD_PageSelect(LCD_PageSelection_SecondPage);

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
        LCD_PageSelect(LCD_PageSelection_FirstPage);
    else
        LCD_PageSelect(LCD_PageSelection_SecondPage);

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
        LCD_PageSelect(LCD_PageSelection_FirstPage);
        LCD->RAM[LCD_RAMRegister_0] |= 0xff;
        LCD->RAM[LCD_RAMRegister_1] |= 0xff;
        LCD->RAM[LCD_RAMRegister_2] |= 0xff;
        LCD->RAM[LCD_RAMRegister_3] |= 0x0f;
        LCD->RAM[LCD_RAMRegister_14] |= 0xff;
        LCD->RAM[LCD_RAMRegister_15] |= 0x0f;

        break;
    case COM_1:
        LCD_PageSelect(LCD_PageSelection_FirstPage);
        LCD->RAM[LCD_RAMRegister_3] |= 0xf0;
        LCD->RAM[LCD_RAMRegister_4] |= 0xff;
        LCD->RAM[LCD_RAMRegister_5] |= 0xff;
        LCD->RAM[LCD_RAMRegister_6] |= 0xff;
        LCD->RAM[LCD_RAMRegister_16] |= 0xff;
        LCD->RAM[LCD_RAMRegister_17] |= 0x0f;
        break;
    case COM_2:
        LCD_PageSelect(LCD_PageSelection_FirstPage);
        LCD->RAM[LCD_RAMRegister_7] |= 0xff;
        LCD->RAM[LCD_RAMRegister_8] |= 0xff;
        LCD->RAM[LCD_RAMRegister_9] |= 0xff;
        LCD->RAM[LCD_RAMRegister_10] |= 0x0f;
        LCD->RAM[LCD_RAMRegister_18] |= 0xff;
        LCD->RAM[LCD_RAMRegister_19] |= 0x0f;
        break;
    case COM_3:
        LCD_PageSelect(LCD_PageSelection_FirstPage);
        LCD->RAM[LCD_RAMRegister_10] |= 0xf0;
        LCD->RAM[LCD_RAMRegister_11] |= 0xff;
        LCD->RAM[LCD_RAMRegister_12] |= 0xff;
        LCD->RAM[LCD_RAMRegister_13] |= 0xff;
        LCD->RAM[LCD_RAMRegister_20] |= 0xff;
        LCD->RAM[LCD_RAMRegister_21] |= 0x0f;
        break;
    case COM_4:
        LCD_PageSelect(LCD_PageSelection_SecondPage);
        LCD->RAM[LCD_RAMRegister_0] |= 0xff;
        LCD->RAM[LCD_RAMRegister_1] |= 0xff;
        LCD->RAM[LCD_RAMRegister_2] |= 0xff;
        LCD->RAM[LCD_RAMRegister_3] |= 0x0f;
        LCD->RAM[LCD_RAMRegister_14] |= 0xff;
        LCD->RAM[LCD_RAMRegister_15] |= 0x0f;

        break;
    case COM_5:
        LCD_PageSelect(LCD_PageSelection_SecondPage);
        LCD->RAM[LCD_RAMRegister_3] |= 0xf0;
        LCD->RAM[LCD_RAMRegister_4] |= 0xff;
        LCD->RAM[LCD_RAMRegister_5] |= 0xff;
        LCD->RAM[LCD_RAMRegister_6] |= 0xff;
        LCD->RAM[LCD_RAMRegister_16] |= 0xff;
        LCD->RAM[LCD_RAMRegister_17] |= 0x0f;
        break;
    case COM_6:
        LCD_PageSelect(LCD_PageSelection_SecondPage);
        LCD->RAM[LCD_RAMRegister_7] |= 0xff;
        LCD->RAM[LCD_RAMRegister_8] |= 0xff;
        LCD->RAM[LCD_RAMRegister_9] |= 0xff;
        LCD->RAM[LCD_RAMRegister_10] |= 0x0f;
        LCD->RAM[LCD_RAMRegister_18] |= 0xff;
        LCD->RAM[LCD_RAMRegister_19] |= 0x0f;
        break;
    case COM_7:
        LCD_PageSelect(LCD_PageSelection_SecondPage);
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
        LCD_PageSelect(LCD_PageSelection_FirstPage);
        LCD->RAM[LCD_RAMRegister_0] &= 0x00;
        LCD->RAM[LCD_RAMRegister_1] &= 0x00;
        LCD->RAM[LCD_RAMRegister_2] &= 0x00;
        LCD->RAM[LCD_RAMRegister_3] &= 0xf0;
        LCD->RAM[LCD_RAMRegister_14] &= 0x00;
        LCD->RAM[LCD_RAMRegister_15] &= 0xf0;

        break;
    case COM_1:
        LCD_PageSelect(LCD_PageSelection_FirstPage);
        LCD->RAM[LCD_RAMRegister_3] &= 0x0f;
        LCD->RAM[LCD_RAMRegister_4] &= 0x00;
        LCD->RAM[LCD_RAMRegister_5] &= 0x00;
        LCD->RAM[LCD_RAMRegister_6] &= 0x00;
        LCD->RAM[LCD_RAMRegister_16] &= 0x00;
        LCD->RAM[LCD_RAMRegister_17] &= 0xf0;
        break;
    case COM_2:
        LCD_PageSelect(LCD_PageSelection_FirstPage);
        LCD->RAM[LCD_RAMRegister_7] &= 0x00;
        LCD->RAM[LCD_RAMRegister_8] &= 0x00;
        LCD->RAM[LCD_RAMRegister_9] &= 0x00;
        LCD->RAM[LCD_RAMRegister_10] &= 0xf0;
        LCD->RAM[LCD_RAMRegister_18] &= 0x00;
        LCD->RAM[LCD_RAMRegister_19] &= 0xf0;
        break;
    case COM_3:
        LCD_PageSelect(LCD_PageSelection_FirstPage);
        LCD->RAM[LCD_RAMRegister_10] &= 0x0f;
        LCD->RAM[LCD_RAMRegister_11] &= 0x00;
        LCD->RAM[LCD_RAMRegister_12] &= 0x00;
        LCD->RAM[LCD_RAMRegister_13] &= 0x00;
        LCD->RAM[LCD_RAMRegister_20] &= 0x00;
        LCD->RAM[LCD_RAMRegister_21] &= 0xf0;
        break;
    case COM_4:
        LCD_PageSelect(LCD_PageSelection_SecondPage);
        LCD->RAM[LCD_RAMRegister_0] &= 0x00;
        LCD->RAM[LCD_RAMRegister_1] &= 0x00;
        LCD->RAM[LCD_RAMRegister_2] &= 0x00;
        LCD->RAM[LCD_RAMRegister_3] &= 0xf0;
        LCD->RAM[LCD_RAMRegister_14] &= 0x00;
        LCD->RAM[LCD_RAMRegister_15] &= 0xf0;

        break;
    case COM_5:
        LCD_PageSelect(LCD_PageSelection_SecondPage);
        LCD->RAM[LCD_RAMRegister_3] &= 0x0f;
        LCD->RAM[LCD_RAMRegister_4] &= 0x00;
        LCD->RAM[LCD_RAMRegister_5] &= 0x00;
        LCD->RAM[LCD_RAMRegister_6] &= 0x00;
        LCD->RAM[LCD_RAMRegister_16] &= 0x00;
        LCD->RAM[LCD_RAMRegister_17] &= 0xf0;
        break;
    case COM_6:
        LCD_PageSelect(LCD_PageSelection_SecondPage);
        LCD->RAM[LCD_RAMRegister_7] &= 0x00;
        LCD->RAM[LCD_RAMRegister_8] &= 0x00;
        LCD->RAM[LCD_RAMRegister_9] &= 0x00;
        LCD->RAM[LCD_RAMRegister_10] &= 0xf0;
        LCD->RAM[LCD_RAMRegister_18] &= 0x00;
        LCD->RAM[LCD_RAMRegister_19] &= 0xf0;
        break;
    case COM_7:
        LCD_PageSelect(LCD_PageSelection_SecondPage);
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
/**
 ****************************************************************************
 * @Function : void LCD_Eland_Num_Set(LCD_Digital_Serial_t Serial, u8 data)
 * @File     : lcd_eland.c
 * @Program  : Serial:witch num to set
 *             data:what to set
 * @Created  : 2017/10/30 by seblee
 * @Brief    : set number
 * @Version  : V1.0
**/
void LCD_Eland_Num_Set(LCD_Digital_Serial_t Serial, u8 data)
{
    __Digital_Coding_t Cache;
    if (Serial < Serial_08) //Serial_01 ~ 07
    {
        Cache = LCD_Eland_Digital_Convert(POSITIVE, data);
        LCD_PageSelect(LCD_PageSelection_FirstPage);

        LCD->RAM[Serial / 2] &= ((Serial % 2) ? 0x7f : 0xf7);
        if (Cache.Digital_B != 0)
            LCD->RAM[Serial / 2] |= ((Serial % 2) ? 0x80 : 0x08);

        LCD->RAM[(Serial + 7) / 2] &= ((Serial + 7) % 2) ? 0x0f : 0xf0;
        LCD->RAM[(Serial + 7) / 2] |= (Cache.Digital_CALK << (((Serial + 7) % 2) ? 4 : 0));

        LCD->RAM[(Serial + 14) / 2] &= (Serial % 2) ? 0x0f : 0xf0;
        LCD->RAM[(Serial + 14) / 2] |= (Cache.Digital_EDMJ << ((Serial % 2) ? 4 : 0));

        LCD->RAM[(Serial + 21) / 2] &= ((Serial + 21) % 2) ? 0x0f : 0xf0;
        LCD->RAM[(Serial + 21) / 2] |= (Cache.Digital_FGHI << (((Serial + 21) % 2) ? 4 : 0));
    }
    else if (Serial < Serial_11) //Serial_08 ~ 10
    {
        Cache = LCD_Eland_Digital_Convert(POSITIVE, data);
        LCD_PageSelect(LCD_PageSelection_FirstPage);

        LCD->RAM[(Serial + 21) / 2] &= (((Serial + 21) % 2) ? 0x7f : 0xf7);
        if (Cache.Digital_B != 0)
            LCD->RAM[(Serial + 21) / 2] |= (((Serial + 21) % 2) ? 0x80 : 0x08);

        LCD->RAM[(Serial + 25) / 2] &= (Serial % 2) ? 0xf0 : 0x0f;
        LCD->RAM[(Serial + 25) / 2] |= (Cache.Digital_CALK << ((Serial % 2) ? 0 : 4));

        LCD->RAM[(Serial + 29) / 2] &= (Serial % 2) ? 0xf0 : 0x0f;
        LCD->RAM[(Serial + 29) / 2] |= (Cache.Digital_EDMJ << ((Serial % 2) ? 0 : 4));

        LCD->RAM[(Serial + 33) / 2] &= (Serial % 2) ? 0xf0 : 0x0f;
        LCD->RAM[(Serial + 33) / 2] |= (Cache.Digital_FGHI << ((Serial % 2) ? 0 : 4));
    }
    else if (Serial < Serial_17) //Serial_11 ~ 16
    {
        Cache = LCD_Eland_Digital_Convert(NEGATIVE, data);
        LCD_PageSelect(LCD_PageSelection_SecondPage);

        if (Serial == Serial_11)
        {
            LCD->RAM[12] &= 0XFB;
            if (Cache.Digital_B != 0)
                LCD->RAM[12] |= 0x04;
        }
        else
        {
            LCD->RAM[(36 - Serial) / 2] &= ((Serial % 2) ? 0xef : 0xfe);
            if (Cache.Digital_B != 0)
                LCD->RAM[(36 - Serial) / 2] |= ((Serial % 2) ? 0x10 : 0x01);
        }
        LCD->RAM[(29 - Serial) / 2] &= (Serial % 2) ? 0xf0 : 0x0f;
        LCD->RAM[(29 - Serial) / 2] |= (Cache.Digital_CALK << ((Serial % 2) ? 0 : 4));

        LCD->RAM[(22 - Serial) / 2] &= (Serial % 2) ? 0x0f : 0xf0;
        LCD->RAM[(22 - Serial) / 2] |= (Cache.Digital_EDMJ << ((Serial % 2) ? 4 : 0));

        LCD->RAM[(15 - Serial) / 2] &= (Serial % 2) ? 0xf0 : 0x0f;
        LCD->RAM[(15 - Serial) / 2] |= (Cache.Digital_FGHI << ((Serial % 2) ? 0 : 4));
    }
    else if (Serial < Serial_20) //Serial_17 ~ 19
    {
        Cache = LCD_Eland_Digital_Convert(NEGATIVE, data);
        LCD_PageSelect(LCD_PageSelection_SecondPage);

        LCD->RAM[(58 - Serial) / 2] &= ((Serial % 2) ? 0xef : 0xfe);
        if (Cache.Digital_B != 0)
            LCD->RAM[(58 - Serial) / 2] |= ((Serial % 2) ? 0x10 : 0x01);

        LCD->RAM[(54 - Serial) / 2] &= (Serial % 2) ? 0x0f : 0xf0;
        LCD->RAM[(54 - Serial) / 2] |= (Cache.Digital_CALK << ((Serial % 2) ? 4 : 0));

        LCD->RAM[(50 - Serial) / 2] &= (Serial % 2) ? 0x0f : 0xf0;
        LCD->RAM[(50 - Serial) / 2] |= (Cache.Digital_EDMJ << ((Serial % 2) ? 4 : 0));

        LCD->RAM[(46 - Serial) / 2] &= (Serial % 2) ? 0x0f : 0xf0;
        LCD->RAM[(46 - Serial) / 2] |= (Cache.Digital_FGHI << ((Serial % 2) ? 4 : 0));
    }
    else if (Serial == Serial_20) //Serial_20
    {
        Cache = LCD_Eland_Digital_Convert(NEGATIVE, data);
        LCD_PageSelect(LCD_PageSelection_SecondPage);

        LCD->RAM[13] &= 0xef;
        if (Cache.Digital_B != 0)
            LCD->RAM[13] |= 0x10;

        LCD->RAM[10] &= 0xf0;
        LCD->RAM[10] |= Cache.Digital_CALK;

        LCD->RAM[6] &= 0x0f;
        LCD->RAM[6] |= (Cache.Digital_EDMJ << 4);

        LCD->RAM[3] &= 0xf0;
        LCD->RAM[3] |= Cache.Digital_FGHI;
    }
}

/**
 ****************************************************************************
 * @Function : static __Digital_Coding_t LCD_Eland_Digital_Convert(LCD_Coding_Dirtction_t direction, uint8_t Data)
 * @File     : lcd_eland.c
 * @Program  : Data:
 * @Created  : 2017/10/30 by seblee
 * @Brief    : Data to Digital
 * @Version  : V1.0
**/
static __Digital_Coding_t LCD_Eland_Digital_Convert(LCD_Coding_Dirtction_t direction, uint8_t Data)
{
    __Digital_Coding_t Cache;
    uint8_t i, Num_Cache;
    if (direction == POSITIVE)
        Cache.WORD = NumberMap[Data];
    else if (direction == NEGATIVE)
    {
        Num_Cache |= (NumberMap[Data] & 0xf000); //Digital_B
        for (i = 0; i < 4; i++)
        {
            if (NumberMap[Data] & (0x0001 << i)) //Digital_CALK
                Num_Cache |= (0x0008 >> i);
            if (NumberMap[Data] & (0x0010 << i)) //Digital_EDMJ
                Num_Cache |= (0x0080 >> i);
            if (NumberMap[Data] & (0x0100 << i)) //Digital_FGHI
                Num_Cache |= (0x0800 >> i);
        }
        Cache.WORD = Num_Cache;
    }
    return Cache;
}
/**
 ****************************************************************************
 * @Function : void LCD_Eland_Week_Set(u8 type,u8 day)
 * @File     : lcd_eland.c
 * @Program  : type: TIME_PART/ALARM_PART
 *             day : The day of the week
 * @Created  : 2017/11/4 by seblee
 * @Brief    : set week day
 * @Version  : V1.0
**/
void LCD_Eland_Week_Set(LCD_Time_Type_t type, LCD_Week_Day_t day)
{
    uint8_t i;
    if (type == TIME_PART)
    {
        LCD_PageSelect(LCD_PageSelection_FirstPage);
        for (i = 0; i < 3; i++)
            LCD->RAM[i] &= 0xbf;
        LCD->RAM[3] &= 0xfb;
        LCD->RAM[14] &= 0xbb;
        LCD->RAM[15] &= 0xfb;
        if (day < WEDNESDAY)
            LCD->RAM[day] |= 0x40;
        else if (day < THURSDAY)
            LCD->RAM[3] |= 0x04;
        else if (day <= SATURDAY)
            LCD->RAM[day / 2 + 12] |= (0x04 << ((day % 2) ? 4 : 0));
    }
    else if (type == ALARM_PART)
    {
        LCD_PageSelect(LCD_PageSelection_SecondPage);
        LCD->RAM[10] &= 0xdf;
        LCD->RAM[11] &= 0x99;
        LCD->RAM[20] &= 0x7f;
        LCD->RAM[21] &= 0xfd;
        switch (day)
        {
        case SUNDAY:
            LCD->RAM[11] |= 0X40;
            break;
        case MONDAY:
            LCD->RAM[11] |= 0X20;
            break;
        case TUESDAY:
            LCD->RAM[11] |= 0X04;
            break;
        case WEDNESDAY:
            LCD->RAM[11] |= 0X02;
            break;
        case THURSDAY:
            LCD->RAM[10] |= 0X20;
            break;
        case FRIDAY:
            LCD->RAM[21] |= 0X02;
            break;
        case SATURDAY:
            LCD->RAM[20] |= 0X80;
            break;
        default:
            break;
        }
    }
}
/**
 ****************************************************************************
 * @Function : void LCD_Eland_Wifi_RSSI_Set(LCD_Wifi_Rssi_t level)
 * @File     : lcd_eland.c
 * @Program  : level:Rssi level
 * @Created  : 2017/11/4 by seblee
 * @Brief    : set wifi play
 * @Version  : V1.0
**/
void LCD_Eland_Wifi_RSSI_Set(LCD_Wifi_Rssi_t level)
{
    LCD_PageSelect(LCD_PageSelection_SecondPage);
    LCD->RAM[12] &= 0x1f;
    LCD->RAM[13] &= 0xfe;

    switch (level)
    {
    case LEVEL0:
        break; // 0x0000,
    case LEVEL1:
        LCD->RAM[13] |= 0x01;
        break;
    case LEVEL2:
        LCD->RAM[12] |= 0x80;
        LCD->RAM[13] |= 0x01;
        break;
    case LEVEL3:
        LCD->RAM[12] |= 0xd0;
        LCD->RAM[13] |= 0x01;
        break;
    case LEVEL4:
        LCD->RAM[12] |= 0xe0;
        LCD->RAM[13] |= 0x01;
        break;
    default:
        break;
    }
}
/**
 ****************************************************************************
 * @Function : void LCD_Eland_Time_Display(_eland_date_time_t time)
 * @File     : lcd_eland.c
 * @Program  : time:to set
 * @Created  : 2017/11/4 by seblee
 * @Brief    : display time
 * @Version  : V1.0
**/
void LCD_Eland_Time_Display(_eland_date_time_t time)
{
    static _eland_date_time_t time_cache;
    LCD_Week_Day_t week_temp;
    if (time_cache.year != time.year)
    {
        LCD_Eland_Num_Set(Serial_01, ((time.year / 10) % 10)); //year
        LCD_Eland_Num_Set(Serial_02, (time.year % 10));        //year
    }
    if (time_cache.month != time.month)
    {
        LCD_Eland_Num_Set(Serial_03, ((time.month / 10) % 10)); //month
        LCD_Eland_Num_Set(Serial_04, (time.month % 10));        //month
    }
    if (time_cache.day != time.day)
    {
        LCD_Eland_Num_Set(Serial_05, ((time.day / 10) % 10)); //day
        LCD_Eland_Num_Set(Serial_06, (time.day % 10));        //day
    }
    if (time_cache.hour != time.hour)
    {
        LCD_Eland_Num_Set(Serial_07, ((time.hour / 10) % 10)); //hour
        LCD_Eland_Num_Set(Serial_08, (time.hour % 10));        //hour
    }
    if (time_cache.minute != time.minute)
    {
        LCD_Eland_Num_Set(Serial_09, ((time.minute / 10) % 10)); //minute
        LCD_Eland_Num_Set(Serial_10, (time.minute % 10));        //minute
    }
    if (time_cache.second != time.second)
    {
        LCD_Eland_Num_Set(Serial_11, ((time.second / 10) % 10)); //second
        LCD_Eland_Num_Set(Serial_12, (time.second % 10));        //second
    }
    if (time_cache.week != time.week)
    {
        if (time.week < RTC_Weekday_Sunday)
            week_temp = (LCD_Week_Day_t)time.week;
        else if (time.week == RTC_Weekday_Sunday)
            week_temp = SUNDAY;
        LCD_Eland_Week_Set(TIME_PART, week_temp);
    }
    memcpy(&time_cache, &time, sizeof(_eland_date_time_t));
}
/**
 ****************************************************************************
 * @Function : void LCD_Eland_Time_Upgrade(_eland_date_time_t time)
 * @File     : lcd_eland.c
 * @Program  : time:real RTC time
 * @Created  : 2017/11/8 by seblee
 * @Brief    : upgrade time toggled point per second
 * @Version  : V1.0
**/
void LCD_Eland_Time_Upgrade(_eland_date_time_t time)
{
    static _eland_date_time_t time_bak;
    if (time_bak.minute != time.minute)
    {
        time_bak = time;
        LCD_Eland_Time_Display(time);
    }
    //toggle point
    LCD_PageSelect(LCD_PageSelection_FirstPage);
    LCD->RAM[14] ^= 0x30;
}
#ifdef testlcdIO
/**
 ****************************************************************************
 * @Function : void LCD_Eland_COM_SCAN(void)
 * @File     : lcd_eland.c
 * @Program  : NONE
 * @Created  : 2017/11/15 by seblee
 * @Brief    : SCAN COMX
 * @Version  : V1.0
**/
void LCD_Eland_COM_SCAN(void)
{
    static u8 flag = 0x01;
    if (flag == 0x08)
        flag = 0x01;
    else
        flag <<= 1;

    GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)(flag & 0x01)); //COM0
    GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction)(flag & 0x02)); //COM1
    GPIO_WriteBit(GPIOA, GPIO_Pin_6, (BitAction)(flag & 0x04)); //COM2
    GPIO_WriteBit(GPIOD, GPIO_Pin_1, (BitAction)(flag & 0x08)); //COM3
    GPIO_WriteBit(GPIOF, GPIO_Pin_4, (BitAction)(flag & 0x10)); //COM4
    GPIO_WriteBit(GPIOF, GPIO_Pin_5, (BitAction)(flag & 0x20)); //COM5
    GPIO_WriteBit(GPIOF, GPIO_Pin_6, (BitAction)(flag & 0x40)); //COM6
    GPIO_WriteBit(GPIOF, GPIO_Pin_7, (BitAction)(flag & 0x80)); //COM7
}
#endif
