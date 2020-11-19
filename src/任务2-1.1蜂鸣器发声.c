#include "reg52.h"		 //51系列函数的头文件定义
#define u8 unsigned char //宏定义，用u8表示unsigned char无符号字符8位数
#define u16 unsigned int //宏定义，用u16表示unsigned int无符号整型16位数
sbit beep = P1 ^ 0;		 //特殊功能位定义，用beep表示单片机的P1.5端口
void delayms(u16 xms)	 /*此句为定义一个子函数，自定义函数名为delayms（xms），
	                       其中xms为16位整数，delayms(1)表示延时1ms，delayms（1000）延时1s*/
{
	u16 i, j;				  //定义该函数内的i和j为16位整数；
	for (i = xms; i > 0; i--) //for循环函数，循环次数为xms指定的次数。
		for (j = 123; j > 0; j--)
			; //嵌套另一个for循环函数，表示循环次数为123次
	//所以这个延时函数执行的总次数为xms*123次
}
void main() //程序从main函数里面执行，遵循从上往下执行的顺序
{
	while (1) //while函数括号中为1表示一直循环执行
	{
		beep = ~beep; //对beep的状态取反，则相当于进行高低电平的脉冲频率输出
		delayms(1);	  //延迟时间的大小影响了脉冲输出的频率大小，从而改变蜂鸣器的声音
	}
}
