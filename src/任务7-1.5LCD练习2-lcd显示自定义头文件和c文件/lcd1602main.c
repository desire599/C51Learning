#include <reg51.h>   //包含头文件reg51.h，定义了51单片机的专用寄存器
#include "lcd1602.h" //因为lcd在项目文件夹内，用双引号表示先从文件夹搜索。
//包含lcd1602.h，从而把lcd1602.c包含进来
void main()                        //主函数
{
    unsigned char lcd1[] = "QING YUAN";  //字符数组可以用“”来存入整个字符串，其中字符数组的最后一位为'\0'，由系统自动加上。
    unsigned char lcd2[] = "GUANG DONG"; //字符数组可以用“”来存入整个字符串，其中字符数组的最后一位为'\0'，由系统自动加上。
    unsigned char i;
    P1 = 0xff; //送全1到P1口
    lcd_int(); //调用LCD初始化函数
    delay(255);
    lcd_w_cmd(0x03 + 0x80); //设置起始显示位置在第一行第3列
    delay(255);
    for (i = 0; lcd1[i] != '\0'; i++) //显示字符串，当字符串结束符为'\0'时，则跳出for循环。
    {
        lcd_w_dat(lcd1[i]);
        delay(200);
    }
    lcd_w_cmd(0x45 + 0x80); //设置起始显示位置在第2行第5列
    delay(255);
    for (i = 0; lcd2[i] != '\0'; i++) //显示字符串，当字符串结束符为'\0'时，则跳出for循环。
    {
        lcd_w_dat(lcd2[i]);
        delay(200);
    }
    while (1)
        ; //原地踏步，待机命令
}

