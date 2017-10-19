#ifndef __STM8l052R8_DG15069_H_
#define __STM8l052R8_DG15069_H_

#include "stm8l15x_lcd.h"
/*
//���Ϸ�ͨ���Ŷ�ӦRAM����ַ
typedef enum
{
    LCD_NUM_1_RAM = LCD->RAM[LCD_RAMRegister_0],
    LCD_NUM_2_RAM = LCD->RAM[LCD_RAMRegister_],
    LCD_NUM_3_RAM = LCD->RAM[LCD_RAMRegister_],
    LCD_NUM_4_RAM = LCD->RAM[LCD_RAMRegister_],
    LCD_NUM_5_RAM = LCD->RAM[LCD_RAMRegister_],
    LCD_NUM_6_RAM = LCD->RAM[LCD_RAMRegister_],
    LCD_NUM_7_RAM = LCD->RAM[LCD_RAMRegister_],
    LCD_NUM_8_RAM = LCD->RAM[LCD_RAMRegister_0]
}LCD_NUM_1_8_RAM_TypeDef;
//���Ϸ�ͨ���Ŷ�����λ��ַ������"1"�� LCD_NUM_1_RAM �ĵ�0x04λ��
typedef enum
{
    LCD_NUM_1_BIT  = 0x04, 
    LCD_NUM_2_BIT  = 0x04,
    LCD_NUM_3_BIT  = 0x04,
    LCD_NUM_4_BIT  = 0x04,
    LCD_NUM_5_BIT  = 0x40,
    LCD_NUM_6_BIT  = 0x40,
    LCD_NUM_7_BIT  = 0x40,
    LCD_NUM_8_BIT  = 0x40
}LCD_NUM_1_8_BIT_TypeDef;


//����ָʾ��ӦRAM����ַ
typedef enum
{
    LCD_FIRE_T1_RAM  = LCD->RAM[LCD_RAMRegister_0] 
    LCD_FIRE_T2_RAM  = LCD->RAM[LCD_RAMRegister_]
    LCD_FIRE_T3_RAM  = LCD->RAM[LCD_RAMRegister_]
    LCD_FIRE_T4_RAM  = LCD->RAM[LCD_RAMRegister_]
    LCD_FIRE_T5_RAM  = LCD->RAM[LCD_RAMRegister_]
    LCD_FIRE_T6_RAM  = LCD->RAM[LCD_RAMRegister_]
    LCD_FIRE_T7_RAM  = LCD->RAM[LCD_RAMRegister_]
    LCD_FIRE_T8_RAM  = LCD->RAM[LCD_RAMRegister_0]
}LCD_FIRE_T1_T8_RAM_TypeDef;
//����ָʾ����λ��ַ
typedef enum
{
    LCD_FIRE_T1_BIT  = 0x02, 
    LCD_FIRE_T2_BIT  = 0x02,
    LCD_FIRE_T3_BIT  = 0x02,
    LCD_FIRE_T4_BIT  = 0x02,
    LCD_FIRE_T5_BIT  = 0x10,
    LCD_FIRE_T6_BIT  = 0x10,
    LCD_FIRE_T7_BIT  = 0x10,
    LCD_FIRE_T8_BIT  = 0x10   
}LCD_FIRE_T1_T8_BIT_TypeDef;

//����ָʾ��ӦRAM����ַ
typedef enum
{
    LCD_FAULT_T11_RAM  = LCD->RAM[LCD_RAMRegister_0], 
    LCD_FAULT_T12_RAM  = LCD->RAM[LCD_RAMRegister_],
    LCD_FAULT_T13_RAM  = LCD->RAM[LCD_RAMRegister_],
    LCD_FAULT_T14_RAM  = LCD->RAM[LCD_RAMRegister_],
    LCD_FAULT_T15_RAM  = LCD->RAM[LCD_RAMRegister_],
    LCD_FAULT_T16_RAM  = LCD->RAM[LCD_RAMRegister_],
    LCD_FAULT_T17_RAM  = LCD->RAM[LCD_RAMRegister_],
    LCD_FAULT_T18_RAM  = LCD->RAM[LCD_RAMRegister_0]   
}LCD_FAULT_T11_T18_RAM_TypeDef;
//����ָʾ����λ��ַ
typedef enum
{
    LCD_FAULT_T11_BIT  = 0x01, 
    LCD_FAULT_T12_BIT  = 0x01,
    LCD_FAULT_T13_BIT  = 0x01,
    LCD_FAULT_T14_BIT  = 0x01,
    LCD_FAULT_T15_BIT  = 0x08,
    LCD_FAULT_T16_BIT  = 0x08,
    LCD_FAULT_T17_BIT  = 0x08,
    LCD_FAULT_T18_BIT  = 0x08   
}LCD_FAULT_T11_T18_BIT_TypeDef;

*/


//���Ϸ�ͨ���Ŷ�ӦRAM����ַ
#define LCD_NUM_1_RAM   LCD->RAM[LCD_RAMRegister_0] 
#define LCD_NUM_2_RAM   LCD->RAM[LCD_RAMRegister_]
#define LCD_NUM_3_RAM   LCD->RAM[LCD_RAMRegister_]
#define LCD_NUM_4_RAM   LCD->RAM[LCD_RAMRegister_]
#define LCD_NUM_5_RAM   LCD->RAM[LCD_RAMRegister_]
#define LCD_NUM_6_RAM   LCD->RAM[LCD_RAMRegister_]
#define LCD_NUM_7_RAM   LCD->RAM[LCD_RAMRegister_]
#define LCD_NUM_8_RAM   LCD->RAM[LCD_RAMRegister_0]
//���Ϸ�ͨ���Ŷ�����λ��ַ
#define LCD_NUM_1_BIT  0x04
#define LCD_NUM_2_BIT  0x04
#define LCD_NUM_3_BIT  0x04
#define LCD_NUM_4_BIT  0x04
#define LCD_NUM_5_BIT  0x40
#define LCD_NUM_6_BIT  0x40
#define LCD_NUM_7_BIT  0x40
#define LCD_NUM_8_BIT  0x40

