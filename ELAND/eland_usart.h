/**
 ****************************************************************************
 * @Warning :Without permission from the author,Not for commercial use
 * @File    :eland_usart.h
 * @Author  :seblee
 * @date    :2017/11/1
 * @version :V 1.0.0
 *************************************************
 * @brief   :
 ****************************************************************************
**/
#ifndef __ELAND_USART_H_
#define __ELAND_USART_H_
/* Private include -----------------------------------------------------------*/
#include "syscfg.h"
/* Private typedef -----------------------------------------------------------*/
typedef enum {
    KEY_FUN_NONE = 0x00, /*空命令*/
    KEY_READ_02 = 0X02,
    TIME_SET_03,
    TIME_READ_04,
    ELAND_STATES_05,
    FIRM_WARE_06,
} __msg_function_t;

typedef enum {
    FrameHeadSataus,
    FrameDataStatus,
    FrameTrailSataus,
    FrameEndStatus,
} __msg_state_t;
/*Eland 状态*/
typedef enum {
    ElandNone = 0,
    ElandBegin,
    APStatus,
    APStatusClosed,
    HttpServerStatus,
    HttpServerStop,
    ELAPPConnected,
    WifyConnected,
    WifyDisConnected,
    WifyConnectedFailed,
    ElandAliloPlay,
    ElandAliloPause,
    ElandAliloStop,
    HTTP_Get_HOST_INFO,
    TCPConnectedELSV,
    TCPHealthCheck,
} Eland_Status_type_t;
/* Private define ------------------------------------------------------------*/
#define Uart_Packet_Header (uint8_t)(0x55)
#define Uart_Packet_Trail (uint8_t)(0xaa)
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern uint8_t Firmware_Conter;
extern Eland_Status_type_t eland_state;
/* Private function prototypes -----------------------------------------------*/
void ReceiveUsart(u8 Cache);
/* Private functions ---------------------------------------------------------*/

#endif /*__ELAND_USART_H_*/
