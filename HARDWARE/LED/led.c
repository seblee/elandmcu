#include "led.h"

void LedInit(void)//LED��ʼ��
{
    //PG0-2��������ߵ�ƽ
    GPIO_Init(KEY_LED1_GPIO_TypeDef, KEY_LED1_GPIO_PIN, GPIO_Mode_Out_PP_High_Slow);
    GPIO_Init(KEY_LED2_GPIO_TypeDef, KEY_LED2_GPIO_PIN, GPIO_Mode_Out_PP_High_Slow);
    GPIO_Init(KEY_LED3_GPIO_TypeDef, KEY_LED3_GPIO_PIN, GPIO_Mode_Out_PP_High_Slow);        
}

void LedDispose()//LED����
{
    GPIO_ResetBits(GPIOG,GPIO_Pin_0);
    GPIO_ResetBits(GPIOG,GPIO_Pin_1); 
    GPIO_ResetBits(GPIOG,GPIO_Pin_2); 
   // GPIO_SetBits(GPIOG,GPIO_Pin_0);
    
}