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
#define MCU_VERSION_MAJOR (uint8_t)1
#define MCU_VERSION_MINOR (uint8_t)26
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void OTA_bootloader_enable(void);
void OTA_bootloader_disable(void);
void OTA_start(void);
void MCU_RESET_STATE(void);
/****************************/
#endif /*__ELAND_OTA_H_*/
