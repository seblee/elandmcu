#ifndef __KEY_H_
#define __KEY_H_

#include "stm8l15x.h"
#include "stm8l15x_gpio.h"

//������ADD���˿�λ�ö���
#define KEY_ADD_GPIO_TypeDef GPIOF
#define KEY_ADD_GPIO_PIN GPIO_Pin_4
//������VPT���˿�λ�ö���
#define KEY_VPT_GPIO_TypeDef GPIOF
#define KEY_VPT_GPIO_PIN GPIO_Pin_6
//������UP���˿�λ�ö���
#define KEY_UP_GPIO_TypeDef GPIOF
#define KEY_UP_GPIO_PIN GPIO_Pin_5
//������DOWN���˿�λ�ö���
#define KEY_DOWN_GPIO_TypeDef GPIOF
#define KEY_DOWN_GPIO_PIN GPIO_Pin_7
//������SET���˿�λ�ö���
#define KEY_SET_GPIO_TypeDef GPIOC
#define KEY_SET_GPIO_PIN GPIO_Pin_1
//������OK���˿�λ�ö���
#define KEY_OK_GPIO_TypeDef GPIOC
#define KEY_OK_GPIO_PIN GPIO_Pin_0

#define    KEY1_Down_FG_SET 0x01
#define    KEY2_Down_FG_SET 0x02
#define    KEY3_Down_FG_SET 0x04

#define    KEY4_Down_FG_SET 0x08
#define    KEY5_Down_FG_SET 0x10
#define    KEY6_Down_FG_SET 0x20

#define    KEY1_Down_FG_RST 0xFE
#define    KEY2_Down_FG_RST 0xFD
#define    KEY3_Down_FG_RST 0xFB

#define    KEY4_Down_FG_RST 0xF7
#define    KEY5_Down_FG_RST 0xEF
#define    KEY6_Down_FG_RST 0xDF

//��ֵ����
#define KEY_NO  0x00 //�ް�������
#define KEY_ADD  0x01//��ַ����(KEY1)
#define KEY_VPT  0x02//��ֵ����(KEY2)
#define KEY_UP  0x03//�ϰ���(KEY3)
#define KEY_DOWN  0x04//�°���(KEY4)
#define KEY_SET  0x05//���ð���(KEY5)
#define KEY_OK  0x06//ȷ�ϰ���(KEY6)

extern u8 KeyFlag;

//���ܺ�������
u8 KeyDispose(void);//�����������ذ���ֵ��
void KeyInit(void);//������ʼ��

#endif