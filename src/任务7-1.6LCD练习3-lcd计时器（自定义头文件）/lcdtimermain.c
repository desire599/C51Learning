#include <reg51.h> //包含头文件reg51.h，定义了51单片机的专用寄存器
#include "lcd1602.h"
//全局变量定义
unsigned char count = 0; //全局变量，对50ms定时器T1进行计满溢出次数统计
unsigned char miao = 0;  //全局变量，可在所有函数中使用，其值为最后一次执行的结果，
                         //是1秒计数统计的变量
unsigned char fen = 0;   //全局变量，分钟统计的变量
unsigned char shi = 0;   //全局变量，小时统计的变量

//函数名：timer_1()
//函数功能：定时器T1中断函数，假设T1在工作方式1下每50ms产生溢出中断
//形式参数：无
//返回值：无
void timer_1() interrupt 3 //50ms定时器T1进入的中断函数，中断序号为3，功能：定时器T1中断函数
{
  count++;                     //50ms定时器T1每隔50ms进入到这个中断函数，count用于统计进入的次数
  TH1 = (65536 - 50000) / 256; //重新设置T1计数初值高8位，不然定时器T1不会第二次重新开始计数
  TL1 = (65536 - 50000) % 256; //重新设置T1计数初值低8位
  if (count == 20)             //当执行了20次后，则表示1s时间到
  {
    count = 0; //当统计到20次时，将50ms的计数变量count清0
    miao++;    //1秒计数变量miao加1
    if (miao == 60)
    {
      miao = 0;      //当miao计数到60，则从0重新开始计数
      fen++;         //分钟fen开始加1
      if (fen == 60) //当分钟fen计数到60时，则从0重新开始计数
      {
        fen = 0;
        shi++; //小时shi开始加1
        if (shi == 24)
          shi = 0; //当时钟shi计数到24时，则从0重新开始计数
      }
    }
  }
}

void main() //主函数
{
  unsigned char time[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':'}; //此处的数组不能加code，否则显示不了
  unsigned char title[] = "Clock";
  unsigned char i;
  P1 = 0xff; //送全1到P1口
  lcd_int(); //调用LCD初始化函数
  delay(255);
  TMOD |= 0x10;                //设置定时器T1，工作方式1
  TH1 = (65536 - 50000) / 256; //设置定时器T1为50ms间隔中断，高8位赋初值，以便第一次50ms后进入定时器中断函数
  TL1 = (65536 - 50000) % 256; //设置定时器T1为50ms间隔中断，低8位赋初值
  EA = 1;                      //总中断允许位置1（EA总中断关掉，则外部中断、定时器计数器都停止）
  ET1 = 1;                     //定时器T1允许位置1
  TR1 = 1;                     //启动定时器T1，表示单片机一上电就启动定时器T1
  lcd_w_cmd(0x06 + 0x80);      //设置标题的显示位置在第一行第6列
  delay(255);
  for (i = 0; title[i] != '\0'; i++) //显示字符串，当字符串结束符为'\0'时，则跳出for循环。
  {
    lcd_w_dat(title[i]);
    delay(200);
  }
  while (1)
  {
    lcd_w_cmd(0x44 + 0x80); //设置起始显示位置在第2行第4列
    delay(255);
    lcd_w_dat(time[shi / 10]); //显示小时的十位
    delay(200);
    lcd_w_dat(time[shi % 10]); //显示小时的十位
    delay(200);
    lcd_w_dat(time[10]); //显示冒号
    delay(200);
    lcd_w_dat(time[fen / 10]); //显示分钟的十位
    delay(200);
    lcd_w_dat(time[fen % 10]); //显示分钟的个位
    delay(200);
    lcd_w_dat(time[10]); //显示冒号
    delay(200);
    lcd_w_dat(time[miao / 10]); //显示秒的十位
    delay(200);
    lcd_w_dat(time[miao % 10]); //显示秒的个位
    delay(200);
  }
}
