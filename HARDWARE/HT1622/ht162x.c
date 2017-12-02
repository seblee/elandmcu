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
static void HT162x_Write_Bit(uint8_t data, uint8_t count, __SEQUENT_t sequent)
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
static void HT162x_Write_Data(uint8_t addrass, uint8_t data)
{
    HT162x_CS_RESET;
    /*change data pin configration*/
    HT162x_DATA_SET_OUT;
    /* Move the actual address to high*/
    addrass <<= 1;
    /*Move the actual data to high*/
    data <<= 4;
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
        data <<= 1;
        HT162x_RD_SET;      //“写”时钟置1
        if (HT162x_DATA_IN) //数据线為1
            data |= 1;
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
    data = HT162x_Read_Bit(LEN_DATA);
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
static void HT162x_Read_Data_Continuously(uint8_t address, uint8_t *data, uint8_t length)
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
        *(data + i) = HT162x_Read_Bit(LEN_DATA);
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
    data = HT162x_Read_Bit(LEN_DATA);
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
    for (i = 0; i < HT1623_MEM_NUM, i++)
    {
        /*change data pin configration*/
        HT162x_DATA_SET_IN;
        /*read BITS */
        data = HT162x_Read_Bit(LEN_DATA);
        /*set the corresponding bit*/
        if ((i % 2) == (comx > COM3))
        {
            if (value == RESET)
                data &= (~(1 << (comx % 4)));
            else
                data |= (1 << (comx % 4));
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
