#include <REG52.H>         //包含51系列单片机的头文件·
sbit k1 = P0 ^ 0;//定义按键k1
void delay(unsigned int i) //延时函数，delay(1)；约延时0.1ms
{
    while (i--)
        ;
}
void main()
{
    P1 = 0xff; //初始全部熄灭
    delay(1000);
    while (1)
    {
        if(k1==0)
        {
            delay(1000);
            if(k1==0)
            {
                while(!k1)
                    ;
                delay(1000);
                P1 = 0xfe;
                delay(1000);
                while(P1!=0x00)//当第8个灯点亮时才跳出循环
                {
                    P1 <<= 1;
                    delay(30000);
                }
            }
        }
    }
}