/**
 ****************************************************************************
 * @Warning :Without permission from the author,Not for commercial use
 * @File    :usart.c
 * @Author  :seblee
 * @date    :2017/10/27
 * @version :V 1.0.0
 *************************************************
 * @brief   :
 ****************************************************************************
**/
/* Private include -----------------------------------------------------------*/
#include "usart.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void stm8_uart1_init(u32 USART_BaudRate,
                            USART_WordLength_TypeDef USART_WordLength,
                            USART_StopBits_TypeDef USART_StopBits,
                            USART_Parity_TypeDef USART_Parity,
                            USART_Mode_TypeDef USART_Mode);

/* Private functions ---------------------------------------------------------*/

/**
 ****************************************************************************
 * @Function : void UART1_Init(void)
 * @File     : uart.c
 * @Program  : none
 * @Created  : 2017/10/20 by seblee
 * @Brief    : serial 1 init
 * @Version  : V1.0
**/
void UART1_Init(void)
{
    stm8_uart1_init(115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_TypeDef)(USART_Mode_Tx | USART_Mode_Rx));

    /* Enable the USART Receive interrupt: this interrupt is generated when the USART
    receive data register is not empty */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    /* Enable the USART Transmit complete interrupt: this interrupt is generated when the USART
    transmit Shift Register is empty */
    //USART_ITConfig(EVAL_COM1, USART_IT_TC, ENABLE);

    /* Enable USART */
    USART_Cmd(USART1, ENABLE);
}
/**
 ****************************************************************************
 * @Function : void stm8_uart_init(CLK_Peripheral_TypeDef UARTx,u32 USART_BaudRate,USART_WordLength_TypeDef USART_WordLength, USART_StopBits_TypeDef USART_StopBits,USART_Parity_TypeDef USART_Parity,USART_Mode_TypeDef USART_Mode )
 * @File     : usart.c
 * @Program  :
 * @Created  : 2017/10/20 by seblee
 * @Brief    : USART init
 * @Version  : V1.0
**/
static void stm8_uart1_init(u32 USART_BaudRate,
                            USART_WordLength_TypeDef USART_WordLength,
                            USART_StopBits_TypeDef USART_StopBits,
                            USART_Parity_TypeDef USART_Parity,
                            USART_Mode_TypeDef USART_Mode)
{
    /* Enable USART clock */
    CLK_PeripheralClockConfig((CLK_Peripheral_TypeDef)CLK_Peripheral_USART1, ENABLE);

    /*Configure USART1 Tx- Rx (PC3- PC2) remapping to PA2- PA3*/
    SYSCFG_REMAPPinConfig(REMAP_Pin_USART1TxRxPortA, ENABLE);

    /* Configure USART Tx as alternate function push-pull  (software pull up)*/
    GPIO_ExternalPullUpConfig(GPIOA, GPIO_Pin_2, ENABLE);

    /* Configure USART Rx as alternate function push-pull  (software pull up)*/
    GPIO_ExternalPullUpConfig(GPIOA, GPIO_Pin_3, ENABLE);

    /* USART configuration */
    // USART_DeInit(USART1);
    USART_Init(USART1,
               USART_BaudRate,
               USART_WordLength,
               USART_StopBits,
               USART_Parity,
               USART_Mode);
}
/**
 ****************************************************************************
 * @Function : void USART1_RX_Service(void)
 * @File     : usart.c
 * @Program  : Cache received data
 * @Created  : 2017/10/20 by seblee
 * @Brief    : receive service function
 * @Version  : V1.0
**/
void USART1_RX_Service(void)
{
    u8 Cache;
    Cache = USART_ReceiveData8(USART1);
    USART_SendData8(USART1, Cache);
}
