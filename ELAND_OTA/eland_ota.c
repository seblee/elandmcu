/**
 ****************************************************************************
 * @Warning :Without permission from the author,Not for commercial use
 * @File    :eland_ota.c
 * @Author  :seblee
 * @date    :2018/1/4
 * @version :V 1.0.0
 *************************************************
 * @brief   :
 ****************************************************************************
**/
/* Private include -----------------------------------------------------------*/
#include "eland_ota.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/**
 ****************************************************************************
 * @Function : void OTA_bootloader_enable(void)
 * @File     : eland_ota.c
 * @Program  : none
 * @Created  : 2018/1/8 by seblee
 * @Brief    : enable bootloader
 * @Version  : V1.0
**/
void OTA_bootloader_enable(void)
{
    FLASH_Unlock(FLASH_MemType_Data);
    asm("nop");
    while (!FLASH_GetFlagStatus(FLASH_FLAG_DUL)) // 等待解锁
        ;
    asm("nop");
#if defined(STM8L15X_MD) || defined(STM8L15X_MDP) || defined(STM8L15X_LD) || defined(STM8L15X_HD) //STM8L
    FLASH_ProgramOptionByte(0x480b, 0x55);
    FLASH_ProgramOptionByte(0x480c, 0xaa);
#else //STM8S
    FLASH_ProgramOptionByte(0x487e, 0x55);
    FLASH_ProgramOptionByte(0x487f, 0xaa);
#endif
    FLASH_Lock(FLASH_MemType_Data);
}
/**
 ****************************************************************************
 * @Function : void OTA_bootloader_disable(void)
 * @File     : eland_ota.c
 * @Program  : none
 * @Created  : 2018/1/8 by seblee
 * @Brief    : disable bootloader
 * @Version  : V1.0
**/
void OTA_bootloader_disable(void)
{
    FLASH_Unlock(FLASH_MemType_Data);
    asm("nop");
    while (!FLASH_GetFlagStatus(FLASH_FLAG_DUL)) // 等待解锁
        ;
    asm("nop");
#if defined(STM8L15X_MD) || defined(STM8L15X_MDP) || defined(STM8L15X_LD) || defined(STM8L15X_HD) //STM8L
    if (FLASH_ReadByte(0x480b) != 0x00)
        FLASH_ProgramOptionByte(0x480b, 0x00);
    if (FLASH_ReadByte(0x480c) != 0x00)
        FLASH_ProgramOptionByte(0x480c, 0x00);
#else //STM8S
    if (FLASH_ReadByte(0x487e) != 0x00)
        FLASH_ProgramOptionByte(0x487e, 0x00);
    if (FLASH_ReadByte(0x487f) != 0x00)
        FLASH_ProgramOptionByte(0x487f, 0x00);
#endif
    FLASH_Lock(FLASH_MemType_Data);

    FLASH_ReadByte(0x487e);
}

/**
 ****************************************************************************
 * @Function : void OTA_start(void)
 * @File     : eland_ota.c
 * @Program  : none
 * @Created  : 2018/1/4 by seblee
 * @Brief    : star reset mcu state
 * @Version  : V1.0
**/
void OTA_start(void)
{
    MCU_RESET_STATE();
    OTA_bootloader_enable();
    asm("jp 0x601F"); // jump to given entry point address
}

/**
 ****************************************************************************
 * @Function : void MCU_RESET_STATE(void)
 * @File     : eland_ota.c
 * @Program  : none
 * @Created  : 2018/1/4 by seblee
 * @Brief    : reset state
 * @Version  : V1.0
**/
void MCU_RESET_STATE(void)
{
    IWDG_ReloadCounter();
    disableInterrupts();
    TIM1_DeInit();
    TIM2_DeInit();
    TIM3_DeInit();
    TIM4_DeInit();
    TIM5_DeInit();
    USART_DeInit(USART1);
    USART_DeInit(USART2);
    USART_DeInit(USART3);
    GPIO_DeInit(GPIOA);
    GPIO_DeInit(GPIOB);
    GPIO_DeInit(GPIOC);
    GPIO_DeInit(GPIOD);
}
