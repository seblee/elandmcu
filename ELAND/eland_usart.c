/**
 ****************************************************************************
 * @Warning :Without permission from the author,Not for commercial use
 * @File    :eland_usart.c
 * @Author  :seblee
 * @date    :2017/11/1
 * @version :V 1.0.0
 *************************************************
 * @brief   :
 ****************************************************************************
**/
/* Private include -----------------------------------------------------------*/
#include "eland_usart.h"
#include "key.h"
#include "usart.h"
#include "rtc.h"
#include "ht162x.h"
#include "eland_ota.h"
#include "lcd_display.h"
#include "rgbled.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint8_t Firmware_Conter = 0;
uint8_t msg_receive_buff[30];
Eland_Status_type_t eland_state = ElandNone;
int32_t RSSI_Value = -120;
MCU_Refresh_type_t MCU_Refreshed = REFRESH_NONE;

/* Private function prototypes -----------------------------------------------*/
static void OprationFrame(void);
static void MODH_Opration_02H(void);
static void MODH_Opration_03H(void);
static void MODH_Opration_04H(void);
static void MODH_Opration_05H(void);
static void MODH_Opration_06H(void);
static void MODH_Opration_07H(void);
static void MODH_Opration_08H(void);
static void MODH_Opration_09H(void);
static void MODH_Opration_10H(void);
/* Private functions ---------------------------------------------------------*/

