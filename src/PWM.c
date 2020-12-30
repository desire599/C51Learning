#include <REG52.H>
sbit PWM = P1 ^ 0;
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
        delay(750);
        PWM = 0;
        delay(250);
    }
}