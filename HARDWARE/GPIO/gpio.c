
/**
 ****************************************************************************
 * @Warning :Without permission from the author,Not for commercial use
 * @File    :gpio.c
 * @Author  :Xiaowine
 * @date    :2017/10/20
 * @version :V 1.0.0
 *************************************************
 * @brief   :
 ****************************************************************************
**/
/* Private include -----------------------------------------------------------*/
#include "gpio.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
 ****************************************************************************
 * @Function : void GPIO_Config(void)
 * @File     : gpio.c
 * @Program  : none
 * @Created  : 2017/10/20 by Xiaowine
 * @Brief    : cfg LED  KEY ....
 * @Version  : V1.0
**/
void GPIO_Config(void)
{
    GPIO_Init(GPIOI, GPIO_Pin_0 | GPIO_Pin_3, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_Init(GPIOH, GPIO_Pin_6 | GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Fast);
}