/**
 ****************************************************************************
 * @Function : void ReceiveUsart(u8 Cache)
 * @File     : eland_usart.c
 * @Program  : Cache:data received
 * @Created  : 2017/11/1 by seblee
 * @Brief    : receive frame
 * @Version  : V1.0
**/
void ReceiveUsart(u8 Cache)
{
    static __msg_state_t UartStatus;
    static uint8_t count, lenth;
    switch (UartStatus)
    {
    case FrameHeadSataus:
    {
        msg_receive_buff[0] = msg_receive_buff[1];
        msg_receive_buff[1] = msg_receive_buff[2];
        msg_receive_buff[2] = Cache;
        if (msg_receive_buff[0] == Uart_Packet_Header)
        {
            count = 0;
            lenth = msg_receive_buff[2];
            if (lenth != 0)
                UartStatus = FrameDataStatus;
            else
                UartStatus = FrameTrailSataus;
        }
    }
    break;
    case FrameDataStatus:
    {
        msg_receive_buff[count + 3] = Cache;
        count++;
        if (count >= lenth)
            UartStatus = FrameTrailSataus;
    }
    break;
    case FrameTrailSataus:
    {
        msg_receive_buff[lenth + 3] = Cache;
        if (msg_receive_buff[lenth + 3] == Uart_Packet_Trail)
            UartStatus = FrameEndStatus;
        else
            UartStatus = FrameHeadSataus;
    }
    break;
    default:
        UartStatus = FrameHeadSataus;
        break;
    }
    if (UartStatus == FrameEndStatus) //接收完一帧处理数据
    {
        //add Opration function
        OprationFrame();
        UartStatus = FrameHeadSataus;
        count = 0;
        IWDG_ReloadCounter();
    }
}
/**
 ****************************************************************************
 * @Function : static void OprationFrame(void)
 * @File     : eland_usart.c
 * @Program  : none
 * @Created  : 2017/11/1 by seblee
 * @Brief    : oprate received data
 * @Version  : V1.0
**/
static void OprationFrame(void)
{
    switch (msg_receive_buff[1])
    {
    case KEY_READ_02:
        MODH_Opration_02H();
        break;
    case TIME_SET_03:
        MODH_Opration_03H();
        break;
    case TIME_READ_04:
        MODH_Opration_04H();
        break;
    case ELAND_STATES_05:
        MODH_Opration_05H();
        break;
    case SEAD_FIRM_WARE_06:
        MODH_Opration_06H();
        break;
    case REND_FIRM_WARE_07:
        MODH_Opration_07H();
        break;
    case SEND_LINK_STATE_08:
        MODH_Opration_08H();
        break;
    case MCU_FIRM_WARE_09:
        MODH_Opration_09H();
        break;
    case ALARM_READ_10:
        MODH_Opration_10H();
        break;

        break;
    default:
        break;
    }
}
/**
 ****************************************************************************
 * @Function : void MODH_Opration_02H(void)
 * @File     : eland_usart.c
 * @Program  : H02 header fun len 鍵值狀態(2Byte) 長按狀態(2Byte)  tral
 *                    55   02  04   xx xx             xx xx       0xaa
 * @Created  : 2017/11/1 by seblee
 * @Brief    : oprate KEY_READ_02
 * @Version  : V1.0
**/
static void MODH_Opration_02H(void)
{
    uint8_t *SendBuf;
    SendBuf = calloc(9, sizeof(uint8_t));
    *SendBuf = Uart_Packet_Header;
    *(SendBuf + 1) = KEY_READ_02;
    *(SendBuf + 2) = 5;
    *(SendBuf + 3) = (uint8_t)(Key_Count >> 8);
    *(SendBuf + 4) = (uint8_t)(Key_Count & 0xff);
    *(SendBuf + 5) = (uint8_t)(Key_Restain >> 8);
    *(SendBuf + 6) = (uint8_t)(Key_Restain & 0xff);
    *(SendBuf + 7) = (uint8_t)(MCU_Refreshed);
    *(SendBuf + 8) = Uart_Packet_Trail;
    USARTx_Send_Data(USART1, SendBuf, 9);
    free(SendBuf);
    MCU_Refreshed = REFRESH_NONE;
}
/**
 ****************************************************************************
 * @Function : void MODH_Opration_03H(void)
 * @File     : eland_usart.c
 * @Program  : H03 header fun len cur_time(....)  tral
 *                    55   03 len cur_time        0xaa
 * @Created  : 2017/11/13 by seblee
 * @Brief    : RTC time set
 * @Version  : V1.0
**/
static void MODH_Opration_03H(void)
{
    uint8_t *SendBuf;
    mico_rtc_time_t mico_time;
    _eland_date_time_t mcu_time;
    memcpy(&mico_time, &msg_receive_buff[3], sizeof(mico_rtc_time_t));
    ELAND_Time_Convert(&mico_time, &mcu_time, MICO_2_MCU);
    RTC_Time_Set(mcu_time);
    SendBuf = calloc(4, sizeof(uint8_t));
    *SendBuf = Uart_Packet_Header;
    *(SendBuf + 1) = TIME_SET_03;
    *(SendBuf + 2) = 0;
    *(SendBuf + 3) = Uart_Packet_Trail;
    USARTx_Send_Data(USART1, SendBuf, 4);
    free(SendBuf);
}
/**
 ****************************************************************************
 * @Function : static void MODH_Opration_04H(void)
 * @File     : lcd_eland.c
 * @Program  : H04 header fun len cur_time(....)  tral
 *                    55   03 len cur_time        0xaa
 * @Created  : 2017/11/15 by seblee
 * @Brief    : RTC time read
 * @Version  : V1.0
**/
static void MODH_Opration_04H(void)
{
    uint8_t *SendBuf;
    mico_rtc_time_t mico_time;
    memset(&mico_time, 0, sizeof(mico_rtc_time_t));
    ELAND_RTC_Read(&mico_time);

    SendBuf = calloc(4 + sizeof(mico_rtc_time_t), sizeof(uint8_t));
    *SendBuf = Uart_Packet_Header;
    *(SendBuf + 1) = TIME_READ_04;
    *(SendBuf + 2) = sizeof(mico_rtc_time_t);
    memcpy((SendBuf + 3), &mico_time, sizeof(mico_rtc_time_t));
    *(SendBuf + sizeof(mico_rtc_time_t) + 3) = Uart_Packet_Trail;

    USARTx_Send_Data(USART1, SendBuf, 4 + sizeof(mico_rtc_time_t));
    free(SendBuf);
}
/**
 ****************************************************************************
 * @Function : static void MODH_Opration_05H(void)
 * @File     : eland_usart.c
 * @Program  : H04 header fun len  tral
 *                    55   05  0   0xaa
 * @Created  : 2017/12/12 by seblee
 * @Brief    : get state
 * @Version  : V1.0
**/
static void MODH_Opration_05H(void)
{
    uint8_t cache;
    static uint8_t state_receive_time = 0;
    uint8_t *SendBuf;
    cache = eland_state;
    eland_state = (Eland_Status_type_t)msg_receive_buff[3];
    SendBuf = calloc(4, sizeof(uint8_t));
    *SendBuf = Uart_Packet_Header;
    *(SendBuf + 1) = ELAND_STATES_05;
    *(SendBuf + 2) = 0;
    *(SendBuf + 3) = Uart_Packet_Trail;
    USARTx_Send_Data(USART1, SendBuf, 4);
    free(SendBuf);
    if (cache != eland_state)
    {
        HT162x_LCD_Num_Set(Serial_15, eland_state / 10);
        HT162x_LCD_Num_Set(Serial_16, eland_state % 10);
    }
    if (cache >= TCP_CN00)
    {
        if (state_receive_time++ >= 100)
            state_receive_time = 0;
        HT162x_LCD_Num_Set(Serial_13, state_receive_time / 10);
        HT162x_LCD_Num_Set(Serial_14, state_receive_time % 10);
    }
}
/**
 ****************************************************************************
 * @Function : static void MODH_Opration_06H(void)
 * @File     : eland_usart.c
 * @Program  : H04 header fun len  tral
 *                    55   05  0   0xaa
 * @Created  : 2017/12/16 by seblee
 * @Brief    : get eland firmware
 * @Version  : V1.0
**/
static void MODH_Opration_06H(void)
{
    uint8_t *SendBuf;
    sscanf((char const *)&msg_receive_buff[3], "%02d.%02d", &Firmware_Version_Major, &Firmware_Version_Minor);
    SendBuf = calloc(4, sizeof(uint8_t));
    *SendBuf = Uart_Packet_Header;
    *(SendBuf + 1) = SEAD_FIRM_WARE_06;
    *(SendBuf + 2) = 0;
    *(SendBuf + 3) = Uart_Packet_Trail;
    USARTx_Send_Data(USART1, SendBuf, 4);
    free(SendBuf);

    HT162x_LCD_Num_Set(Serial_17, Firmware_Version_Major / 10);
    HT162x_LCD_Num_Set(Serial_18, Firmware_Version_Major % 10);
    HT162x_LCD_Num_Set(Serial_19, Firmware_Version_Minor / 10);
    HT162x_LCD_Num_Set(Serial_20, Firmware_Version_Minor % 10);
}
/**
 ****************************************************************************
 * @Function : static void MODH_Opration_07H(void)
 * @File     : eland_usart.c
 * @Program  : NONE
 * @Created  : 2018/1/8 by seblee
 * @Brief    : READ MCU FIRMWARE
 * @Version  : V1.0
**/
static void MODH_Opration_07H(void)
{
    uint8_t *SendBuf;
    SendBuf = calloc(9, sizeof(uint8_t));
    *SendBuf = Uart_Packet_Header;
    *(SendBuf + 1) = REND_FIRM_WARE_07;
    *(SendBuf + 2) = 5;

    sprintf((char *)(SendBuf + 3), "%02d.%02d", MCU_VERSION_MAJOR, MCU_VERSION_MINOR);
    *(SendBuf + 8) = Uart_Packet_Trail;
    USARTx_Send_Data(USART1, SendBuf, 9);
    free(SendBuf);
}

