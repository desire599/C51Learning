//程序：ex6_2.c
//功能：乙机接收及显示程序，采用中断方式实现
#include <reg51.h>												//包含头文件reg51.h，定义51单片机的专用寄存器
unsigned char receive[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //定义接收数据缓冲区
void disp();													//显示函数的提前声明
void main()														//主函数
{
	unsigned char i;
	TMOD = 0x20; //定时器T1工作于方式2，8位定时器，TL1和TH1存放的数值相同，TH为备用初值，以便自动装载初值，从而实现自动循环定时中断。
	TL1 = 0xfd;	 //波特率定义为2400bps，要与甲机的设置一致
	TH1 = 0xfd;
	TR1 = 1;				//启动定时器
	SCON = 0x40;			//定义串行口工作于方式1
	REN = 1;				//接收允许
	for (i = 0; i < 6; i++) //执行6次，表示接收6次数据
	{
		while (RI == 0)
			;			   //空循环等待，当查询到RI为1时，表示接收数据完毕，不满足RI==0的条件，退出while循环
		receive[i] = SBUF; //接收数据，从SBUF缓冲区取数据存到接收数组receive
		RI = 0;			   //RI由软件清0
	}
	while (1)
	{
		disp(); //写while循环可让数码管一直显示数值
	}
}
//函数名：disp
//函数功能：在6个LED上显示buffer中的6个数
//入口参数：无
//出口参数：无
void disp()
{
	unsigned char i, j, temp;
	unsigned char code led[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
	//定义0～9显示码，共阴极数码管，即让公共端为低电平，数码管即可使用
	temp = 0x01; //借助一个变量temp来做移位操作，存放状态；0000 0001
	//因为移位运算符<<是移位后自动补0，而数码管是低电平有效，所以不能直接用1111 1110
	//如果直接写P2=0x01，则会在后面执行for循环第一句P2=0xff时被覆盖掉，从而无法实现移位循环
	for (i = 0; i < 6; i++)
	{
		P2 = 0xff & 0x3f;  // 与操作进行屏蔽第7、8位（要屏蔽的相应位设0），即0011 1111=0x3f，让其余6位为高电平。每次必须要熄灭全部的数码管，否则动态数码管无法显示出来。
		P1 = led[receive[i]]; //传送选择led数码管数组对应的receive数组的数据到P1口，显示出来。
		P2 = ~temp;			  //对temp取反（由0000 0001变为1111 1110）后赋值给P2，此时只打开第一个数码管P2.0
		for (j = 0; j < 100; j++)
			;		//软件空循环，延时约1ms
		temp <<= 1; //将w的变量左移一位，0000 0010。这样第二次循环时，P2=~temp=1111 1101，只打开第二个数码管P2.1
					//当for循环结束后，由于主函数有while(1)disp()，所以又重新进入disp()子函数，执行temp=0x01，这样又可以重新循环而temp不会全部变成0.
	}
}
