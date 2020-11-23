//任务8-1可调光台灯
//功能：利用PCF8591芯片进行DA转换，输出模拟电压控制台灯的亮度
#include <reg51.h>
#include <INTRINS.H>
sbit SDA = P2 ^ 7; //P2.7定义I2C数据线引脚
sbit SCL = P2 ^ 6; //P2.6定义I2C时钟线引脚
sbit S1 = P1 ^ 1;  //P1.1控制按键，亮度增加
sbit S2 = P1 ^ 0;  //P1.0控制按键，亮度减小
