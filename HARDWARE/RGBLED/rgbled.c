/**
 ****************************************************************************
 * @Warning :Without permission from the author,Not for commercial use
 * @File    :rgbled.c
 * @Author  :seblee
 * @date    :2017/10/20
 * @version :V 1.0.0
 *************************************************
 * @brief   :
 ****************************************************************************
**/
/* Private include -----------------------------------------------------------*/
#include "rgbled.h"
#include "lcd_display.h"
#include "key.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
__IO uint16_t TIM2_CCR1_Val = 0;
__IO uint16_t TIM2_CCR2_Val = 0;
__IO uint16_t TIM3_CCR1_Val = 0;
__IO uint16_t TIM3_CCR2_Val = 0;
__IO uint16_t TIM5_CCR1_Val = 0;
__IO uint16_t TIM5_CCR2_Val = 0;
/* Private function prototypes -----------------------------------------------*/
static void RGBLED_GPIO_Cfg(void);
static void TIM3_Config(void);
static void TIM2_Config(void);
//static void TIM5_Config(void);
/* Private functions ---------------------------------------------------------*/

/**
 ****************************************************************************
 * @Function : void RGBLED_CFG(void)
 * @File     : rgbled.c
 * @Program  : none
 * @Created  : 2017/10/20 by seblee
 * @Program  : PI0 TIM3---channel 1
 *             PI3 TIM3---channel 2
 *             PH7 TIM5---channel 2
 *             PH6 TIM5---channel 1
 * @Version  : V1.0
**/
void RGBLED_CFG(void)
{
    RGBLED_GPIO_Cfg();
    TIM3_Config();
    TIM2_Config();
    RGBLED_Set_Brightness(eland_data.brightness_normal); //back light turn brightest
}

