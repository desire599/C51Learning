//程序：任务5-7-6个数码管进行24小时计时
//功能：六个动态数码管共阴极接法，定时器采用中断方式
#include "reg51.h" //包含头文件reg51.h，定义了51单片机的专用寄存器
//全局变量定义
unsigned char count = 0; //对50ms定时时间进行计数
unsigned char sec = 0;   //秒计数变量
unsigned char min = 0;   //分计数变量
unsigned char hour = 0;  //小时计数变量

//函数名：timer_1()
//函数功能：定时器T1中断函数，假设T1在工作方式1下每50ms产生中断，执行该中断函数
//形式参数：无
//返回值：无
void timer_1() interrupt 3 //（5）编写中断服务程序，T1的中断类型号为3
{
  count++;                    //50ms计数器加1
  TH1 = (65536 - 5000) / 256; //重新设置T1计数初值高8位
  TL1 = (65536 - 5000) % 256; //重新设置T1计数初值低8位
  if (count == 20)            //1s时间到
  {
    count = 0; //50ms计数器清0
    sec++;     //秒计数器加1
    if (sec == 60)
    {
      sec = 0; //sec计数到60，则从0开始计数
      min++;   //此时分钟min计数加1
      if (min == 60)
      {
        min = 0;
        hour++; //此时小时hour计数加1
        if (hour == 24)
        {
          hour = 0; //计满24小时后又清零
        }
      }
    }
  }
}

//函数名：disp()
//函数功能：将数组显示在六个动态连接的数码管上，采用共阴极接法，方便使用“P口的低电平有效”原则
//形式参数：无，由于需要3个变量的显示，秒、分、时，所以可以采用无参写法
//返回值：无

void disp() //由于涉及三个变量，可用无参写法，直接使用该三个全局变量。当然也可以同时定义三个形参。
{
  unsigned char j;
  unsigned char code led[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
  //定义0～9显示码，共阴极数码管
  //由于需要接6个数码管，重新接线，P2的值有改变
  //依次为秒的个位，秒的十位，分钟的个位，分钟的十位，小时的个位，小时的十位
  /*1、秒部分的两位数显示。*/
  P2 = 0xff;
  P1 = led[sec % 10]; //显示秒sec个位，取余数放在右起第1个数码管
  P2 = 0xfe;          //只打开第1个数码管,P2.0 P2=1111 1110=0xfe
  for (j = 0; j < 50; j++)
    ;                 //空运算100次，延时约1ms
  P2 = 0xff;          //关闭6个数码管
  P1 = led[sec / 10]; //显示秒sec十位，除法后的商放在右起第2个数码管
  P2 = 0xfd;          //只打开第2个数码管,P2.1 P2=1111 1101=0xfd
  for (j = 0; j < 50; j++)
    ; //空运算100次，延时约1ms
  /*2、分钟部分的两位数显示。*/
  P2 = 0xff;
  P1 = led[min % 10]; //显示分min个位，取余数放在右起第3个数码管
  P2 = 0xfb;          //只打开第3个数码管,P2.2 P2=1111 1011
  for (j = 0; j < 50; j++)
    ;                 //空运算100次，延时约1ms
  P2 = 0xff;          //关闭6个数码管
  P1 = led[min / 10]; //显示分min十位，除法后的商放在右起第4个数码管
  P2 = 0xf7;          //只打开第4个数码管,P2.3 P2=1111 0111
  for (j = 0; j < 50; j++)
    ; //空运算100次，延时约1ms
  /*3、小时部分的两位数显示。*/
  P2 = 0xff;
  P1 = led[hour % 10]; //显示小时hour个位，取余数放在右起第5个数码管
  P2 = 0xef;           //只打开第5个数码管,P2.4 P2=1110 1111
  for (j = 0; j < 50; j++)
    ;                  //空运算100次，延时约1ms
  P2 = 0xff;           //关闭6个数码管
  P1 = led[hour / 10]; //显示小时hour十位，除法后的商放在右起第6个数码管
  P2 = 0xdf;           //只打开第6个数码管,P2.5 P2=1101 1111
  for (j = 0; j < 50; j++)
    ; //空运算100次，延时约1ms
}

void main() //主函数
{
  TMOD = 0x10;                 //（1）设置T1为工作方式1
  TH1 = (65536 - 50000) / 256; //（2）设置T1计数初值高8位，定时时间50ms
  TL1 = (65536 - 50000) % 256; //（2）设置T1计数初值低8位
  EA = 1;                      //（3）开放总中断允许
  ET1 = 1;                     //（3）开放T1中断允许
  TR1 = 1;                     //（4）启动T1开始计数
  while (1)
  {
    disp(); //调用显示函数
  }
}
