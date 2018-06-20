/**
 ****************************************************************************
 * @Warning :Without permission from the author,Not for commercial use
 * @File    :HT162x.c
 * @Author  :seblee
 * @date    :2017/12/1
 * @version :V 1.0.0
 *************************************************
 * @brief   :
 ****************************************************************************
**/
/* Private include -----------------------------------------------------------*/
#include "ht162x.h"
#include "lcd_display.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void HT162x_Write_Command(_HT162x_CMD_t cmd);
/* Private functions ---------------------------------------------------------*/
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
1111011101010111---0
1111001000000000---1
1011011101110110---2
1111011101110010---3
1111001000110011---4
1101011101110011---5
1101011101110111---6
1111001100010000---7
1111011101110111---8
1111011101110011---9
  */
const uint16_t NumberMap[11] = {
    // /* 0       1       2       3       4   */
    // 0XF757, 0XF200, 0XB776, 0XF772, 0XF233,
    // /* 5       6       7       8       9   */
    // 0XD773, 0XD777, 0XF310, 0XF777, 0XF773
    /* 0       1       2       3       4   */
    0xFEAE, 0xF400, 0xBEEC, 0xFEE4, 0xF466,
    /* 5       6       7       8       9   */
    0xDEE6, 0xDEEE, 0xF620, 0xFEEE, 0xFEE6,
    /*空*/
    0x0000};
const LCD_SEGx_TypeDef Week_seg[2][8] = {
    /* time week register*/
    /*SUN    MON    TUE    WED    THU    FRI    SAT */
    {SEG06, SEG14, SEG22, SEG26, SEG30, SEG34, SEG38, SEG47},
    /* alarm week register*/
    /*SUN    MON    TUE    WED    THU    FRI    SAT */
    {SEG10, SEG09, SEG06, SEG05, SEG01, SEG37, SEG35, SEG47},
};

const LCD_COMx_TypeDef Week_com[3] = {COM0, COM7, COMMAX};

const LCD_SEGx_TypeDef AMPM_seg[2][3] = {
    /*AM    PM */
    {SEG22, SEG23, SEG47},
    /*AM    PM */
    {SEG38, SEG39, SEG47},
};
const LCD_Digital_Serial_t Double_Digits_Position[2][10] = {
    /*AM    PM */
    {Serial_01, Serial_03, Serial_05, Serial_07, Serial_09, Serial_11, Serial_13, Serial_15, Serial_17, Serial_19},
    /*AM    PM */
    {Serial_02, Serial_04, Serial_06, Serial_08, Serial_10, Serial_12, Serial_14, Serial_16, Serial_18, Serial_20},
};
const uint8_t Position[2][5] = {
    {0, 1, 2, 3, 4}, //time part
    {5, 6, 7, 8, 9}, //alarm part
};

/**
 ****************************************************************************
 * @Function : void HT162x_init(void)
 * @File     : HT162x.c
 * @Program  : none
 * @Created  : 2017/12/1 by seblee
 * @Brief    : init HT162x IO
 * @Version  : V1.0
**/
void HT162x_init(void)
{
    GPIO_Init(HT162x_CS_PORT, HT162x_CS_PIN, GPIO_Mode_Out_PP_High_Fast);     //CS
    GPIO_Init(HT162x_RD_PORT, HT162x_RD_PIN, GPIO_Mode_Out_PP_High_Fast);     //CS
    GPIO_Init(HT162x_WR_PORT, HT162x_WR_PIN, GPIO_Mode_Out_PP_High_Fast);     //CS
    GPIO_Init(HT162x_DATA_PORT, HT162x_DATA_PIN, GPIO_Mode_Out_PP_High_Fast); //CS

    //ALL OUT PUT 1
    HT162x_CS_SET;   //CS
    HT162x_RD_SET;   //RD
    HT162x_WR_SET;   //WR
    HT162x_DATA_SET; //DATA

    HT162x_Write_Command(SYS_EN); //Turn on system oscillator
    HT162x_Write_Command(LCD_ON); //Turn on LCD display
    HT162x_Write_Command(RC_32K); //System clock source,on - chip RC oscillator
    HT162x_Write_Command(F128);   //Time base clock output : 128Hz
    HT162x_Write_Command(NORMAL); //Normal mod

    if (rst_flag != RST_FLAG_IWDGF)
        HT162x_LCD_Clear(RESET);
}

