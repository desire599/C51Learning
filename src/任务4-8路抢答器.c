//功能：八路抢答器控制程序
#include <reg51.h>               //包含头文件reg51.h，定义了51单片机的专用寄存器
void delay(unsigned int i);     //延时函数声明
void main()                             //主函数
{
   unsigned char button;      //保存按键信息
   unsigned char code disp[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0xbf};  
        //定义数组led，依次存储包括0～7和"-"的共阳极数码管显示码表
   P0=0xff;                   //读引脚状态，需先置1
   P1=disp[5];              //显示"-"
   while(1)
   {
   		button=P0;             //第一次读按键状态
	  	delay(1200);	        //延时消抖
	  	button=P0;              //第二次读按键状态            	
 	  	switch(button)        //根据按键的值进行多分支跳转
    	{
		   case 0xfe: P1=disp[0];delay(10000);while(1){};break;    //0按下，显示0，待机
	     	case 0xfd: P1=disp[1];delay(10000);while(1);break;    //1按下，显示1，待机
		   case 0xfb: P1=disp[2];delay(10000);while(1);break;    //2按下，显示2，待机
	 	   case 0xf7: P1=disp[3];delay(10000);while(1);break;    //3按下，显示3，待机
		   case 0xef: P1=disp[4];delay(10000);while(1);break;    //4按下，显示4，待机
   	   		case 0xdf: P1=disp[5];delay(10000);while(1);break;    //5按下，显示5，待机
		   case 0xbf: P1=disp[6];delay(10000);while(1);break;    //6按下，显示6，待机
	   	 	case 0x7f: P1=disp[7];delay(10000);while(1);break;    //7按下，显示7，待机
		   default: break;
	    }  											
   }
}
//delay
//函数功能：实现软件延时
//形式参数：无符号整型变量i，控制空循环的循环次数
//返回值：无
void  delay(unsigned int i) 	   //延时函数
{
  unsigned int k;
	for(k=0;k<i;k++);
}