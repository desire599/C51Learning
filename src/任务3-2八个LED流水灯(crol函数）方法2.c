#include <REG52.H>         //包含51系列单片机的头文件
#include <intrins.h>       //包含_crol_()循环左移位函数
void delay(unsigned int i) //延时函数，delay(1)；约延时0.1ms
{
    while (i--)
        ;
}
void main()
{
    unsigned char i; //定义循环变量i
    P1 = 0xff;       //初始全部熄灭
    delay(1000);
    P1 = 0xfe; //让第一个LED灯亮
    while (1)
    {
        /*判断是否第1个灯亮，然后执行*/
        if (P1 == 0xfe)
        {
            for (i = 0; i < 7; i++) //已知流水灯每个循环移动7次
            {
                delay(30000);
                P1 = _crol_(P1, 1); //执行一次，则P1从1111 1110变成1111 1101
            }
        }
        /*判断是否第8个灯亮，然后执行*/
        if (P1 == 0x7f)
        {
            for (i = 0; i < 7; i++) //已知流水灯每个循环移动7次
            {
                delay(30000);
                P1 = _cror_(P1, 1); //执行一次，则P1从0111 1111变成1011 1111
            }
        }
    }
}