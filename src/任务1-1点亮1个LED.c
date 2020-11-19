#include <reg51.h> //包含51单片机的头文件，或者reg52.h
void main()
{
    P1=0xfe; // 表示P1.0=0，由于硬件接线该LED为低电平导通，所以点亮LED
}

/*第2种写法
sbit LED=P1^0; //特殊功能位定义，用LED这个英文名代替P1.0引脚
void main()
{
    LED=0; // 表示P1.0=0，由于硬件接线该LED为低电平导通，所以点亮LED
}
*/