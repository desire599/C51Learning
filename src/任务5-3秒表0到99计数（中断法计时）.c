﻿//程序：ex5_1.c
//功能：00～99的简易秒表设计，两个静态数码管，定时器采用中断方式
#include "reg51.h" //包含头文件reg51.h，定义了51单片机的专用寄存器
//全局变量定义
unsigned char count = 0; //对50ms定时时间进行计数
unsigned char miao = 0;  //秒计数器

//函数名：timer_1()
//函数功能：定时器T1中断函数，假设T1在工作方式1下每50ms产生中断，执行该中断函数
//形式参数：无
//返回值：无
void timer_1() interrupt 3 //（5）编写中断服务程序，T1的中断类型号为3
{
  count++;                     //50ms计数器加1
  TH1 = (65536 - 50000) / 256; //重新设置T1计数初值高8位，这样定时器又可以开始重新计时50ms
  TL1 = (65536 - 50000) % 256; //重新设置T1计数初值低8位
  /*由于采用中断法定时时是每隔50ms自动进入这个函数，不能像查询法那样写好时间，
  因此只有通过统计进入中断函数的次数count，才知道定时的时间，因此使用miao变量来计时*/
  if (count == 20) //1s时间到
  {
    count = 0; //50ms计数器清0
    miao++;    //秒计数器加1
    if (miao == 100)
      miao = 0; //miao计数到100，则从0开始计数
  }
}

//函数名：disp(unsigned char i)
//函数功能：将i的值显示在两个静态连接的数码管上
//形式参数：i，取值范围0～99
//返回值：无
void disp(unsigned char i)
{
  unsigned char led[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};
  //定义0～9显示码，共阳极数码管
  P1 = led[i / 10]; //显示i高位，商放在左边第一个LED
  P2 = led[i % 10]; //显示i低位，取余数放在第二个LED
}

void main() //主函数
{
  TMOD = 0x10;                 //（1）设置T1为工作方式1
  TH1 = (65536 - 50000) / 256; //（2）设置T1计数初值高8位，定时时间50ms
  TL1 = (65536 - 50000) % 256; //（2）设置T1计数初值低8位
  EA = 1;                      //（3）开放总中断允许
  ET1 = 1;                     //（3）开放定时器T1中断允许
  TR1 = 1;                     //（4）启动T1开始计数
  while (1)
  {
    disp(miao); //显示秒计数器值
  }
}
