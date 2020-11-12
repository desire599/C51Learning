#include <reg52.h>
int i = 0;
sbit k1 = P0 ^ 0;
void delay(unsigned int i)
{
    while(i--)
        ;
}
int key()
{
    if(k1==0)
    {
        delay(1000);
        if(k1==0)
        {
            while(!k1);
            if(i==0)
            {
                i = 1;
                return 1;
            }
            else
            {
                i = 0;
                return 2;
            }
        }
    }
    return 0;
}
void main()
{
    P1 = 0xff;
    while(1)
    {
        switch (key())
        {
            case 1:while(P1!=0x00){P1 <<= 1;delay(20000);};break;
            case 2:P1=0xff;break;
            default:   break;
        }
    }
}