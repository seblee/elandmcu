#include "key.h"


u8 KeyDispose(void);//�����������ذ���ֵ��
void KeyInit(void);//������ʼ��

u8 KeyFlag;//�������±�־������а������£���Ӧλ��־���������ͷ�ʱ�����ذ���ֵ


//*************���ܺ���ʵ��**************


/*
�������ƣ�������ʼ������
��ڲ�������
���ز�������
*/
void KeyInit(void)//������ʼ��
{
    //�����а���������Ϊ����
    GPIO_Init(KEY_ADD_GPIO_TypeDef, KEY_ADD_GPIO_PIN, GPIO_Mode_In_PU_No_IT);//��ʼ��"ADD"���� KEY1
    GPIO_Init(KEY_VPT_GPIO_TypeDef, KEY_VPT_GPIO_PIN, GPIO_Mode_In_PU_No_IT);//��ʼ��"VPT"���� KEY2
    GPIO_Init(KEY_UP_GPIO_TypeDef, KEY_UP_GPIO_PIN, GPIO_Mode_In_PU_No_IT);//��ʼ��"UP"���� KEY3
    GPIO_Init(KEY_DOWN_GPIO_TypeDef, KEY_DOWN_GPIO_PIN, GPIO_Mode_In_PU_No_IT);//��ʼ��"DOWN"���� KEY4
    GPIO_Init(KEY_SET_GPIO_TypeDef, KEY_SET_GPIO_PIN, GPIO_Mode_In_PU_No_IT);//��ʼ��"SET"���� KEY5
    GPIO_Init(KEY_OK_GPIO_TypeDef, KEY_OK_GPIO_PIN, GPIO_Mode_In_PU_No_IT);//��ʼ��"OK"���� KEY6
}

u8 KeyDispose(void)//�����������ذ���ֵ��
{   
    u8 keyNum = KEY_NO; //������ֵ��ʱ����
    if(!GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_4))//��ȡIO�ڵ�ƽ���жϲ�����
    {
        if(!KeyFlag&&KEY1_Down_FG_SET)//֮ǰû�а���
        {
            KeyFlag = KeyFlag||KEY1_Down_FG_SET;//�����±�־
        }              
    }
    else    //��ȡ���ߵ�ƽ���ж��Ƿ�Ϊ�����ͷ�
    {
        if(KeyFlag&&KEY1_Down_FG_SET)//���֮ǰ�а������±�־�����ڼ��Ϊ�����ͷ�
        {
            KeyFlag = KeyFlag&&KEY1_Down_FG_RST;//�尴�±�־   
            keyNum = KEY_ADD;   //"��ַ"���������� ---KEY1
        }
    }
        
    if(!GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_6))
    {
        keyNum = KEY_VPT;   //"��ֵ"����������---KEY2
    }
    
    if(!GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_5))
    {
        keyNum = KEY_UP;   //"��"����������---KEY3
    }
    
    if(!GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_7))
    {
        keyNum = KEY_DOWN;//"��"����������---KEY4
    }
    
    
    if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1))
    {
        keyNum = KEY_SET;//"����"����������---KEY5
    }
    
    if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0))
    {
        keyNum = KEY_OK;//"OK"����������---KEY6
    }    
    return keyNum;   
}