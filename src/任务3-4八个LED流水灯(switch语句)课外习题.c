#include <reg52.h>
sbit k1 = P0 ^ 0;
void delay(unsigned int i)
{
    while(i--)
        ;
}
unsigned char key()
{
/*把局部变量counter写成静态变量类型，则在重复进入key函数时，
counter不会每次进入就重新变成0，而是记住其上一次的数值*/
    static unsigned char counter = 0;
    if(k1==0)
    {
        delay(1000);
        if(k1==0)
        {
            while(!k1);
						delay(1000);
            counter++;
            if(counter==5)
                counter = 0;
        }
    }
    return counter;
}
void main()
{
    P1 = 0xff;
    while(1)
    {
        switch (key())
        {
            case 1:P1=0x00;break;
            case 2:P1=0xf0;break;
            case 3:P1=0x0f;break;
            case 4:while(key()==4){
                    P1 = 0xaa;
                    delay(10000);
                    P1 = 0x55;
                    delay(10000);
                };break;
            default:P1=0xff;break;
        }
    }
}