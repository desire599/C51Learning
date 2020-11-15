#include <REG52.H>
/*第一种写法，调用delay(1000)相当于延时10ms
void delay(unsigned int i)
{
    while (i--);
}
*/

/*第二中写法，调用delay(1000)相当于延时1000ms，即1秒*/
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