/**
 ****************************************************************************
 * @Function : static void HT162x_Write_Bit(uint8_t data,uint8_t count)
 * @File     : HT162x.c
 * @Program  : data:to write in MSB mode
 *             count:how many bits to write
 * @Created  : 2017/12/1 by seblee
 * @Brief    : write bits
 * @Version  : V1.0
**/
static void HT162x_Write_Bit(uint8_t data, uint8_t count, _SEQUENT_t sequent)
{
    uint8_t i, Temp = ((sequent == MSB) ? 0x80 : 0X01);
    for (i = 0; i < count; i++)
    {
        HT162x_WR_RESET; //“写”时钟置0
        if (data & Temp)
            HT162x_DATA_SET; //数据线置1
        else
            HT162x_DATA_RESET; //数据线置0
        nop();
        HT162x_WR_SET; //“写”时钟置1
        if (sequent == MSB)
            Temp >>= 1;
        else if (sequent == LSB)
            Temp <<= 1; //准备下一位
        nop();
    }
}
/**
 ****************************************************************************
 * @Function : static void HT162x_Write_Command(_HT162x_CMD_t cmd)
 * @File     : HT162x.c
 * @Program  : cmd:the command to write
 * @Created  : 2017/12/1 by seblee
 * @Brief    : write command to ht162X
 * @Version  : V1.0
**/
static void HT162x_Write_Command(_HT162x_CMD_t cmd)
{
    HT162x_CS_RESET;
    /*change data pin configration*/
    HT162x_DATA_SET_OUT;
    /*wrtie cmd id for write*/
    HT162x_Write_Bit(CMD_ID_COMMAND, LEN_CMD_ID, MSB);
    /*wrtie DATA for write*/
    HT162x_Write_Bit(cmd, LEN_COMMMAND, MSB);
    HT162x_CS_SET;
}
/**
 ****************************************************************************
 * @Function : static void HT162x_Write_Data(uint8_t addrass,uint8_t data)
 * @File     : HT162x.c
 * @Program  : address:the address of RAM to write
 *             data:data(4 bit) for write
 * @Created  : 2017/12/1 by seblee
 * @Brief    : write one data
 * @Version  : V1.0
**/
void HT162x_Write_Data(uint8_t addrass, uint8_t data)
{
    HT162x_CS_RESET;
    /*change data pin configration*/
    HT162x_DATA_SET_OUT;
    /* Move the actual address to high*/
    addrass <<= 1;
    /*wrtie cmd id for write*/
    HT162x_Write_Bit(CMD_ID_WRITE, LEN_CMD_ID, MSB);
    /*wrtie address for write*/
    HT162x_Write_Bit(addrass, LEN_ADDRESS, MSB);
    /*wrtie DATA for write*/
    HT162x_Write_Bit(data, LEN_DATA, LSB);
    HT162x_CS_SET;
}
/**
 ****************************************************************************
 * @Function : static void TH162x_Write_Data_Write_Continuously(uint8_t addrass, uint8_t *data,uint8_t length)
 * @File     : HT162x.c
 * @Program  : address:the address of RAM to write data:data point for write lenth:number of data
 * @Created  : 2017/12/1 by seblee
 * @Brief    : write data continuously
 * @Version  : V1.0
**/
static void TH162x_Write_Data_Write_Continuously(uint8_t addrass, uint8_t *data, uint8_t length)
{
    uint8_t i;
    HT162x_CS_RESET;
    /*change data pin configration*/
    HT162x_DATA_SET_OUT;
    /* Move the actual address to high*/
    addrass <<= 1;
    /*wrtie cmd id for write*/
    HT162x_Write_Bit(CMD_ID_WRITE, LEN_CMD_ID, MSB);
    /*wrtie address for write*/
    HT162x_Write_Bit(addrass, LEN_ADDRESS, MSB);
    for (i = 0; i < length; i++)
    {
        /*wrtie DATA for write*/
        HT162x_Write_Bit(*(data + i), LEN_DATA, LSB);
    }
    HT162x_CS_SET;
}
/**
 ****************************************************************************
 * @Function : uint8_t HT162x_Read_Bit(uint8_t count)
 * @File     : ht162x.c
 * @Program  : count:how many bits to read
 * @Created  : 2017/12/1 by seblee
 * @Brief    : read data in bits
 * @Version  : V1.0
**/
uint8_t HT162x_Read_Bit(uint8_t count)
{
    uint8_t data = 0, i;
    for (i = 0; i < count; i++)
    {
        HT162x_RD_RESET; //“写”时钟置0
        nop();
        data >>= 1;
        HT162x_RD_SET; //“写”时钟置1
        nop();
        nop();
        if (HT162x_DATA_IN) //数据线為1
            data |= 0x80;
    }
    return data;
}