/**
 ****************************************************************************
 * @Function : static void RGBLED_GPIO_Cfg(void)
 * @File     : rgbled.c
 * @Program  : none
 * @Created  : 2017/10/20 by seblee
 * @Program  : PB0 TIM2---channel 1
 *             PB2 TIM2---channel 2
 *             PB1 TIM3---channel 1
 *             PD0 TIM3---channel 2
 * @Version  : V1.0
**/
static void RGBLED_GPIO_Cfg(void)
{
    /*!< TIM3 Channel 1 (PB1) remapping to PI0 */
    //SYSCFG_REMAPPinConfig(REMAP_Pin_TIM3Channel1, ENABLE);
    /*!< TIM3 Channel 2 (PD0) remapping to PI3 */
    //SYSCFG_REMAPPinConfig(REMAP_Pin_TIM3Channel2, ENABLE);

    GPIO_Init(GPIOB, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2, GPIO_Mode_Out_PP_Low_Fast);

    GPIO_Init(GPIOD, GPIO_Pin_0, GPIO_Mode_Out_PP_Low_Fast);
}
static void TIM3_Config(void)
{
    /* Enable TIM3 clock */
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM3, ENABLE);
    /*
  - TIM1CLK = 2 MHz
  - TIM1 counter clock = TIM1CLK / TIM1_PRESCALER+1 = 2 MHz/1+1 = 1 MHz
  */
    /* Time base configuration */
    TIM3_TimeBaseInit(TIM3_Prescaler_16, TIM3_CounterMode_Up, TIM3_PERIOD);
    /*
  - The TIM1 CCR1 register value is equal to 32768:
  - CC1 update rate = TIM1 counter clock / TIM3_PERIOD = 30.51 Hz,
  - So the TIM1 Channel 1 generates a periodic signal with a frequency equal to 15.25 Hz.
  */
    /* Toggle Mode configuration: Channel1 */
    TIM3_OC1Init(TIM3_OCMode_PWM1, TIM3_OutputState_Enable, TIM3_CCR1_Val, TIM3_OCPolarity_High, TIM3_OCIdleState_Reset);
    TIM3_OC1PreloadConfig(ENABLE); //输出比较1通道预加载使能
    /*
  - The TIM1 CCR2 register is equal to 16384:
  - CC2 update rate = TIM1 counter clock / CCR2_Val = 61.03 Hz
  - So the TIM1 channel 2 generates a periodic signal with a frequency equal to 30.51 Hz.
  */
    /* Toggle Mode configuration: Channel2 */
    TIM3_OC2Init(TIM3_OCMode_PWM1, TIM3_OutputState_Enable, TIM3_CCR2_Val, TIM3_OCPolarity_High, TIM3_OCIdleState_Reset);
    TIM3_OC2PreloadConfig(ENABLE); //输出比较1通道预加载使能

    TIM3_CtrlPWMOutputs(ENABLE); //PWM输出使能
    TIM3_Cmd(ENABLE);            //使能
}
static void TIM2_Config(void)
{
    /* Enable TIM5 clock */
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);
    /*
  - TIM1CLK = 2 MHz
  - TIM1 counter clock = TIM1CLK / TIM1_PRESCALER+1 = 2 MHz/1+1 = 1 MHz
  */
    /* Time base configuration */
    TIM2_TimeBaseInit(TIM2_Prescaler_16, TIM2_CounterMode_Up, TIM2_PERIOD);
    /*
  - The TIM1 CCR1 register value is equal to 32768:
  - CC1 update rate = TIM1 counter clock / CCR1_Val = 30.51 Hz,
  - So the TIM1 Channel 1 generates a periodic signal with a frequency equal to 15.25 Hz.
  */
    /* Toggle Mode configuration: Channel1 */
    TIM2_OC1Init(TIM2_OCMode_PWM1, TIM2_OutputState_Enable, TIM2_CCR1_Val, TIM2_OCPolarity_High, TIM2_OCIdleState_Reset);
    TIM2_OC1PreloadConfig(ENABLE); //输出比较1通道预加载使能 TIM2_OCIdleState_Set
    /*
  - The TIM1 CCR2 register is equal to 16384:
  - CC2 update rate = TIM1 counter clock / CCR2_Val = 61.03 Hz
  - So the TIM1 channel 2 generates a periodic signal with a frequency equal to 30.51 Hz.
  */
    /* Toggle Mode configuration: Channel2 */
    TIM2_OC2Init(TIM2_OCMode_PWM1, TIM2_OutputState_Enable, TIM2_CCR2_Val, TIM2_OCPolarity_High, TIM2_OCIdleState_Reset);
    TIM2_OC2PreloadConfig(ENABLE); //输出比较2通道预加载使能

    TIM2_CtrlPWMOutputs(ENABLE); //PWM输出使能
    TIM2_Cmd(ENABLE);            //使能
}
void TIM5_Config(void)
{
    /* Enable TIM5 clock */
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM5, ENABLE);
    /*
  - TIM1CLK = 2 MHz
  - TIM1 counter clock = TIM1CLK / TIM1_PRESCALER+1 = 2 MHz/1+1 = 1 MHz
  */
    /* Time base configuration */
    TIM5_TimeBaseInit(TIM5_Prescaler_16, TIM5_CounterMode_Up, TIM5_PERIOD);
    /*
  - The TIM1 CCR1 register value is equal to 32768:
  - CC1 update rate = TIM1 counter clock / CCR1_Val = 30.51 Hz,
  - So the TIM1 Channel 1 generates a periodic signal with a frequency equal to 15.25 Hz.
  */
    /* Toggle Mode configuration: Channel1 */
    TIM5_OC1Init(TIM5_OCMode_PWM1, TIM5_OutputState_Enable, TIM5_CCR1_Val, TIM5_OCPolarity_High, TIM5_OCIdleState_Reset);
    TIM5_OC1PreloadConfig(ENABLE); //输出比较1通道预加载使能 TIM5_OCIdleState_Set
    /*
  - The TIM1 CCR2 register is equal to 16384:
  - CC2 update rate = TIM1 counter clock / CCR2_Val = 61.03 Hz
  - So the TIM1 channel 2 generates a periodic signal with a frequency equal to 30.51 Hz.
  */
    /* Toggle Mode configuration: Channel2 */
    TIM5_OC2Init(TIM5_OCMode_PWM1, TIM5_OutputState_Enable, TIM5_CCR2_Val, TIM5_OCPolarity_High, TIM5_OCIdleState_Reset);
    TIM5_OC2PreloadConfig(ENABLE); //输出比较2通道预加载使能

    TIM5_CtrlPWMOutputs(ENABLE); //PWM输出使能
    TIM5_Cmd(ENABLE);            //使能
}
/**
 ****************************************************************************
 * @Function : void RGBLED_Input_RGB(u8 Red,u8 Green,u8 Blue)
 * @File     : rgbled.c
 * @Program  : Red:R
 *             Green:G
 *             Blue:B
 * @Created  : 2017/10/27 by seblee
 * @Brief    : write rgb parament
 * @Version  : V1.0
**/
void RGBLED_Input_RGB(u8 Red, u8 Green, u8 Blue)
{
    TIM2_SetCompare1(Red);
    TIM2_SetCompare2(Blue);
    TIM3_SetCompare1(Green);
}
/**
 ****************************************************************************
 * @Function : void RGBLED_Color_Set(__eland_color_t color, uint8_t brightness)
 * @File     : rgbled.c
 * @Program  : color:color to set
 *             brightness: led bright
 * @Created  : 2017/10/27 by seblee
 * @Brief    : set led color
 * @Version  : V1.0
**/
void RGBLED_Color_Set(__eland_color_t color, uint8_t brightness)
{
    uint8_t bright = 0;
    if (brightness > 100)
        bright = 100;
    else
        bright = brightness;
    switch (color)
    {
    case ELAND_BLACK:
        RGBLED_Input_RGB(0, 0, 0);
        break;
    case ELAND_BLUE:
        RGBLED_Input_RGB(0, 0, 255 * bright / 100);
        break;
    case ELAND_WHITE_BLUE:
        RGBLED_Input_RGB(25 * bright / 100, 25 * bright / 100, 255 * bright / 100);
        break;
    case ELAND_PURPLE:
        RGBLED_Input_RGB(128 * bright / 100, 0, 128 * bright / 100);
        break;
    case ELAND_RED:
        RGBLED_Input_RGB(255 * bright / 100, 0, 0);
        break;
    case ELAND_PINK:
        RGBLED_Input_RGB(255 * bright / 100, 100 * bright / 100, 110 * bright / 100);
        break;
    case ELAND_ORANGE:
        RGBLED_Input_RGB(255 * bright / 100, 165 * bright / 100, 0);
        break;
    case ELAND_YELLOW:
        RGBLED_Input_RGB(255 * bright / 100, 255 * bright / 100, 0);
        break;
    case ELAND_YELLOW_GREEN:
        RGBLED_Input_RGB(173 * bright / 100, 255 * bright / 100, 47 * bright / 100);
        break;
    case ELAND_GREEN:
        RGBLED_Input_RGB(0, 128 * bright / 100, 0);
        break;
    case ELAND_WHITE:
        RGBLED_Input_RGB(255 * bright / 100, 255 * bright / 100, 255 * bright / 100);
        break;
    default:
        RGBLED_Input_RGB(255 * bright / 100, 255 * bright / 100, 255 * bright / 100);
        break;
    }
}
/**
 ****************************************************************************
 * @Function : void RGBLED_Rainbow_Set(__rainbow_t color)
 * @File     : rgbled.c
 * @Program  : color:witch to set
 * @Created  : 2018/1/13 by seblee
 * @Brief    : set color
 * @Version  : V1.0
**/
void RGBLED_Rainbow_Set(__rainbow_t color)
{
    switch (color)
    {
    case RAINBOW_RED: /**赤#ff0000**/
        RGBLED_Input_RGB(0xff, 0x00, 0x00);
        break;
    case RAINBOW_ORANGE: /**橙#ff9900**/
        RGBLED_Input_RGB(0xff, 0x99, 0x00);
        break;
    case RAINBOW_YELLOW: /**黃#ffcc00**/
        RGBLED_Input_RGB(0xff, 0xcc, 0x00);
        break;
    case RAINBOW_WARBLER: /**鶯#66cc00**/
        RGBLED_Input_RGB(0x66, 0xcc, 0x00);
        break;
    case RAINBOW_GREEN: /**綠*#009933**/
        RGBLED_Input_RGB(0x00, 0x99, 0x33);
        break;
    case RAINBOW_WATER: /**水#00ccff**/
        RGBLED_Input_RGB(0x00, 0xcc, 0xff);
        break;
    case RAINBOW_WATERBLUE: /**青#0066ff**/
        RGBLED_Input_RGB(0x00, 0x66, 0xff);
        break;
    case RAINBOW_PURPLE: /**紫#9900ff**/
        RGBLED_Input_RGB(0x99, 0x00, 0xff);
        break;
    case RAINBOW_PEACH: /**桃#ff00cc**/
        RGBLED_Input_RGB(0xff, 0x00, 0xcc);
        break;
    case RAINBOW_GRAY: /**灰#333333**/
        RGBLED_Input_RGB(0x33, 0x33, 0x33);
        break;
    default: /****/
        RGBLED_Input_RGB(0x00, 0x00, 0x00);
        break;
    }
}