//����ָʾ��ӦRAM����ַ
#define LCD_FIRE_T1_RAM   LCD->RAM[LCD_RAMRegister_0] 
#define LCD_FIRE_T2_RAM   LCD->RAM[LCD_RAMRegister_]
#define LCD_FIRE_T3_RAM   LCD->RAM[LCD_RAMRegister_]
#define LCD_FIRE_T4_RAM   LCD->RAM[LCD_RAMRegister_]
#define LCD_FIRE_T5_RAM   LCD->RAM[LCD_RAMRegister_]
#define LCD_FIRE_T6_RAM   LCD->RAM[LCD_RAMRegister_]
#define LCD_FIRE_T7_RAM   LCD->RAM[LCD_RAMRegister_]
#define LCD_FIRE_T8_RAM   LCD->RAM[LCD_RAMRegister_0]
//����ָʾ����λ��ַ
#define LCD_FIRE_T1_BIT   0x02 
#define LCD_FIRE_T2_BIT   0x02
#define LCD_FIRE_T3_BIT   0x02
#define LCD_FIRE_T4_BIT   0x02
#define LCD_FIRE_T5_BIT   0x10
#define LCD_FIRE_T6_BIT   0x10
#define LCD_FIRE_T7_BIT   0x10
#define LCD_FIRE_T8_BIT   0x10 
//����ָʾ��ӦRAM����ַ
#define LCD_FAULT_TT1_RAM   LCD->RAM[LCD_RAMRegister_0] 
#define LCD_FAULT_TT2_RAM   LCD->RAM[LCD_RAMRegister_]
#define LCD_FAULT_TT3_RAM   LCD->RAM[LCD_RAMRegister_]
#define LCD_FAULT_TT4_RAM   LCD->RAM[LCD_RAMRegister_]
#define LCD_FAULT_TT5_RAM   LCD->RAM[LCD_RAMRegister_]
#define LCD_FAULT_TT6_RAM   LCD->RAM[LCD_RAMRegister_]
#define LCD_FAULT_TT7_RAM   LCD->RAM[LCD_RAMRegister_]
#define LCD_FAULT_TT8_RAM   LCD->RAM[LCD_RAMRegister_0]
//����ָʾ����λ��ַ
#define LCD_FAULT_T11_BIT  0x01 
#define LCD_FAULT_T12_BIT  0x01
#define LCD_FAULT_T13_BIT  0x01
#define LCD_FAULT_T14_BIT  0x01
#define LCD_FAULT_T15_BIT  0x08
#define LCD_FAULT_T16_BIT  0x08
#define LCD_FAULT_T17_BIT  0x08
#define LCD_FAULT_T18_BIT  0x08 

/*
    ����ָʾ �������ƣ������ר�����ú���
*/
//����"��ַ"��ӦRAM����ַ
#define LCD_ADD_HZ_T9_RAM   LCD->RAM[LCD_RAMRegister_]

//����"��ֵ"��ӦRAM����ַ
#define LCD_VPT_HZ_T10_RAM   LCD->RAM[LCD_RAMRegister_0]

//����"���ϱ���"��ӦRAM����ַ
#define LCD_FAULT_FIRE_HZ_T19_RAM   LCD->RAM[LCD_RAMRegister_1] 

//�¶ȵ�λ"��"��ӦRAM����ַ
#define LCD_TEMP_UNIT_T20_RAM   LCD->RAM[LCD_RAMRegister_]

//��ѹ��λ"V"��ӦRAM����ַ
#define LCD_V_UNIT_T21_RAM   LCD->RAM[LCD_RAMRegister_]

//������λ"A"��ӦRAM����ַ
#define LCD_A_UNIT_T23_RAM   LCD->RAM[LCD_RAMRegister_0]

//������λ"m"��ӦRAM����ַ
#define LCD_M_UNIT_T22_RAM   LCD->RAM[LCD_RAMRegister_]



#define LCD_FIRE_SET(N)
#define LCD_FIRE_CLEAR(N)
#define LCD_FAULT_SET(N) 
#define LCD_FAULT_CLEAR(N) 


/*���ܺ�������*/
void DG15069Init(void);//��ʽҺ����ʼ��
void ShowLcdAll(void);//����LCD��������
void ShowNum(u8 Num);  //��ʾ�Ϸ�ͨ������Ϣ
void ClearNum(u8 Num);  //����Ϸ�ĳͨ������Ϣ
void ShowFire(u8 fireNum);  //��ʾĳͨ������Ϣ
void ClearFire(u8 fireNum);  //���ĳͨ������Ϣ
void ShowFault(u8 faultNum);//��ʾĳͨ��������Ϣ
void ClearFault(u8 faultNum);//���ĳͨ��������Ϣ


void ShowData(u8 chDat,u8 dat);//������ʾͨ���ź�����
void SetParameter(void);//���ò������棬5S���Զ��˳�
void SetADD(void);//���õ�ַ����
void SetTempVPT(void);//�����¶���ֵ����
void SetLeakVPT(void);//����©������ֵ����




#endif