/**
 ****************************************************************************
 * @Function : uint8_t HT162x_Read_Data(uint8_t address)
 * @File     : ht162x.c
 * @Program  : address:the address of data to read
 * @Created  : 2017/12/1 by seblee
 * @Brief    : read one data
 * @Version  : V1.0
**/
uint8_t HT162x_Read_Data(uint8_t address)
{
    uint8_t data;
    HT162x_CS_RESET;
    /*change data pin configration*/
    HT162x_DATA_SET_OUT;
    /* Move the actual address to high*/
    address <<= 1;
    /*wrtie cmd id for write*/
    HT162x_Write_Bit(CMD_ID_READ, LEN_CMD_ID, MSB);
    /*wrtie address for write*/
    HT162x_Write_Bit(address, LEN_ADDRESS, MSB);
    /*change data pin configration*/
    HT162x_DATA_SET_IN;
    /*read DATA */
    data = HT162x_Read_Bit(LEN_DATA) >> 4;
    HT162x_CS_SET;
    return data;
}
/**
 ****************************************************************************
 * @Function : static void HT162x_Read_Data_Continuously(uint8_t* data)
 * @File     : ht162x.c
 * @Program  : *data:the point of buffer save data
 * @Created  : 2017/12/1 by seblee
 * @Brief    : read data Continuously
 * @Version  : V1.0
**/
void HT162x_Read_Data_Continuously(uint8_t address, uint8_t *data, uint8_t length)
{
    uint8_t i;
    HT162x_CS_RESET;
    /*change data pin configration*/
    HT162x_DATA_SET_OUT;
    /* Move the actual address to high*/
    address <<= 1;
    /*wrtie cmd id for write*/
    HT162x_Write_Bit(CMD_ID_READ, LEN_CMD_ID, MSB);
    /*wrtie address for write*/
    HT162x_Write_Bit(address, LEN_ADDRESS, MSB);
    /*change data pin configration*/
    HT162x_DATA_SET_IN;
    for (i = 0; i < length; i++)
    {
        /*read DATA */
        *(data + i) = HT162x_Read_Bit(LEN_DATA) >> 4;
    }
    HT162x_CS_SET;
}
/**
 ****************************************************************************
 * @Function : void HT162x_LCD_Clear(FlagStatus value)
 * @File     : ht162x.c
 * @Program  : none
 * @Created  : 2017/12/1 by seblee
 * @Brief    : clear RAM
 * @Version  : V1.0
**/
void HT162x_LCD_Clear(FlagStatus value)
{
    uint8_t cache[HT1623_MEM_NUM];
    if (value)
        memset(cache, 0xff, HT1623_MEM_NUM);
    else
        memset(cache, 0, HT1623_MEM_NUM);
    TH162x_Write_Data_Write_Continuously(0, cache, HT1623_MEM_NUM);
    IWDG_ReloadCounter();
}

