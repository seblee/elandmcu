/**
 ****************************************************************************
 * @Warning :Without permission from the author,Not for commercial use
 * @File    :iwdg.c
 * @Author  :Xiaowine
 * @date    :2017/10/20
 * @version :V 1.0.0
 *************************************************
 * @brief   :
 ****************************************************************************
**/
#include "iwdg.h"
/**
 ****************************************************************************
 * @Function : void IWDG_Config(void)
 * @File     : iwdg.c
 * @Program  : none
 * @Created  : 2017/10/20 by Xiaowine
 * @Brief    : iwdg configration
 * @Version  : V1.0
**/
void IWDG_Config(void)
{
    /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
    IWDG_Enable();

    /* IWDG timeout equal to 214 ms (the timeout may varies due to LSI frequency
     dispersion) */
    /* Enable write access to IWDG_PR and IWDG_RLR registers */
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

    /* IWDG configuration: IWDG is clocked by LSI = 38KHz */
    IWDG_SetPrescaler(IWDG_Prescaler_32);

    /* IWDG timeout equal to 214.7 ms (the timeout may varies due to LSI frequency dispersion) */
    /* IWDG timeout = (RELOAD_VALUE + 1) * Prescaler / LSI 
                  = (254 + 1) * 32 / 38 000 
                  = 214.7 ms */
    IWDG_SetReload((uint8_t)0xaa);

    /* Reload IWDG counter */
    IWDG_ReloadCounter();
}