/**
 ****************************************************************************
 * @Function : static void MODH_Opration_08H(void)
 * @File     : eland_usart.c
 * @Program  : H08 header fun    RSSI       tral
 *                    55   08  RSSI value   0xaa
 * @Created  : 2018/1/3 by seblee
 * @Brief    : get wifi rssi
 * @Version  : V1.0
**/
static void MODH_Opration_08H(void)
{
    uint8_t *SendBuf;
    RSSI_Value = (int32_t)((uint32_t)msg_receive_buff[3] |
                           ((uint32_t)msg_receive_buff[4] << 8) |
                           ((uint32_t)msg_receive_buff[5] << 16) |
                           ((uint32_t)msg_receive_buff[6] << 24));

    SendBuf = calloc(4, sizeof(uint8_t));
    *SendBuf = Uart_Packet_Header;
    *(SendBuf + 1) = SEND_LINK_STATE_08;
    *(SendBuf + 2) = 0;
    *(SendBuf + 3) = Uart_Packet_Trail;
    USARTx_Send_Data(USART1, SendBuf, 4);
    free(SendBuf);
}
/**
 ****************************************************************************
 * @Function : static void MODH_Opration_09H(void)
 * @File     : eland_usart.c
 * @Program  : H08 header fun len   tral
 *                    55   09  00   0xaa
 * @Created  : 2018/1/4 by seblee
 * @Brief    : firmware update
 * @Version  : V1.0
**/
static void MODH_Opration_09H(void)
{
    uint8_t *SendBuf;
    SendBuf = calloc(4, sizeof(uint8_t));
    *SendBuf = Uart_Packet_Header;
    *(SendBuf + 1) = MCU_FIRM_WARE_09;
    *(SendBuf + 2) = 0;
    *(SendBuf + 3) = Uart_Packet_Trail;
    USARTx_Send_Data(USART1, SendBuf, 4);
    free(SendBuf);
    OTA_start();
}
/**
 ****************************************************************************
 * @Function : static void MODH_Opration_10H(void)
 * @File     : eland_usart.c
 * @Program  : H08 header fun len   tral
 * @Created  : 2018/1/11 by seblee
 * @Brief    : SEND MCU_ALARM TO ELAND
 * @Version  : V1.0
**/
static void MODH_Opration_10H(void)
{
    uint8_t *SendBuf;
    SendBuf = calloc(4 + sizeof(_alarm_MCU_data_t), sizeof(uint8_t));
    *SendBuf = Uart_Packet_Header;
    *(SendBuf + 1) = ALARM_READ_10;
    *(SendBuf + 2) = sizeof(_alarm_MCU_data_t);
    memcpy((SendBuf + 3), &alarm_data, sizeof(_alarm_MCU_data_t));
    *(SendBuf + 3 + sizeof(_alarm_MCU_data_t)) = Uart_Packet_Trail;
    USARTx_Send_Data(USART1, SendBuf, 4 + sizeof(_alarm_MCU_data_t));
    free(SendBuf);
}