/**
 ****************************************************************************
 * @Function : void HT162x_LCD_Write_Pixel(LCD_COMx_TypeDef comx,LCD_SEGx_TypeDef segx, FlagStatus value)
 * @File     : ht162x.c
 * @Program  : comx:witch com of the pixel
 *             segx:witch seg of the pixel
 *             value:change to the value
 * @Created  : 2017/12/2 by seblee
 * @Brief    : write pixel
 * @Version  : V1.0
**/
void HT162x_LCD_Change_Pixel(LCD_COMx_TypeDef comx, LCD_SEGx_TypeDef segx, FlagStatus value)
{
    uint8_t address, data;
    if (comx >= COMMAX)
        return;
    address = (uint8_t)segx * 2 + ((comx < COM4) ? 0 : 1);
    HT162x_CS_RESET;
    /*change data pin configration*/
    HT162x_DATA_SET_OUT;
    /* Move the actual address to high*/
    address <<= 1;
    /*wrtie cmd id for write*/
    HT162x_Write_Bit(CMD_ID_WR_MODIFY_RD, LEN_CMD_ID, MSB);
    /*wrtie address for write*/
    HT162x_Write_Bit(address, LEN_ADDRESS, MSB);
    /*change data pin configration*/
    HT162x_DATA_SET_IN;
    /*read BITS */
    data = HT162x_Read_Bit(LEN_DATA) >> 4;
    /*set the corresponding bit*/

    if (value == RESET)
        data &= (~(1 << (comx % 4)));
    else
        data |= (1 << (comx % 4));
    /*change data pin configration*/
    HT162x_DATA_SET_OUT;
    /*Write BITS */
    HT162x_Write_Bit(data, LEN_DATA, LSB);
    HT162x_CS_SET;
}
/**
 ****************************************************************************
 * @Function : void HT162x_LCD_Toggle_Pixel(LCD_COMx_TypeDef comx, LCD_SEGx_TypeDef segx)
 * @File     : ht162x.c
 * @Program  : comx:witch com of the pixel value:change to the value
 * @Created  : 2017/12/5 by seblee
 * @Brief    : Toggle the pixel state
 * @Version  : V1.0
**/
void HT162x_LCD_Toggle_Pixel(LCD_COMx_TypeDef comx, LCD_SEGx_TypeDef segx)
{
    uint8_t address, data;
    if (comx >= COMMAX)
        return;
    address = (uint8_t)segx * 2 + ((comx < COM4) ? 0 : 1);
    HT162x_CS_RESET;
    /*change data pin configration*/
    HT162x_DATA_SET_OUT;
    /* Move the actual address to high*/
    address <<= 1;
    /*wrtie cmd id for write*/
    HT162x_Write_Bit(CMD_ID_WR_MODIFY_RD, LEN_CMD_ID, MSB);
    /*wrtie address for write*/
    HT162x_Write_Bit(address, LEN_ADDRESS, MSB);
    /*change data pin configration*/
    HT162x_DATA_SET_IN;
    /*read BITS */
    data = HT162x_Read_Bit(LEN_DATA) >> 4;
    /*set the corresponding bit*/
    data ^= (1 << (comx % 4));
    /*change data pin configration*/
    HT162x_DATA_SET_OUT;
    /*Write BITS */
    HT162x_Write_Bit(data, LEN_DATA, LSB);
    HT162x_CS_SET;
}

