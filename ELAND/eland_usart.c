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
uint8_t msg_receive_buff[300];
Eland_Status_type_t eland_state = ElandNone;
LCD_Wifi_Rssi_t RSSI_Value = LEVELNUM;
MCU_Refresh_type_t MCU_Refreshed = REFRESH_NONE;

/* Private function prototypes -----------------------------------------------*/
static void OprationFrame(void);
static void MODH_Opration_01H(void); /* SEND ELAND ERROR CODE*/
static void MODH_Opration_02H(void); /* READ MCU KEY STATE*/
static void MODH_Opration_03H(void); /* SEND ELAND TIME*/
static void MODH_Opration_04H(void); /* READ MCU TIME*/
static void MODH_Opration_05H(void); /* SEND ELAND STATES*/
static void MODH_Opration_06H(void); /* SEND ELAND FIRMWARE VERSION*/
static void MODH_Opration_07H(void); /* READ MUC FIRMWARE VERSION*/
static void MODH_Opration_08H(void); /* SEND WIFI LINK STATE*/
static void MODH_Opration_09H(void); /* START MCU FIRM WARE UPDATE*/
static void MODH_Opration_0AH(void); /* READ MCU ALARM*/
static void MODH_Opration_0BH(void); /* SEND NEXT ALARM STATE*/
static void MODH_Opration_0CH(void); /* SEND ELAND DATA TO MCU*/
static void MODH_Opration_0DH(void); /* RESET SYSTEM */
static void MODH_Opration_0EH(void); /* DEVICE DATA DELETE */
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
        IWDG_ReloadCounter();
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
    case SEND_ELAND_ERR_01:
        MODH_Opration_01H();
        break;
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
    case SEND_FIRM_WARE_06:
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
    case ALARM_READ_0A:
        MODH_Opration_0AH();
        break;
    case ALARM_SEND_0B:
        MODH_Opration_0BH();
        break;
    case ELAND_DATA_0C:
        MODH_Opration_0CH();
        break;
    case ELAND_RESET_0D:
        MODH_Opration_0DH();
        break;
    case ELAND_DELETE_0E:
        MODH_Opration_0EH();
        break;
    default:
        break;
    }
}
/**
 ****************************************************************************
 * @Function : static void MODH_Opration_01H(void)
 * @File     : eland_usart.c
 * @Program  : H02 header fun len error  tral
 *                    55   01  04   xx    aa
 * @Created  : 2017/11/1 by seblee
 * @Brief    : oprate get eland error
 * @Version  : V1.0
**/
static void MODH_Opration_01H(void)
{
    uint8_t SendBuf[4];
    __eland_error_t eland_error = (__eland_error_t)msg_receive_buff[3];
    if ((eland_error == EL_FLASH_OK) || (eland_error == EL_FLASH_ERR))
        eland_flash_state = eland_error;
    SendBuf[0] = Uart_Packet_Header;
    SendBuf[1] = SEND_ELAND_ERR_01;
    SendBuf[2] = 0;
    SendBuf[3] = Uart_Packet_Trail;
    USARTx_Send_Data(USART1, SendBuf, 4);
}
/**
 ****************************************************************************
 * @Function : static void MODH_Opration_02H(void)
 * @File     : eland_usart.c
 * @Program  : H02 header fun len 鍵值狀態(2Byte) 長按狀態(2Byte)  tral
 *                    55   02  04   xx xx             xx xx       0xaa
 * @Created  : 2017/11/1 by seblee
 * @Brief    : oprate KEY_READ_02
 * @Version  : V1.0
**/
static void MODH_Opration_02H(void)
{
    uint8_t SendBuf[9];
    SendBuf[0] = Uart_Packet_Header;
    SendBuf[1] = KEY_READ_02;
    SendBuf[2] = 5;
    SendBuf[3] = (uint8_t)(Key_Count >> 8);
    SendBuf[4] = (uint8_t)(Key_Count & 0xff);
    SendBuf[5] = (uint8_t)(Key_Restain >> 8);
    SendBuf[6] = (uint8_t)(Key_Restain & 0xff);
    SendBuf[7] = (uint8_t)(MCU_Refreshed);
    SendBuf[8] = Uart_Packet_Trail;
    USARTx_Send_Data(USART1, SendBuf, 9);

    MCU_Refreshed = REFRESH_NONE;
}
/**
 ****************************************************************************
 * @Function : static void MODH_Opration_03H(void)
 * @File     : eland_usart.c
 * @Program  : H03 header fun len cur_time(....)  tral
 *                    55   03 len cur_time        0xaa
 * @Created  : 2017/11/13 by seblee
 * @Brief    : RTC time set
 * @Version  : V1.0
**/
static void MODH_Opration_03H(void)
{
    uint8_t SendBuf[4];
    mico_rtc_time_t mico_time;
    _eland_date_time_t mcu_time;
    memcpy(&mico_time, &msg_receive_buff[3], sizeof(mico_rtc_time_t));
    if (memcmp(&mico_time, &CurrentMicoTime, sizeof(mico_rtc_time_t)))
    {
        ELAND_Time_Convert(&mico_time, &mcu_time, MICO_2_MCU);
        RTC_Time_Set(mcu_time);
    }
    SendBuf[0] = Uart_Packet_Header;
    SendBuf[1] = TIME_SET_03;
    SendBuf[2] = 0;
    SendBuf[3] = Uart_Packet_Trail;
    USARTx_Send_Data(USART1, SendBuf, 4);
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
    uint8_t SendBuf[4 + sizeof(mico_rtc_time_t)];
    mico_rtc_time_t mico_time;
    memset(&mico_time, 0, sizeof(mico_rtc_time_t));
    ELAND_RTC_Read(&mico_time);

    SendBuf[0] = Uart_Packet_Header;
    SendBuf[1] = TIME_READ_04;
    SendBuf[2] = sizeof(mico_rtc_time_t);
    memcpy((SendBuf + 3), &mico_time, sizeof(mico_rtc_time_t));
    SendBuf[sizeof(mico_rtc_time_t) + 3] = Uart_Packet_Trail;

    USARTx_Send_Data(USART1, SendBuf, 4 + sizeof(mico_rtc_time_t));
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
    uint8_t SendBuf[4];
    eland_state = (Eland_Status_type_t)msg_receive_buff[3];
    SendBuf[0] = Uart_Packet_Header;
    SendBuf[1] = ELAND_STATES_05;
    SendBuf[2] = 0;
    SendBuf[3] = Uart_Packet_Trail;
    USARTx_Send_Data(USART1, SendBuf, 4);
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
    uint8_t SendBuf[4];
    sscanf((char const *)&msg_receive_buff[3], "%02d.%02d", &Firmware_Version_Major, &Firmware_Version_Minor);
    SendBuf[0] = Uart_Packet_Header;
    SendBuf[1] = SEND_FIRM_WARE_06;
    SendBuf[2] = 0;
    SendBuf[3] = Uart_Packet_Trail;
    USARTx_Send_Data(USART1, SendBuf, 4);
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
    uint8_t SendBuf[9];
    SendBuf[0] = Uart_Packet_Header;
    SendBuf[1] = REND_FIRM_WARE_07;
    SendBuf[2] = 5;

    sprintf((char *)(SendBuf + 3), "%02d.%02d", MCU_VERSION_MAJOR, MCU_VERSION_MINOR);
    SendBuf[8] = Uart_Packet_Trail;
    USARTx_Send_Data(USART1, SendBuf, 9);
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
    uint8_t SendBuf[4];
    RSSI_Value = (LCD_Wifi_Rssi_t)msg_receive_buff[3];
    Eland_mode = (_ELAND_MODE_t)msg_receive_buff[4];
    eland_state = (Eland_Status_type_t)msg_receive_buff[5];
    SendBuf[0] = Uart_Packet_Header;
    SendBuf[1] = SEND_LINK_STATE_08;
    SendBuf[2] = 0;
    SendBuf[3] = Uart_Packet_Trail;
    USARTx_Send_Data(USART1, SendBuf, 4);
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
    uint8_t SendBuf[4];
    SendBuf[0] = Uart_Packet_Header;
    SendBuf[1] = MCU_FIRM_WARE_09;
    SendBuf[2] = 0;
    SendBuf[3] = Uart_Packet_Trail;
    USARTx_Send_Data(USART1, SendBuf, 4);
    OTA_start();
}
/**
 ****************************************************************************
 * @Function : static void MODH_Opration_0AH(void)
 * @File     : eland_usart.c
 * @Program  : H08 header fun len   tral
 * @Created  : 2018/1/11 by seblee
 * @Brief    : SEND MCU_ALARM TO ELAND
 * @Version  : V1.0
**/
static void MODH_Opration_0AH(void)
{
    uint8_t SendBuf[4 + sizeof(_alarm_mcu_data_t)];
    SendBuf[0] = Uart_Packet_Header;
    SendBuf[1] = ALARM_READ_0A;
    SendBuf[2] = sizeof(_alarm_mcu_data_t);
    memcpy((SendBuf + 3), &alarm_data_simple, sizeof(_alarm_mcu_data_t));
    SendBuf[3 + sizeof(_alarm_mcu_data_t)] = Uart_Packet_Trail;
    USARTx_Send_Data(USART1, SendBuf, 4 + sizeof(_alarm_mcu_data_t));
}
/**
 ****************************************************************************
 * @Function : static void MODH_Opration_0BH(void)
 * @File     : eland_usart.c
 * @Program  : H08 header fun len  alarmdata   tral
 *                  55     0b len  alarmdata   aa
 * @Created  : 2018/1/11 by seblee
 * @Brief    :   MCU_ALARM TO display
 * @Version  : V1.0
**/
static void MODH_Opration_0BH(void)
{
    uint8_t SendBuf[4];
    if (msg_receive_buff[2] == sizeof(_alarm_mcu_data_t))
    {
        memcpy(&alarm_data_display, &msg_receive_buff[3], sizeof(_alarm_mcu_data_t));
        Alarm_is_empty = FALSE;
    }
    else if (msg_receive_buff[2] == 0)
    {
        memset(&alarm_data_display, 0, sizeof(_alarm_mcu_data_t));
        Alarm_is_empty = TRUE;
    }
    SendBuf[0] = Uart_Packet_Header;
    SendBuf[1] = ALARM_SEND_0B;
    SendBuf[2] = 0;
    SendBuf[3] = Uart_Packet_Trail;
    USARTx_Send_Data(USART1, SendBuf, 4);

    Alarm_need_Refresh = TRUE;
}
/**
 ****************************************************************************
 * @Function : static void MODH_Opration_0CH(void)
 * @File     : eland_usart.c
 * @Program  : H08 header fun len  elanddata   tral
 *                  55     0b len  elanddata   aa
 * @Created  : 2018/1/11 by seblee
 * @Brief    : MCU_ALARM TO display
 * @Version  : V1.0
**/
static void MODH_Opration_0CH(void)
{
    uint8_t SendBuf[4];
    memcpy(&eland_data, &msg_receive_buff[3], sizeof(__ELAND_DATA_2_MCU_t));
    eland_data.night_mode_begin_time = msg_receive_buff[11] |
                                       (uint32_t)msg_receive_buff[12] << 8 |
                                       (uint32_t)msg_receive_buff[13] << 16 |
                                       (uint32_t)msg_receive_buff[14] << 24;
    eland_data.night_mode_end_time = msg_receive_buff[15] |
                                     (uint32_t)msg_receive_buff[16] << 8 |
                                     (uint32_t)msg_receive_buff[17] << 16 |
                                     (uint32_t)msg_receive_buff[18] << 24;
    SendBuf[0] = Uart_Packet_Header;
    SendBuf[1] = ELAND_DATA_0C;
    SendBuf[2] = 0;
    SendBuf[3] = Uart_Packet_Trail;
    USARTx_Send_Data(USART1, SendBuf, 4);
    ELAND_DATA_Refreshed = TRUE;
}

