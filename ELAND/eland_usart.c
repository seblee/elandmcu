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
#include "lcd_eland.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint8_t msg_receive_buff[30];
Eland_Status_type_t eland_state = ElandNone;
/* Private function prototypes -----------------------------------------------*/
static void OprationFrame(void);
static void MODH_Opration_02H(void);
static void MODH_Opration_03H(void);
static void MODH_Opration_04H(void);
static void MODH_Opration_05H(void);
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
    SendBuf = calloc(8, sizeof(uint8_t));
    *SendBuf = Uart_Packet_Header;
    *(SendBuf + 1) = KEY_READ_02;
    *(SendBuf + 2) = 4;
    *(SendBuf + 3) = (uint8_t)(Key_Count >> 8);
    *(SendBuf + 4) = (uint8_t)(Key_Count & 0xff);
    *(SendBuf + 5) = (uint8_t)(Key_Restain >> 8);
    *(SendBuf + 6) = (uint8_t)(Key_Restain & 0xff);
    *(SendBuf + 7) = Uart_Packet_Trail;
    USARTx_Send_Data(USART1, SendBuf, 8);
    free(SendBuf);
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
    _eland_date_time_t mcu_time;
    memset(&mico_time, 0, sizeof(mico_rtc_time_t));
    ELAND_RTC_Read(&mcu_time);
    ELAND_Time_Convert(&mico_time, &mcu_time, MCU_2_MICO);

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
 * @Program  : H04 header fun len cur_time(....)  tral
 *                    55   03 len cur_time        0xaa
 * @Created  : 2017/12/11 by seblee
 * @Brief    : get eland state
 * @Version  : V1.0
**/
static void MODH_Opration_05H(void)
{
    uint8_t *SendBuf;
    eland_state = (Eland_Status_type_t)msg_receive_buff[3];
    SendBuf = calloc(4, sizeof(uint8_t));
    *SendBuf = Uart_Packet_Header;
    *(SendBuf + 1) = ELAND_STATES_05;
    *(SendBuf + 2) = 0;
    *(SendBuf + 3) = Uart_Packet_Trail;
    USARTx_Send_Data(USART1, SendBuf, 4);
    free(SendBuf);
}
/**
 ****************************************************************************
 * @Function : void eland_state_display(void)
 * @File     : eland_usart.c
 * @Program  : none
 * @Created  : 2017/12/11 by seblee
 * @Brief    : display eland state
 * @Version  : V1.0
**/
void eland_state_display(void)
{
    LCD_Wifi_Rssi_t AP_state = LEVEL0;
    switch (eland_state)
    {
    case ElandAPStatus:
        if (AP_state == LEVEL0)
            AP_state = LEVEL4;
        else
            AP_state = LEVEL0;
        LCD_Eland_Wifi_RSSI_Set(AP_state);
        break;
    default:
        break;
    }
}