/**
 ****************************************************************************
 * @Function : void HT162x_LCD_Write_COMx(LCD_COMx_TypeDef comx, FlagStatus value)
 * @File     : ht162x.c
 * @Program  : comx:witch com to write
 *             value:change to the value
 * @Created  : 2017/12/2 by seblee
 * @Brief    : scan comx
 * @Version  : V1.0
**/
void HT162x_LCD_Change_COMx(LCD_COMx_TypeDef comx, FlagStatus value)
{
    uint8_t address, data, i;
    if (comx >= COMMAX)
        return;
    address = 0;
    HT162x_CS_RESET;
    /*change data pin configration*/
    HT162x_DATA_SET_OUT;
    /* Move the actual address to high*/
    address <<= 1;
    /*wrtie cmd id for write*/
    HT162x_Write_Bit(CMD_ID_WR_MODIFY_RD, LEN_CMD_ID, MSB);
    /*wrtie address for write*/
    HT162x_Write_Bit(address, LEN_ADDRESS, MSB);
    for (i = 0; i < HT1623_MEM_NUM; i++)
    {
        /*change data pin configration*/
        HT162x_DATA_SET_IN;
        /*read BITS */
        data = HT162x_Read_Bit(LEN_DATA) >> 4;
        /*set the corresponding bit*/
        if ((i % 2) == (comx > COM3))
        {
            if (value == RESET)
                data &= (~((uint8_t)1 << (comx % 4)));
            else
                data |= ((uint8_t)1 << (comx % 4));
        }
        /*change data pin configration*/
        HT162x_DATA_SET_OUT;
        /*Write BITS */
        HT162x_Write_Bit(data, LEN_DATA, LSB);
    }
    HT162x_CS_SET;
}
/**
 ****************************************************************************
 * @Function : void HT162x_LCD_Wtrtie_SEGxData(LCD_SEGx_TypeDef segx, uint8_t data)
 * @File     : ht162x.c
 * @Program  : segx:the segx to change
 *             value:change to the value
 * @Created  : 2017/12/2 by seblee
 * @Brief    : scan segx
 * @Version  : V1.0
**/
void HT162x_LCD_Wtrtie_SEGxData(LCD_SEGx_TypeDef segx, uint8_t data)
{
    uint8_t address;
    address = segx * 2;
    HT162x_CS_RESET;
    /*change data pin configration*/
    HT162x_DATA_SET_OUT;
    /* Move the actual address to high*/
    address <<= 1;
    /*wrtie cmd id for write*/
    HT162x_Write_Bit(CMD_ID_WRITE, LEN_CMD_ID, MSB);
    /*wrtie address for write*/
    HT162x_Write_Bit(address, LEN_ADDRESS, MSB);
    /*Write BITS */
    HT162x_Write_Bit(data, LEN_DATA, LSB);
    data >>= 4;
    /*Write BITS */
    HT162x_Write_Bit(data, LEN_DATA, LSB);
    HT162x_CS_SET;
}
/**
 ****************************************************************************
 * @Function : void HT162x_LCD_RAM_Change(uint8_t address, uint8_t data, uint8_t Mask)
 * @File     : ht162x.c
 * @Program  : data:the data tu set
 *             mask:the bit not change
 * @Created  : 2017/12/4 by seblee
 * @Brief    : change the ram
 * @Version  : V1.0
**/
static void HT162x_LCD_RAM_Change(uint8_t address, uint8_t data, uint8_t Mask)
{
    uint8_t Cache;
    HT162x_CS_RESET;
    /*change data pin configration*/
    HT162x_DATA_SET_OUT;
    /* Move the actual address to high*/
    address <<= 1;
    /*wrtie cmd id for write*/
    HT162x_Write_Bit(CMD_ID_WR_MODIFY_RD, LEN_CMD_ID, MSB);
    /*wrtie address for write*/
    HT162x_Write_Bit(address, LEN_ADDRESS, MSB);
    /*change data pin configration*/
    HT162x_DATA_SET_IN;
    /*read BITS */
    Cache = HT162x_Read_Bit(LEN_DATA) >> 4;
    /*clear corresponding bits*/
    Cache &= Mask;
    /*clear the irrelevant bits in the data*/
    data &= ~Mask;
    /*set the corresponding bit*/
    Cache |= data;
    /*change data pin configration*/
    HT162x_DATA_SET_OUT;
    /*Write BITS */
    HT162x_Write_Bit(Cache, LEN_DATA, LSB);
    HT162x_CS_SET;
}
/**
 ****************************************************************************
 * @Function : static __Digital_Coding_t HT162x_LCD_Digital_Convert(LCD_Coding_Dirtction_t direction, uint8_t Data)
 * @File     : ht162x.c
 * @Program  : direction:the data direction Data:witch data
 * @Created  : 2017/12/4 by seblee
 * @Brief    : get the Digital
 * @Version  : V1.0
**/
static __Digital_Coding_t HT162x_LCD_Digital_Convert(LCD_Coding_Dirtction_t direction, uint8_t Data)
{
    __Digital_Coding_t Cache;
    uint8_t i;
    if (direction == POSITIVE)
        Cache.WORD = NumberMap[Data];
    else if (direction == NEGATIVE)
    {
        Cache.WORD = 0;
        for (i = 0; i < 4; i++)
        {
            if (NumberMap[Data] & (0x0001 << i)) //Digital_IJK
                Cache.WORD |= (0x0008 >> i);
            if (NumberMap[Data] & (0x0010 << i)) //Digital_HML
                Cache.WORD |= (0x0080 >> i);
            if (NumberMap[Data] & (0x0100 << i)) //Digital_GDA
                Cache.WORD |= (0x0800 >> i);
            if (NumberMap[Data] & (0x1000 << i)) //Digital_FECB
                Cache.WORD |= (0x8000 >> i);
        }
    }
    return Cache;
}

/**
  ****************************************************************************
  * @Function : void LCD_Eland_Num_Set(LCD_Digital_Serial_t Serial, u8 data)
  * @File     : ht162x.c
  * @Program  : Serial:witch num to set
  *             data:what to set
  * @Created  : 2017/12/4 by seblee
  * @Brief    : set number
  * @Version  : V1.0
 **/
