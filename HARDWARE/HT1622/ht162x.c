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
const uint16_t NumberMap[10] = {
    // /* 0       1       2       3       4   */
    // 0XF757, 0XF200, 0XB776, 0XF772, 0XF233,
    // /* 5       6       7       8       9   */
    // 0XD773, 0XD777, 0XF310, 0XF777, 0XF773
    /* 0       1       2       3       4   */
    0xFEAE, 0xF400, 0xBEEC, 0xFEE4, 0xF466,
    /* 5       6       7       8       9   */
    0xDEE6, 0xDEEE, 0xF620, 0xFEEE, 0xFEE6};

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
    HT162x_RD_SET;   //CS
    HT162x_WR_SET;   //CS
    HT162x_DATA_SET; //CS

    HT162x_Write_Command(SYS_EN); //Turn on system oscillator
    HT162x_Write_Command(LCD_ON); //Turn on LCD display
    HT162x_Write_Command(RC_32K); //System clock source,on - chip RC oscillator
    HT162x_Write_Command(F128);   //Time base clock output : 128Hz
    HT162x_Write_Command(NORMAL); //Normal mod

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
        HT162x_WR_SET;         //“写”时钟置1
        if (sequent == MSB)
            Temp >>= 1;
        else if (sequent == LSB)
            Temp <<= 1; //准备下一位
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
        data >>= 1;
        HT162x_RD_SET; //“写”时钟置1
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
    uint8_t *cache;
    cache = malloc(HT1623_MEM_NUM);
    if (value)
        memset(cache, 0xff, HT1623_MEM_NUM);
    else
        memset(cache, 0, HT1623_MEM_NUM);
    TH162x_Write_Data_Write_Continuously(0, cache, HT1623_MEM_NUM);
    free(cache);
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
    /*change data pin configration*/
    HT162x_DATA_SET_IN;
    /*read BITS */
    Cache = HT162x_Read_Bit(LEN_DATA) >> 4;
    /*clear corresponding bits*/
    Cache &= Mask;
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
        HT162x_LCD_RAM_Change((129 - Serial * 8), Cache.Digital_FECB, 0x08);
        HT162x_LCD_RAM_Change((129 - Serial * 8 + 2), Cache.Digital_GDA, 0x08);
        HT162x_LCD_RAM_Change((129 - Serial * 8 + 4), Cache.Digital_HML, 0x08);
        HT162x_LCD_RAM_Change((129 - Serial * 8 + 6), Cache.Digital_IJK, 0x08);

        HT162x_LCD_Change_Pixel(COM7, SEG14, (FlagStatus)(Cache.Digital_FECB & 0x08));
    }
    else if (Serial < Serial_16) //Serial_12 ~ 16
    {
        Cache = HT162x_LCD_Digital_Convert(NEGATIVE, data);
        HT162x_LCD_RAM_Change((129 - Serial * 8), Cache.Digital_FECB, 0x00);
        HT162x_LCD_RAM_Change((129 - Serial * 8 + 2), Cache.Digital_GDA, 0x08);
        HT162x_LCD_RAM_Change((129 - Serial * 8 + 4), Cache.Digital_HML, 0x08);
        HT162x_LCD_RAM_Change((129 - Serial * 8 + 6), Cache.Digital_IJK, 0x08);
    }
    else //Serial_17 ~ 20
    {
        Cache = HT162x_LCD_Digital_Convert(NEGATIVE, data);
        HT162x_LCD_RAM_Change((209 - Serial * 8), Cache.Digital_FECB, 0x00);
        HT162x_LCD_RAM_Change((209 - Serial * 8 + 2), Cache.Digital_GDA, 0x08);
        HT162x_LCD_RAM_Change((209 - Serial * 8 + 4), Cache.Digital_HML, 0x08);
        HT162x_LCD_RAM_Change((209 - Serial * 8 + 6), Cache.Digital_IJK, 0x08);
    }
}
