#include "reg51.h"
void delay(unsigned int i) //自定义延时函数
{
    while (i--) //执行软件空运算，从而实现系统的延时效果
        ;
}

void main()
{
    while (1)
    {
        P1 = 0xff; // 表示P1.0=1，熄灭LED
        delay(1000); //延时约10ms
        P1 = 0xfe; // 表示P1.0=0，点亮LED
        delay(1000);
    }
}

/*第2种写法：led闪烁
sbit led=P1^0;
void main()
{
    while (1S
    {
        led=~led; // 对P1.0的状态每次进行取反，在0和1之间变化，实现闪烁效果
        delay(1000); //延时约10ms
    }
}
*/S