void HT162x_LCD_Num_Set(LCD_Digital_Serial_t Serial, u8 data)
{
    __Digital_Coding_t Cache;
    if ((Serial > Serial_20) || (data > 10))
        return;
    if (Serial < Serial_11) //Serial_01 ~ 10
    {
        Cache = HT162x_LCD_Digital_Convert(POSITIVE, data);
        HT162x_LCD_RAM_Change((Serial * 8), Cache.Digital_IJK, 0x01);
        HT162x_LCD_RAM_Change((Serial * 8 + 2), Cache.Digital_HML, 0x01);
        HT162x_LCD_RAM_Change((Serial * 8 + 4), Cache.Digital_GDA, 0x01);
        HT162x_LCD_RAM_Change((Serial * 8 + 6), Cache.Digital_FECB, 0x00);
    }
    else if (Serial == Serial_11)
    {
        Cache = HT162x_LCD_Digital_Convert(NEGATIVE, data);
        HT162x_LCD_RAM_Change((121 - Serial * 8), Cache.Digital_FECB, 0x08);
        HT162x_LCD_RAM_Change((121 - Serial * 8 + 2), Cache.Digital_GDA, 0x08);
        HT162x_LCD_RAM_Change((121 - Serial * 8 + 4), Cache.Digital_HML, 0x08);
        HT162x_LCD_RAM_Change((121 - Serial * 8 + 6), Cache.Digital_IJK, 0x08);

        HT162x_LCD_Change_Pixel(COM7, SEG14, (FlagStatus)(Cache.Digital_FECB & 0x08));
    }
    else if (Serial < Serial_17) //Serial_12 ~ 16
    {
        Cache = HT162x_LCD_Digital_Convert(NEGATIVE, data);
        HT162x_LCD_RAM_Change((121 - Serial * 8), Cache.Digital_FECB, 0x00);
        HT162x_LCD_RAM_Change((121 - Serial * 8 + 2), Cache.Digital_GDA, 0x08);
        HT162x_LCD_RAM_Change((121 - Serial * 8 + 4), Cache.Digital_HML, 0x08);
        HT162x_LCD_RAM_Change((121 - Serial * 8 + 6), Cache.Digital_IJK, 0x08);
    }
    else //Serial_17 ~ 20
    {
        Cache = HT162x_LCD_Digital_Convert(NEGATIVE, data);
        HT162x_LCD_RAM_Change((201 - Serial * 8), Cache.Digital_FECB, 0x00);
        HT162x_LCD_RAM_Change((201 - Serial * 8 + 2), Cache.Digital_GDA, 0x08);
        HT162x_LCD_RAM_Change((201 - Serial * 8 + 4), Cache.Digital_HML, 0x08);
        HT162x_LCD_RAM_Change((201 - Serial * 8 + 6), Cache.Digital_IJK, 0x08);
    }
}
/**
 ****************************************************************************
 * @Function : void HT162x_LCD_Double_Digits_Write(uint8_t position,uint8_t num)
 * @File     : ht162x.c
 * @Program  : position:the posoition of num
 *             num:number to write
 *             mode: 1 2
 * @Created  : 2018/3/2 by seblee
 * @Brief    :
 * @Version  : V1.0
**/
void HT162x_LCD_Double_Digits_Write(uint8_t position, uint8_t num, uint8_t mode)
{
    if (mode == 0)
    {
        HT162x_LCD_Num_Set(Double_Digits_Position[TENS][position], 10);
        HT162x_LCD_Num_Set(Double_Digits_Position[UNITS][position], 10);
        return;
    }
    if (num < 10)
        HT162x_LCD_Num_Set(Double_Digits_Position[TENS][position], (mode == 1) ? 10 : 0);
    else
        HT162x_LCD_Num_Set(Double_Digits_Position[TENS][position], ((num / 10) % 10));
    HT162x_LCD_Num_Set(Double_Digits_Position[UNITS][position], (num % 10));
}

