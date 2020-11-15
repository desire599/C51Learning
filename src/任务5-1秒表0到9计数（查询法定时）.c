﻿//程序：定时器练习程序
//功能：0~9计数，每隔一秒计一次，如此循环显示
#include "reg51.h"  //包含头文件reg51.h，定义了51单片机的专用寄存器
void t1_50ms(unsigned char i) //自定义定时器T1的函数，函数名t1_50ms
{
	unsigned char k;
	TMOD=0x10;    //1、主程序中设置定时器T1，工作方式为1。（此句可写在主程序，也可写在定时器子程序）
	for(k=0;k<i;k++)
	{ //设定时器T1的工作方式1，每执行一次所需时间50ms。
		TH1=(65536-50000)/256; //2、每次重新对T1的高8位赋初值，低8位的数值为256，故做除法取商放前面高8位
	    TL1=(65536-50000)%256; //2、每次重新对T1的低8位赋初值，取余数放后8位
		TR1=1;  //3、启动定时器T1
		while(!TF1); //4、查询是否计满数溢出，每次计满50ms时，则自动将TF1=1，退出while循环
		TF1=0;       // 5、对溢出标志清0，重新开始
	}
}

void main()
{
	char i;       //定义i为8位字符型数
	unsigned char code disp[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
//定义0～9显示码，共阳极数码管
	while(1)
	{
		for (i = 0;i<10;i++)
		{
			P1 = disp[i];
			t1_50ms(20);
		}
	}

}	
		
		

