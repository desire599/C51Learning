#include <reg52.h>
#include <stdio.h>
#include <string.h>

/*-----端口定义-----*/
#define DataPort P1 // LCD数据线D0-D7
sbit RS = P2 ^ 2;   // 数据\指令 选择 RS
sbit RW = P2 ^ 1;   // 读\写 选择
sbit E = P2 ^ 0;    // 读\写使能
sbit cs1 = P2 ^ 4;  // 片选1
sbit cs2 = P2 ^ 3;  // 片选2

/*-----定义LCD大小-----*/
#define LCD_X_MAX 128 //   定义液晶x轴的像素数 *
#define LCD_Y_MAX 64  //   定义液晶y轴的像素数 *

//ASCII 字模宽度及高度
#define ASC_CHR_WIDTH 8
#define ASC_CHR_HEIGHT 16
#define HZ_CHR_WIDTH 16
#define HZ_CHR_HEIGHT 16

#define uchar unsigned char

uchar code GB_16[5][32] ={ //5行，表示5个汉字；每个汉字有32个字码

	//单(0) 片(1) 机(2) 技(3) 术(4)，字体为宋体

{0x00,0x00,0xF8,0x49,0x4A,0x4C,0x48,0xF8, 
0x48,0x4C,0x4A,0x49,0xF8,0x00,0x00,0x00, //以上16列为汉字上半部分
0x10,0x10,0x13,0x12,0x12,0x12,0x12,0xFF, //以下16列为汉字下半部分
0x12,0x12,0x12,0x12,0x13,0x10,0x10,0x00},/*"单",0*/

{0x00,0x00,0x00,0xFE,0x20,0x20,0x20,0x20,
0x20,0x3F,0x20,0x20,0x20,0x20,0x00,0x00,
0x00,0x80,0x60,0x1F,0x02,0x02,0x02,0x02,
0x02,0x02,0xFE,0x00,0x00,0x00,0x00,0x00},/*"片",1*/

{0x10,0x10,0xD0,0xFF,0x90,0x10,0x00,0xFE,
0x02,0x02,0x02,0xFE,0x00,0x00,0x00,0x00,
0x04,0x03,0x00,0xFF,0x00,0x83,0x60,0x1F,
0x00,0x00,0x00,0x3F,0x40,0x40,0x78,0x00},/*"机",2*/

{0x10,0x10,0x10,0xFF,0x10,0x90,0x08,0x88,
0x88,0x88,0xFF,0x88,0x88,0x88,0x08,0x00,
0x04,0x44,0x82,0x7F,0x01,0x80,0x80,0x40,
0x43,0x2C,0x10,0x28,0x46,0x81,0x80,0x00},/*"技",3*/

{0x00,0x10,0x10,0x10,0x10,0xD0,0x30,0xFF,
0x30,0xD0,0x12,0x1C,0x10,0x10,0x00,0x00,
0x10,0x08,0x04,0x02,0x01,0x00,0x00,0xFF,
0x00,0x00,0x01,0x02,0x04,0x08,0x10,0x00},/*"术",4*/

};

/*--------函数列表-------*/
/*-----状态检查-----*/
void Check_Busy(void)
{
    uchar dat;
    RS = 0;
    RW = 1;
    do
    {
        DataPort = 0x00;
        E = 1;
        dat = DataPort;
        E = 0;
        dat = 0x80 & dat; //仅当第7位为0时才可操作(判别busy信号)
    } while (!(dat == 0x00));
}

/*-----向LCD发送命令------*/
void Writ_Comd(uchar command)
{
    Check_Busy();
    RW = 0;
    RS = 0;
    DataPort = command;
    E = 1;
    E = 0;
}

/*-----写显示数据------*/
void Write_Dat(uchar dat)
{
    Check_Busy();
    RW = 0;
    RS = 1;
    DataPort = dat;
    E = 1;
    E = 0;
}

/*------设定行地址(页)--X 0-7------*/
void Set_Line(uchar line)
{
    line = line & 0x07; // 0<=line<=7
    line = line | 0xb8; //1011 1xxx
    Writ_Comd(line);
}

/*------设定列地址--Y 左右半屏各0-63列-----*/
void Set_Column(uchar column)
{
    column = column & 0x3f; // 0=<column<=63
    column = column | 0x40; // 01xx xxxx
    Writ_Comd(column);
}