/**
 ****************************************************************************
 * @Function : void HT162x_LCD_Week_Set(LCD_Time_Type_t type, LCD_Week_Day_t day)
 * @File     : ht162x.c
 * @Program  : type: TIME_PART/ALARM_PART day : The day of the week
 * @Created  : 2017/12/5 by seblee
 * @Brief    : set week dat
 * @Version  : V1.0
**/
void HT162x_LCD_Week_Set(LCD_Time_Type_t type, LCD_Week_Day_t day)
{
    uint8_t i;
    static LCD_Week_Day_t weekday[2] = {WEEKDAYNUM, WEEKDAYNUM};
    if (type > ALARM_PART)
        return;

    if (weekday[type] == WEEKDAYNUM)
    {
        /*clear all TIME_PART display*/
        for (i = 0; i < 7; i++)
            HT162x_LCD_Change_Pixel(Week_com[type], Week_seg[type][i], RESET);
    }
    else if (weekday[type] == WEEKDAYMAX)
    {
    }
    else if (weekday[type] != day)
    {
        HT162x_LCD_Change_Pixel(Week_com[type], Week_seg[type][weekday[type]], RESET); //clear week
    }
    if (day < WEEKDAYMAX)
        HT162x_LCD_Change_Pixel(Week_com[type], Week_seg[type][day], SET); //clear week
    weekday[type] = day;
}
/**
 ****************************************************************************
 * @Function : void HT162x_LCD_RSSI_Set(LCD_Wifi_Rssi_t value)
 * @File     : ht162x.c
 * @Program  : value:the value of RSSI
 * @Created  : 2017/12/5 by seblee
 * @Brief    : set rssi
 * @Version  : V1.0
**/
void HT162x_LCD_RSSI_Set(LCD_Wifi_Rssi_t value)
{
    static LCD_Wifi_Rssi_t value_BAK = LEVELNUM;
    if (value > LEVEL4)
        return;
    if (value_BAK != value)
    {
        HT162x_LCD_Change_Pixel(COM7, SEG20, (FlagStatus)(value & 0x08));
        HT162x_LCD_Change_Pixel(COM7, SEG19, (FlagStatus)(value & 0x04));
        HT162x_LCD_Change_Pixel(COM7, SEG18, (FlagStatus)(value & 0x02));
        HT162x_LCD_Change_Pixel(COM7, SEG17, (FlagStatus)(value & 0x01));
        value_BAK = value;
    }
}
/**
 ****************************************************************************
 * @Function : void HT162x_LCD_TCP_STATE_Set(FlagStatus state)
 * @File     : ht162x.c
 * @Program  : state:connected SET disconnected RESET
 * @Created  : 2018/1/27 by seblee
 * @Brief    : Set elsv state
 * @Version  : V1.0
**/
void HT162x_LCD_TCP_STATE_Set(FlagStatus state)
{
    static FlagStatus state_bak = RESET;

    if (state != state_bak)
    {
        HT162x_LCD_Change_Pixel(COM7, SEG21, state);
        state_bak = state;
    }
}

