/**
 ****************************************************************************
 * @Warning :Without permission from the author,Not for commercial use
 * @File    :usart.h
 * @Author  :seblee
 * @date    :2017/10/27
 * @version :V 1.0.0
 *************************************************
 * @brief   :
 ****************************************************************************
**/
#ifndef __USART_H_
#define __USART_H_
/* Private include -----------------------------------------------------------*/
#include "syscfg.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void UART1_Init(void);
void USART1_RX_Service(void);
void USARTx_Send_Data(USART_TypeDef *USARTx, uint8_t *P_data, uint16_t length);

/* Private functions ---------------------------------------------------------*/

#endif /*__USART_H_*/
