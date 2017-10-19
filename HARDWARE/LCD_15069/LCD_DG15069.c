#include "LCD_DG15069.h"
/*
const u8 NumberMap[10]=
{
    // 0   1    2   3    4    5    6    7     8    9  
    0x5F,0x42,0xF5,0x67,0xEa,0xAF,0xBF,0x046,0xFF,0xEF
};
*/

/*���ܺ���ʵ��*/
void DG15069Init(void)//��ʽҺ����ʼ��
{
    LCD_DeInit();//LCD��ؼĴ����ָ�Ĭ��ֵ  
    /* Enable LCD/RTC clock */
    CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_LCD, ENABLE);
   
    #ifdef USE_LSE
    CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1);
    #else
    CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_1);
    #endif
   
    /* Initialize the LCD */
    LCD_Init(LCD_Prescaler_1, LCD_Divider_31, LCD_Duty_1_4, 
                           LCD_Bias_1_3, LCD_VoltageSource_External);
    
    /* Mask register
    For declare the segements used.
    in the Discovery we use 0 to 15 segments. */
    LCD_PortMaskConfig(LCD_PortMaskRegister_0, 0xFF);
    LCD_PortMaskConfig(LCD_PortMaskRegister_1, 0xFF);
   // LCD_PortMaskConfig(LCD_PortMaskRegister_2, 0xff);
    
    /* To set contrast to mean value */
   // LCD_ContrastConfig(LCD_Contrast_3V0);
    LCD_ContrastConfig(LCD_Contrast_Level_4);
    
    LCD_DeadTimeConfig(LCD_DeadTime_0);
    LCD_PulseOnDurationConfig(LCD_PulseOnDuration_1);
    
    /* Enable LCD peripheral */ 
    LCD_Cmd(ENABLE);
}
/*����LCD��������*/
void ShowLcdAll(void)
{
    
    LCD_WriteRAM(LCD_RAMRegister_0,0xff);

    LCD_WriteRAM(LCD_RAMRegister_1,0xff);

    LCD_WriteRAM(LCD_RAMRegister_3,0xff);
    LCD_WriteRAM(LCD_RAMRegister_4,0xff); 
    LCD_WriteRAM(LCD_RAMRegister_5,0xff);
    LCD_WriteRAM(LCD_RAMRegister_7,0xff);
    LCD_WriteRAM(LCD_RAMRegister_8,0xff); 
    LCD_WriteRAM(LCD_RAMRegister_10,0xff);
    LCD_WriteRAM(LCD_RAMRegister_11,0xff);
    LCD_WriteRAM(LCD_RAMRegister_12,0xff); 
}
/*
    ���ܣ�������ʾ�Ϸ�ͨ������Ϣ
    ������ͨ���� 1-8 
*/
void ShowNum(u8 Num)  //��ʾ�Ϸ�ͨ������Ϣ
{
    if((8<Num)||(1>Num))//ͨ�����жϣ�������Χ��ֱ�ӷ���
        return;
    switch(Num)
    {
    case 1:
        LCD->RAM[LCD_RAMRegister_0] = LCD->RAM[LCD_RAMRegister_0]||LCD_NUM_1_BIT;
        break; 
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    case 6:
        break;
    case 7:
        break;
    case 8:
        break;
    default:
        break;
    }
  
}
/*
    ���ܣ���������Ϸ�ͨ������Ϣ
    ������ͨ���� 1-8 
*/
void ClearNum(u8 Num)  //����Ϸ�ĳͨ������Ϣ
{
    if((8<Num)||(1>Num))//ͨ�����жϣ�������Χ��ֱ�ӷ���
        return;
    switch(Num)
    {
    case 1:
        break; 
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    case 6:
        break;
    case 7:
        break;
    case 8:
        break;
    default:
        break;
    }
}

/*
    ���ܣ�������ʾĳͨ���Ĺ�����Ϣ
    ������ͨ���� 1-8 
*/
void ShowFault(u8 faultNum)//��ʾĳͨ��������Ϣ
{
    if((8<faultNum)||(1>faultNum))//ͨ�����жϣ�������Χ��ֱ�ӷ���
        return;
    switch(faultNum)
    {
    case 1:
        
        break; 
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    case 6:
        break;
    case 7:
        break;
    case 8:
        break;
    default:
        break;
    }
    
}
/*
    ���ܣ��������ĳͨ���Ĺ�����Ϣ
    ������ͨ���� 1-8 
*/
void ClearFault(u8 faultNum)//���ĳͨ��������Ϣ
{
    if((8<faultNum)||(1>faultNum))//ͨ�����жϣ�������Χ��ֱ�ӷ���
        return;
    switch(faultNum)
    {
    case 1:
        break; 
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    case 6:
        break;
    case 7:
        break;
    case 8:
        break;
    default:
        break;
    }
}
/*
    ���ܣ�������ʾĳͨ���Ļ���Ϣ
    ������ͨ���� 1-8 
*/
void ShowFire(u8 fireNum)  //��ʾĳͨ������Ϣ
{
    if((8<fireNum)||(1>fireNum))//ͨ�����жϣ�������Χ��ֱ�ӷ���
        return;
    switch(fireNum)
    {
    case 1:
        break; 
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    case 6:
        break;
    case 7:
        break;
    case 8:
        break;
    default:
        break;
    }

}
/*
    ���ܣ��������ĳͨ���Ļ���Ϣ
    ������ͨ���� 1-8 
*/
void ClearFire(u8 fireNum)  //���ĳͨ������Ϣ
{
    if((8<fireNum)||(1>fireNum))//ͨ�����жϣ�������Χ��ֱ�ӷ���
        return;
    switch(fireNum)
    {
    case 1:
        break; 
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    case 6:
        break;
    case 7:
        break;
    case 8:
        break;
    default:
        break;
    }
}