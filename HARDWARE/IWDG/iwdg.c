/**
 ****************************************************************************
 * @Warning :Without permission from the author,Not for commercial use
 * @File    :iwdg.c
 * @Author  :seblee
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
 * @Created  : 2017/10/20 by seblee
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
    IWDG_SetPrescaler(IWDG_Prescaler_256);

    /* IWDG timeout equal to 1717.89 ms (the timeout may varies due to LSI frequency dispersion) */
    /* IWDG timeout = (RELOAD_VALUE + 1) * Prescaler / LSI
                  = (254 + 1) * 256 / 38 000
                  = 1717.89 ms */
    IWDG_SetReload((uint8_t)0xfe);

    /* Reload IWDG counter */
    IWDG_ReloadCounter();
}
