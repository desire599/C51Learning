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
		P2 = 0xff & 0x3f; // 与操作进行屏蔽第7、8位（要屏蔽的相应位设0），即0011 1111=0x3f，让其余6位为高电平。每次必须要熄灭全部的数码管，否则动态数码管无法显示出来。
		P1 = led[i];	  //传送选择led数码管数组对应的receive数组的数据到P1口，显示出来。
		P2 = ~temp;		  //对temp取反（由0000 0001变为1111 1110）后赋值给P2，此时只打开第一个数码管P2.0
		for (j = 0; j < 100; j++)
			;		//软件空循环，延时约1ms
		temp <<= 1; //将w的变量左移一位，0000 0010。这样第二次循环时，P2=~temp=1111 1101，只打开第二个数码管P2.1
			//当for循环结束后，由于主函数有while(1)disp()，所以又重新进入disp()子函数，执行temp=0x01，这样又可以重新循环而temp不会全部变成0.
	}
}
