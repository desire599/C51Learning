#include "reg51.h"
void delay(unsigned int i) //�Զ�����ʱ����
{
    while (i--) //ִ����������㣬�Ӷ�ʵ��ϵͳ����ʱЧ��
        ;
}

void main()
{
    while (1)
    {
        P1 = 0xff; // ��ʾP1.0=1��Ϩ��LED
        delay(1000); //��ʱԼ10ms
        P1 = 0xfe; // ��ʾP1.0=0������LED
        delay(1000);
    }
}

/*��2��д����led��˸
sbit led=P1^0;
void main()
{
    while (1S
    {
        led=~led; // ��P1.0��״̬ÿ�ν���ȡ������0��1֮��仯��ʵ����˸Ч��
        delay(1000); //��ʱԼ10ms
    }
}
*/S