#include <REG52.H>         //包含51系列单片机的头文件
#include <intrins.h>       //包含_crol_()循环左移位函数
void delay(unsigned int i) //延时函数，delay(1)；约延时0.1ms
{
    while (i--)
        ;
}
void main()
{
    P1 = 0xff; //初始全部熄灭
    delay(1000);
    P1 = 0xfe; //让第一个LED灯亮
    while (1)
    {
        /*判断是否第1个灯亮，然后执行*/
        if (P1 == 0xfe)
        {
            while (1)
            {
                delay(30000);
                P1 = _crol_(P1, 1); //执行一次，则P1从1111 1110变成1111 1101
                if (P1 == 0x7f)
                    break; //当第8个灯亮时，跳出循环
            }
        }
        /*判断是否第8个灯亮，然后执行*/
        if (P1 == 0x7f)
        {
            while (1)
            {
                delay(30000);
                P1 = _cror_(P1, 1); //执行一次，则P1从0111 1111变成1011 1111
                if (P1 == 0xfe)
                    break; //当第1个灯亮时，跳出循环
            }
        }
    }
}