
# 工程目的

此工程用於做，網絡鬧鐘LCD部分程序

# v01.29
2018年8月6日16:55:44
## 
* set version v01.29

# v01.28
2018年8月6日16:55:44
## 
* set version v01.28

# v01.27
2018年8月6日16:55:44
## 
* set version v01.27

# v01.26
2018年7月20日14:34:03
## 
* add Today_Second __no_init
* ELAND_CLOCK_MON ELAND_CLOCK_ALARM --> now_is_night = false
* ELAND_AP Set_Brightness(100)
 

# v01.25
2018年7月13日13:43:23
## 
    issue time reset when asm("jp 0x8000")
    change alarm_off Key_Restain to 4s


# v01.15
2018年4月28日15:00:25
## change 3.3V lcd 

# v01.14
2018年4月28日14:19:34
## 
    1 issue NA LED color
    2 alarm number = 0 clear alarm display;
    3 add led bright
    Key_Short_Restain 1s;
    4 default led bright
    OTA display;
    5 simple clock 1 minute set time;
    6 simple clock time&alarm set long press 1s;
    7 add eland_error operation
    8 add eland data read;
    9 restart read alarm;
    10 iwdg change to IWDG_Prescaler_256;


# HX1623 驅動
2017年12月6日10:11:12
LCD換成1/4 bias 1/8 duty
增加驅動芯片 HT1623

