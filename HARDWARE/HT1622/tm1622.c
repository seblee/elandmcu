/*****************************************************************************
 *版权信息：深圳天微电子有限公司
 *文 件 名：TM1622-V1.0
 *当前版本：V1.0
 *MCU 型号：STC12C5608AD
 *开发环境：Keil uVision4
 *晶震频率：11.0592MHZ       
 *完成日期：2013-09-09
 *程序功能：1.LCD驱动：32SEG*8COM(由2组4COM LCD屏组成），分别显示0~9.
 *免责声明：1.此程序为TM1622驱动LCD演示程序，仅作参考之用。
            2.如有直接使用本例程程序造成经济损失的，本公司不承担任何责任             
********************************************************************************/
#include <reg52.h>   //MCU头文件
#include "intrins.h" //包含nop指令头文件

#define nop() \
    _nop_();  \
    _nop_();  \
    _nop_();  \
    _nop_();  \
    _nop_(); //宏定义

/********************定义控制端口**********************/
sbit CS = P2 ^ 1;    //片选
sbit READ = P2 ^ 0;  //读数据时钟
sbit WRITE = P1 ^ 7; //写数据时钟
sbit DATA = P1 ^ 6;  //数据口

//****定义数据*************** 0    1    2   3     4    5    6    7    8    9    0    1   2    3    4    5     6   7    8    9    0     1   2    3    4     5
unsigned char code date0[] = {
    0x0B,
    0x00,
    0x07,
    0x05,
    0x0C,
    0x0D,
    0x0F,
    0x00,
    0x0F,
    0x0D,
    0x0B,
    0x00,
    0x07,
    0x05,
    0x0C,
    0x0D,
    0x0F,
    0x00,
    0x0F,
    0x0D,
    0x0B,
    0x00,
    0x07,
    0x05,
    0x0C,
    0x0D,
};
unsigned char code date1[] = {
    0x0E,
    0x06,
    0x0C,
    0x0e,
    0x06,
    0x0A,
    0x0A,
    0x0E,
    0x0E,
    0x0E,
    0x0E,
    0x06,
    0x0C,
    0x0e,
    0x06,
    0x0A,
    0x0A,
    0x0E,
    0x0E,
    0x0E,
    0x0E,
    0x06,
    0x0C,
    0x0e,
    0x06,
    0x0A,
};
unsigned char Buffer[64]; //定义数组

/***************延时函数nms**************/
void delay_nms(unsigned int n)
{
    unsigned int i;
    while (n--)
    {
        for (i = 0; i < 550; i++)
            ;
    }
}

/***************写100命令函数**************/
void write_cmd_100(unsigned char cmd)
{
    unsigned char i;

    CS = 0;
    WRITE = 0;
    nop();
    DATA = 1;
    nop();
    WRITE = 1;
    nop();

    WRITE = 0;
    nop();
    DATA = 0;
    nop();
    WRITE = 1;
    nop();

    WRITE = 0;
    nop();
    DATA = 0;
    nop();
    WRITE = 1;
    nop();

    for (i = 0; i < 8; i++)
    {
        WRITE = 0;
        if (cmd & 0x80)
            DATA = 1;
        else
            DATA = 0;
        WRITE = 1;
        cmd <<= 1;
    }

    WRITE = 0;
    nop();
    DATA = 0;
    nop();
    WRITE = 1;
    nop();
    WRITE = 0;
    nop();
    CS = 1;
}

/***************写101数据函数**************/
void write_101(unsigned char address, unsigned char dat)
{

    unsigned char i, add;
    add = address << 2;

    CS = 0;
    WRITE = 0;
    nop();
    DATA = 1;
    nop();
    WRITE = 1;
    nop();

    WRITE = 0;
    nop();
    DATA = 0;
    nop();
    WRITE = 1;
    nop();

    WRITE = 0;
    nop();
    DATA = 1;
    nop();
    WRITE = 1;
    nop();

    for (i = 0; i < 6; i++)
    {
        WRITE = 0;
        if (add & 0x80)
            DATA = 1;
        else
            DATA = 0;
        WRITE = 1;
        add <<= 1;
    }

    for (i = 0; i < 4; i++)
    {
        WRITE = 0;
        if (dat & 0x01)
            DATA = 1;
        else
            DATA = 0;
        WRITE = 1;
        dat >>= 1;
    }
    WRITE = 0;
    DATA = 0;
    CS = 1;
}

/***************向显存写数据**************/
void writeDDRAM(void)
{
    unsigned char i;
    for (i = 0; i < 16; i++) //向64个地址写数据，显示0~9
    {
        write_101((2 * (2 * i)), date0[i]);
        write_101((2 * (2 * i) + 2), date1[i]);

        write_101((2 * (2 * i) + 1), date0[i]);
        write_101((2 * (2 * i) + 3), date1[i]);
    }
}

/***************TM1622初始化函数**************/
void init_tm1622()
{
    write_cmd_100(0x01); //打开系统振荡器
    write_cmd_100(0x03); //开启LCD显示
    write_cmd_100(0x18); //片内RC 32KHZ
    write_cmd_100(0xe3); //正常模式
}

void main()
{
    unsigned char i;
    init_tm1622(); //初始化
    nop();
    nop();

    for (i = 0; i < 64; i++) //发送全0，清显存
    {
        Buffer[i] = 0x00;
        write_101(i, Buffer[i]);
    }

    while (1)
    {
        writeDDRAM(); //显示0~9
        delay_nms(200);
    }
}
