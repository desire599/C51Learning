/*任务3-6：从P1.0引脚输出周期为1s，占空比为75%的方波*/
#include <REG52.H>
sbit PWM = P1 ^ 0;//定义方波输出引脚名为PWM
void delay(unsigned int i) //delay(1);表示1ms
{
    unsigned int j, k;
    for (j = 0; j < i; j++)
    {
        for (k = 0; k < 123; k++)
            ;
    }
}
void main()
{
    while (1)
    {
        PWM = 1;
        delay(750);//高电平持续750ms
        PWM = 0;
        delay(250);//低电平持续250ms
    }
}