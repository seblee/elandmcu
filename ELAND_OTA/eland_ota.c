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
static void MCU_RESET_STATE(void);
/* Private functions ---------------------------------------------------------*/

void OTA_bootloader_enable(void)
{
      static uint8_t opt_M[5]  ;
     opt_M[0] = FLASH_ReadByte(0x1000);
     opt_M[1] = FLASH_ReadByte(0x1001);
     opt_M[2] = FLASH_ReadByte(0x1002);
     opt_M[3] = FLASH_ReadByte(0x1003);
     opt_M[4] = FLASH_ReadByte(0x1004);

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

// #if defined(STM8L15X_MD) || defined(STM8L15X_MDP) || defined(STM8L15X_LD) || defined(STM8L15X_HD) //STM8L
// void OTA_bootloader_enable(void)
// {
//     FLASH->DUKR = 0xae;
//     asm("nop");
//     FLASH->DUKR = 0x56; // 解除写保护
//     asm("nop");
//     while (!(FLASH->IAPSR & 0x08))
//         ; // 等待解锁
//     asm("nop");
//     FLASH->CR2 = 0x80; // 对选项字节进行写操作
//     asm("nop");
//     *((unsigned char *)0x480b) = 0x55;
//     asm("nop");
//     *((unsigned char *)0x480c) = 0xaa; // 写入选项字节
// }
// #else //STM8S
// void OTA_bootloader_enable(void)
// {
//     FLASH->DUKR = 0xae;
//     asm("nop");
//     FLASH->DUKR = 0x56;
//     asm("nop");
//     while (!(FLASH->IAPSR & 0x08))
//         ;
//     asm("nop");
//     FLASH->CR2 = 0x80;
//     asm("nop");
//     FLASH->NCR2 = 0x7f;
//     asm("nop");
//     *((unsigned char *)0x487e) = 0x55;
//     asm("nop");
//     *((unsigned char *)0x487f) = 0xaa;
// }
// #endif
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
    asm("jp 0x601F"); // jump to given entry point address
}

/**
 ****************************************************************************
 * @Function : static void MCU_RESET_STATE(void)
 * @File     : eland_ota.c
 * @Program  : none
 * @Created  : 2018/1/4 by seblee
 * @Brief    : reset state
 * @Version  : V1.0
**/
static void MCU_RESET_STATE(void)
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
