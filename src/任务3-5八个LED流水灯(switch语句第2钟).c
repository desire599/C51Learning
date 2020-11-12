#include<REG52.H>
sbit k1 = P0 ^ 0;//定义按键的引脚
void delay(unsigned int i)
{
    while(i--);
}
unsigned char key()//判断按键是否按下的函数，按下然后松开一次返回1，否则返回0
{
    if(k1==0)
    {
        delay(1000);
        if(k1==0)
        {
            while(!k1);
            delay(1000);
            return 1;
        }
    }
    return 0;
}
void main()
{
    unsigned char counter=0;//按键按下次数统计
    P1 = 0xff;
    while(1)
    {
        while(key())//按键按下，key()返回1，则执行一次；再次返回到while判断key()此时按键没按则不执行
        {
            counter++;//次数加1，变成1
			if(counter==3)//当第三次按下时，counter又变成了1
                counter=1;
        }    
        switch(counter)//根据couner变量的不同值执行case对应语句
        {
            case 1:while(P1!=0x00){P1 <<= 1;delay(30000);};break;
            case 2:P1 = 0xff;break;
            default:break;
        }
    }
}