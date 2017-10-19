#ifndef __LED_H_
#define __LED_H_
#include "stm8l15x_gpio.h"

//LED�� "LED1"�˿�λ�ö���
#define KEY_LED1_GPIO_TypeDef   GPIOG
#define KEY_LED1_GPIO_PIN       GPIO_Pin_0
//���� "LED2"�˿�λ�ö���
#define KEY_LED2_GPIO_TypeDef   GPIOG
#define KEY_LED2_GPIO_PIN       GPIO_Pin_1
//���� "LED3"�˿�λ�ö���
#define KEY_LED3_GPIO_TypeDef   GPIOG
#define KEY_LED3_GPIO_PIN       GPIO_Pin_2

#define LED1 0x01
#define LED2 0x02
#define LED3 0x03


void LedInit(void);//LED��ʼ��
void LedDispose(void);//LED����

#endif