/**
 ****************************************************************************
 * @Function : void RGBLED_RGBCode_Set(u32 ColorCode)
 * @File     : rgbled.c
 * @Program  : ColorCode:color code eg.#CC00FF
 * @Created  : 2017/10/27 by seblee
 * @Brief    : set color by color code
 * @Version  : V1.0
**/
void RGBLED_RGBCode_Set(u32 ColorCode)
{
    u8 R_Cache, G_Cache, B_Cache;
    R_Cache = (u8)(ColorCode & 0xff);
    G_Cache = (u8)((ColorCode >> 8) & 0xff);
    B_Cache = (u8)((ColorCode >> 16) & 0xff);
    RGBLED_Input_RGB(R_Cache, G_Cache, B_Cache);
}
/**
 ****************************************************************************
 * @Function : void RGBLED_Set_Brightness(uint8_t Brightness)
 * @File     : rgbled.c
 * @Program  : Brightness:value of brightness
 * @Created  : 2018/1/11 by seblee
 * @Brief    : set background bright
 * @Version  : V1.0
**/
void RGBLED_Set_Brightness(uint8_t Brightness)
{
    static uint8_t BrightnessBak = 0;
    if (Brightness != BrightnessBak)
    {
        if (Brightness > 100)
            Brightness = 100;
        TIM3_SetCompare2(TIM3_PERIOD / 100 * Brightness); //back light turn brightest
        BrightnessBak = Brightness;
    }
}
/**
 ****************************************************************************
 * @Function : void RGBLED_Set_Brightness(void)
 * @File     : rgbled.c
 * @Program  : Brightness:value of brightness
 * @Created  : 2018/1/11 by seblee
 * @Brief    : set background bright
 * @Version  : V1.0
**/
void RGBLED_SwitchRainBow_Color(void)
{
    static __rainbow_t color = RAINBOW_RED;
    if (Key_Down_Trg & KEY_Add)
    {
        if (color >= RAINBOW_GRAY)
            color = RAINBOW_RED;
        else
            color++;
        RGBLED_Rainbow_Set(color);
        HT162x_LCD_Num_Set(Serial_11, (((color + 1) / 10) % 10));
        HT162x_LCD_Num_Set(Serial_12, ((color + 1) % 10));
    }
    else if (Key_Down_Trg & KEY_Minus)
    {
        if (color <= RAINBOW_RED)
            color = RAINBOW_GRAY;
        else
            color--;
        RGBLED_Rainbow_Set(color);
        HT162x_LCD_Num_Set(Serial_11, (((color + 1) / 10) % 10));
        HT162x_LCD_Num_Set(Serial_12, ((color + 1) % 10));
    }
}
/**
 ****************************************************************************
 * @Function : void RGBLED_FlashRainBow_Color(void)
 * @File     : rgbled.c
 * @Program  : Brightness:value of brightness
 * @Created  : 2018/1/11 by seblee
 * @Brief    : set background bright
 * @Version  : V1.0
**/
void RGBLED_FlashRainBow_Color(void)
{
    static __rainbow_t color = RAINBOW_RED;
    static uint8_t count = 0;
    if (count++ > 25)
    {
        count = 0;
        if (color >= RAINBOW_GRAY)
            color = RAINBOW_RED;
        else
            color++;
        RGBLED_Rainbow_Set(color);
    }
}