/*-----设定显示开始行--XX-----*/
void Set_StartLine(uchar startline) //0--63
{
    startline = startline & 0x07;
    startline = startline | 0xc0; //1100 0000
    Writ_Comd(startline);
}

/*-----开关显示-----ONOFF=1:ON;ONOFF=0:OFF-----*/
void Set_OnOff(uchar onoff)
{
    onoff = 0x3e | onoff; //0011 111x
    Writ_Comd(onoff);
}

/*------选择屏幕：screE: 0-全屏,1-左屏,2-右屏-----*/
void Select_Screen(uchar screE)
{
    switch (screE)
    {
    case 0:
        cs1 = 1; //高电平有效
        cs2 = 1;
        break;
    case 1:
        cs1 = 0;
				cs2 = 1;//有些硬件的左右屏可能不同，根据实际显示效果进行修改
        break;
    case 2:
        cs1 = 1;
        cs2 = 0;
        break;
    }
}

/*-----清屏：screE: 0-全屏,1-左屏,2-右-----*/
void LCD_Clr(uchar screE)
{
    unsigned char i, j;
    Select_Screen(screE);
    for (i = 0; i < 8; i++)
    {
        Set_Line(i);              //选择一行，总共8行
        for (j = 0; j < 128; j++) //每行全屏的话，每行有128列
        {
            Write_Dat(0x00);
        }
    }
}

/*-----初始化LCD------*/
void LCD_Init(void)
{
    uchar i = 250; //延时
    while (i--)
        ;
    Select_Screen(0);  //选择屏幕，0-全屏
    Set_OnOff(0);     //关显示
    LCD_Clr(0);       //清屏
    Select_Screen(0);  //选择屏幕，0-全屏
    Set_OnOff(1);     //开显示
    Select_Screen(0);  //选择屏幕，0-全屏
    Set_StartLine(0); //开始行:0
}

// 在指定位置显示字符：x=0-120（字母、数字）/0-112（汉字），y=0-6
void Out_Char(uchar x, uchar y, uchar i, bit mode) // x为列坐标，y为页坐标，在指定位置显示字符
	{     //i为汉字数组行数选择，mode=1，正常显示（阴码）；mode=0，反白显示（阳码）
    unsigned char j;//定义列计数的循环变量
	  int cData;//定义待发送的数据存储变量
    for (j = 0; j < 2 * HZ_CHR_WIDTH; j++) //由于一个汉字采用上下两个部分，每一部分占16列，所以要循环16*2的列次
    {        
        if (mode == 1)
            cData = GB_16[i][j]; //从GB_16汉字数组中取对应的汉字字码，存入待发送的数据存储变量
        else
            cData = ~GB_16[i][j];
        if (j < HZ_CHR_WIDTH) //汉字上半部，共需要扫描16列
        {
            if ((x + j) < 64) //坐标列数x的值加上当前的扫描列数如果小于64列，则只需要打开左半屏
            {
                Select_Screen(1); //选择左半屏
                Set_Column(x + j); //此时的选择列数为x坐标+当前的k值
            }
            else
            {
                Select_Screen(2); //选择右半屏
								Set_Column(x + j - 64);//减去64列，表示把左半屏已经显示的去除掉
            }
            Set_Line(y);//设置页数，每页8行对应LCD通信数据口的8个引脚状态
        }
        else //汉字下半部，共需要扫描16列
        {
            if ((x + j - HZ_CHR_WIDTH) < 64)
            {
                Select_Screen(1); //选择左半屏
                Set_Column(x + j - HZ_CHR_WIDTH);//下半部分其实在下一页，所以要换行下一页的最左边开始，所以要减去汉字宽度16列
            }
            else
            {
                Select_Screen(2); //选择右半屏
                Set_Column((x + j - HZ_CHR_WIDTH) - 64);
            }
            Set_Line(y + 1); //设置下一页
        }
        Write_Dat(cData); //把汉字字码发送到LCD
    }
}
void main()
{
	unsigned char i,j=16;//i为汉字数组的个数增量,j为初始坐标值，列数
  LCD_Init(); //LCD初始化
	for(i=0;i<5;i++) //共5个汉字需要连续显示，所以循环5次
	{
		Out_Char(j,0,i,0);//j为起始列，0为第0页（要2页才能显示一个汉字），i为汉字选位，0为模式0
		j=j+16;//每个汉字的位置相隔一个字体的宽度，即16个像素
	}
}