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
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint8_t msg_receive_buff[30];
/* Private function prototypes -----------------------------------------------*/
static void OprationFrame(void);
static void MODH_Read_02H(void);
static void MODH_Read_03H(void);
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
        MODH_Read_02H();
        break;
    case TIME_SET_03:
        MODH_Read_03H();
        break;
    case TIME_READ_04:
        break;
    default:
        break;
    }
}
/**
 ****************************************************************************
 * @Function : void MODH_Read_02H(void)
 * @File     : eland_usart.c
 * @Program  : H02 header fun len 鍵值狀態(2Byte) 長按狀態(2Byte)  tral
 *                    55   02  04   xx xx             xx xx       0xaa
 * @Created  : 2017/11/1 by seblee
 * @Brief    : oprate KEY_READ_02
 * @Version  : V1.0
**/
static void MODH_Read_02H(void)
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
 * @Function : void MODH_Read_03H(void)
 * @File     : eland_usart.c
 * @Program  : H02 header fun len cur_time(....)  tral
 *                    55   03 len cur_time        0xaa
 * @Created  : 2017/11/13 by seblee
 * @Brief    : RTC time set
 * @Version  : V1.0
**/
static void MODH_Read_03H(void)
{
    uint8_t *SendBuf;
    platform_rtc_time_t time;
    _eland_date_time_t time_1;
    memcpy(&time, &msg_receive_buff[3], sizeof(platform_rtc_time_t));
    time_1 = ELAND_Time_Convert(time);
    RTC_Time_Set(time_1);
    SendBuf = calloc(4, sizeof(uint8_t));
    *SendBuf = Uart_Packet_Header;
    *(SendBuf + 1) = TIME_SET_03;
    *(SendBuf + 2) = 0;
    *(SendBuf + 3) = Uart_Packet_Trail;
    USARTx_Send_Data(USART1, SendBuf, 4);
    free(SendBuf);
}