/**
 ****************************************************************************
 * @Function : void HT162x_LCD_AMPM_Set(LCD_Time_Type_t type,LCD_AMPM_Distinguish_t value)
 * @File     : ht162x.c
 * @Program  : type:witch is time or alarm  value:witch is AM or PM
 * @Created  : 2017/12/5 by seblee
 * @Brief    : set AM PM
 * @Version  : V1.0
**/
void HT162x_LCD_AMPM_Set(LCD_Time_Type_t type, LCD_AMPM_Distinguish_t value)
{
    static LCD_AMPM_Distinguish_t AMPM_BAK[2] = {AMPMNUM, AMPMNUM};

    if (AMPM_BAK[type] != value)
    {
        HT162x_LCD_Change_Pixel(COM7, AMPM_seg[type][0], RESET);
        HT162x_LCD_Change_Pixel(COM7, AMPM_seg[type][1], RESET);
        if (value < AMPMMAX)
            HT162x_LCD_Change_Pixel(COM7, AMPM_seg[type][value], SET);
        AMPM_BAK[type] = value;
    }
}
/**
 ****************************************************************************
 * @Function : void HT162x_LCD_Date_Display(LCD_Time_Type_t type, mico_rtc_time_t time)
 * @File     : ht162x.c
 * @Program  : time:the time to display
 * @Created  : 2017/12/5 by seblee
 * @Brief    : refresh time
 * @Version  : V1.0
**/
void HT162x_LCD_Date_Display(LCD_Time_Type_t type, mico_rtc_time_t time)
{
    static mico_rtc_time_t time_cache[2] = {
        {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
        {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
    };
    if (type >= TIME_CLEAR)
    {
        if (time_cache[type - TIME_CLEAR].year == 0xff)
            return;
        memset(&time_cache[type - TIME_CLEAR], 0xff, sizeof(mico_rtc_time_t));
        /*****year*******/
        HT162x_LCD_Double_Digits_Write(Position[type - TIME_CLEAR][DIGIT_YEAR], time.year, 0);
        /*****month******/
        HT162x_LCD_Double_Digits_Write(Position[type - TIME_CLEAR][DIGIT_MONTH], time.month, 0);
        /*****date******/
        HT162x_LCD_Double_Digits_Write(Position[type - TIME_CLEAR][DIGIT_DAY], time.date, 0);
        /*******point**********/
        if ((type - TIME_CLEAR) == TIME_PART)
        {
            HT162x_LCD_Change_Pixel(COM0, SEG08, RESET);
            HT162x_LCD_Change_Pixel(COM0, SEG16, RESET);
        }
        else if ((type - TIME_CLEAR) == ALARM_PART)
        {
            HT162x_LCD_Change_Pixel(COM7, SEG15, RESET);
            HT162x_LCD_Change_Pixel(COM7, SEG07, RESET);
        }
        HT162x_LCD_Week_Set((LCD_Time_Type_t)(type - TIME_CLEAR), WEEKDAYMAX); //week
        return;
    }
    if ((time_cache[type].year != time.year) ||
        (time_cache[type].month != time.month) ||
        (time_cache[type].date != time.date) ||
        (time_cache[type].weekday != time.weekday))
    {
        /*****year*******/
        HT162x_LCD_Double_Digits_Write(Position[type][DIGIT_YEAR], time.year, 2);
        /*****month******/
        HT162x_LCD_Double_Digits_Write(Position[type][DIGIT_MONTH], time.month, 1);
        /*****date******/
        HT162x_LCD_Double_Digits_Write(Position[type][DIGIT_DAY], time.date, 1);

        if (type == TIME_PART)
        {
            HT162x_LCD_Change_Pixel(COM0, SEG08, SET);
            HT162x_LCD_Change_Pixel(COM0, SEG16, SET);
        }
        else if (type == ALARM_PART)
        {
            HT162x_LCD_Change_Pixel(COM7, SEG15, SET);
            HT162x_LCD_Change_Pixel(COM7, SEG07, SET);
        }
        HT162x_LCD_Week_Set(type, (LCD_Week_Day_t)(time.weekday - 1)); //week
        memcpy(&time_cache[type], &time, sizeof(mico_rtc_time_t));
    }
}
/**
 ****************************************************************************
 * @Function : void HT162x_LCD_Time_Display(mico_rtc_time_t time)
 * @File     : ht162x.c
 * @Program  : time:the time to display
 * @Created  : 2017/12/5 by seblee
 * @Brief    : refresh time
 * @Version  : V1.0
**/
void HT162x_LCD_Time_Display(LCD_Time_Type_t type, mico_rtc_time_t time)
{
    static mico_rtc_time_t time_cache[2] = {
        {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
        {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
    };
    static uint8_t time_format[2] = {0};
    uint8_t cache;

    if (type >= TIME_CLEAR)
    {
        if (time_cache[type - TIME_CLEAR].year == 0xff)
            return;
        memset(&time_cache[type - TIME_CLEAR], 0xff, sizeof(mico_rtc_time_t));
        /*****hour*******/
        HT162x_LCD_Double_Digits_Write(Position[type - TIME_CLEAR][DIGIT_HOUR], time.hr, 0);
        /*******point**********/
        if ((type - TIME_CLEAR) == ALARM_PART)
        {
            HT162x_LCD_Change_Pixel(COM7, SEG31, RESET);
            HT162x_LCD_Change_Pixel(COM7, SEG30, RESET);
        }
        /****minute******/
        HT162x_LCD_Double_Digits_Write(Position[type - TIME_CLEAR][DIGIT_MINUTE], time.min, 0);
        /*******am pm***********/
        HT162x_LCD_AMPM_Set((LCD_Time_Type_t)(type - TIME_CLEAR), AMPMMAX);
        return;
    }

    if ((time_cache[type].hr != time.hr) ||
        (time_cache[type].min != time.min) ||
        (time_format[type] != eland_data.time_display_format))
    {
        cache = time.hr;
        if (eland_data.time_display_format == 1)
        {
            if (time.hr >= 12)
                HT162x_LCD_AMPM_Set(type, PM);
            else
                HT162x_LCD_AMPM_Set(type, AM);
            if (time.hr > 12)
                cache -= 12;
        }
        else
            HT162x_LCD_AMPM_Set(type, AMPMMAX);
        /*****hour*******/
        HT162x_LCD_Double_Digits_Write(Position[type][DIGIT_HOUR], cache, 1);
        /****minute******/
        HT162x_LCD_Double_Digits_Write(Position[type][DIGIT_MINUTE], time.min, 2);
    }
    if (type == ALARM_PART)
    {
        HT162x_LCD_Change_Pixel(COM7, SEG31, SET);
        HT162x_LCD_Change_Pixel(COM7, SEG30, SET);
    }
    time_format[type] = eland_data.time_display_format;
    memcpy(&time_cache[type], &time, sizeof(mico_rtc_time_t));
}
