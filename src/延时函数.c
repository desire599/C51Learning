#include <REG52.H>
/*
void delay(unsigned int i)
{
    while (i--);
}
*/
void delay(unsigned int time_ms)
{
    unsigned char i, j;
    for (i = 0; i < time_ms;i++)	
        for (j = 0; j < 123;j++);
}
void main()
{
    while (1)
    {
        P1 = 0xfe;
        delay(100);
        P1 = 0xFF;
        delay(100);
    }
    
}