/**
 ****************************************************************************
 * @Function : static void MODH_Opration_0DH(void)
 * @File     : eland_usart.c
 * @Program  : H08 header fun len   tral
 *                  55     0b  0     aa
 * @Created  : 2018/1/11 by seblee
 * @Brief    : MCU_ALARM TO display
 * @Version  : V1.0
**/
static void MODH_Opration_0DH(void)
{
    HT162x_LCD_Clear(RESET);
    eland_state = ElandNone;
    RSSI_Value = LEVELNUM;
    RTC_Time_Set(CurrentMCUTime);
    ELAND_RTC_Read(&CurrentMicoTime);
    memset(&alarm_data_display, 0, sizeof(_alarm_mcu_data_t));
    MCU_RESET_STATE();
    asm("jp 0x8000"); // jump to given entry point address
}
/**
 ****************************************************************************
 * @Function : static void MODH_Opration_0EH(void)
 * @File     : eland_usart.c
 * @Program  : none
 * @Created  : 2018/4/8 by seblee
 * @Brief    : eland data delete
 * @Version  : V1.0
**/
static void MODH_Opration_0EH(void)
{
    HT162x_LCD_Clear(RESET);
    eland_state = ElandNone;
    RSSI_Value = LEVELNUM;
    memset(&alarm_data_display, 0, sizeof(_alarm_mcu_data_t));
    MCU_RESET_STATE();
    asm("jp 0x8000"); // jump to given entry point address
}
