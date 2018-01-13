/**
 ****************************************************************************
 * @Warning :Without permission from the author,Not for commercial use
 * @File    :eland_ota.h
 * @Author  :seblee
 * @date    :2018/1/4
 * @version :V 1.0.0
 *************************************************
 * @brief   :
 ****************************************************************************
**/
#ifndef __ELAND_OTA_H_
#define __ELAND_OTA_H_
/* Private include -----------------------------------------------------------*/
#include "syscfg.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void OTA_bootloader_enable(void);
void OTA_bootloader_disable(void);
void OTA_start(void);

/****************************/
#endif /*__ELAND_OTA_H_*/