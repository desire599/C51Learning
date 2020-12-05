#include <reg51.h>   //包含头文件reg51.h，定义了51单片机的专用寄存器
#include <intrins.h> //包含头文件intrins.h，代码中引用了_nop_()函数
#include "lcd.h"  //包含lcd.h头文件
//定义控制信号端口
sbit RS = P3 ^ 0; //RS是数据和命令选择位，其中0xb0是P3.0在单片机中的位地址，也可写成RS=0xb0;
sbit RW = P3 ^ 1; //RW是写和读选择位
sbit E = P3 ^ 2;  //E是LCD使能位
void delay_3us();    //延时函数声明
void delay(unsigned int i); //延时函数声明
//4、函数名：lcd_int
//函数功能：lcd初始化
//形式参数：无
//返回值：无
void lcd_int()
{
    lcd_w_cmd(0x3c);        //设置工作方式，设置8位数据总线，双行显示，5X10的点阵字符
    lcd_w_cmd(0x0c);        //设置显示状态，显示打开，无光标显示，光标不闪烁
    lcd_w_cmd(0x06);        //设置输入方式，字体增量右移
    lcd_w_cmd(0x01);        //清屏必须要放在前面的设置之后
    lcd_w_cmd(0x00 + 0x80); //设置字符的初始显示位置在第一行第1列
}
//1、函数名：lcd_r_start
//函数功能：读状态字
//形式参数：无
//返回值：返回状态字，最高位D7=0，LCD控制器空闲；D7=1,LCD控制器忙
unsigned char lcd_r_start()
{
    unsigned char s;
    RW = 1;      //当RW=1，RS=0时，为读取LCD的命令状态
    delay_3us(); //由于单片机程序执行速度比LCD的操作速度快，每个语句后面都要延时几微秒。
    RS = 0;
    delay_3us();
    E = 1; //E端时序=1
    delay_3us();
    s = P1; //从LCD的数据口P0读状态，存入s
    delay_3us();
    E = 0; //E端时序=0，产生下降沿时，LCD执行命令
    delay_3us();
    RW = 0;
    delay_3us();
    return (s); //返回读取的LCD状态字
}
//2、函数名：lcd_w_cmd
//函数功能：写命令字
//形式参数：命令字已存入com单元中
//返回值：无
void lcd_w_cmd(unsigned char com)
{
    unsigned char i;
    do
    {                      //查LCD忙操作
        i = lcd_r_start(); //调用读状态字函数的函数，把返回值存入i
        i = i & 0x80;      //"与"操作屏蔽掉i的低7位，如果i值的最高位为1，则表示LCD正在忙，此时不能做其他操作
        delay(2);
    } while (i != 0); //如果i值=1，继续查询；如果i值为0，则退出循环
    RW = 0;           //如果前面LCD已经空闲，则可以进行写命令的操作。
    delay_3us();
    RS = 0; //RW=0（写），RS=0（命令），设置写LCD命令字
    delay_3us();
    E = 1; //E端时序=1
    delay_3us();
    P1 = com; //将com中的命令字写入LCD数据口
    delay_3us();
    E = 0; //E端=0，产生下降沿时，LCD执行命令
    delay_3us();
}
//3、函数名：lcd_w_dat
//函数功能：写数据
//形式参数：数据已存入dat单元中
//返回值：无
void lcd_w_dat(unsigned char dat)
{
    unsigned char i;
    do
    {                      //查忙操作
        i = lcd_r_start(); //调用读状态字函数
        i = i & 0x80;      //"与"操作屏蔽掉低7位
        delay(2);
    } while (i != 0); //LCD忙，继续查询，否则退出循环
    RW = 0;
    delay_3us();
    RS = 1; //RW=0，RS=1，写LCD命令字
    delay_3us();
    E = 1; //E端时序
    delay_3us();
    P1 = dat; //将dat中的显示数据写入LCD数据口
    delay_3us();
    E = 0;
    delay_3us();
}
//函数名：delay_3us
//函数功能：采用软件实现延时，约3个机器周期
//形式参数：无
//返回值：无
void delay_3us()
{
    _nop_();
    _nop_();
    _nop_();
}

//函数名：delay
//函数功能：实现软件延时
//形式参数：无符号整型变量i，控制空循环的循环次数
//返回值：无
void delay(unsigned int i) //延时函数
{
    while (i--)
        ;
}