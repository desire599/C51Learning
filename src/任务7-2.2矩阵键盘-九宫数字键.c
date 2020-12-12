//任务9-2矩阵按键-调整键盘的编号与数字键一致
//功能：设置密码8，开机显示-，密码锁打开；输错显示E，如果输错3次则系统锁定
#include <reg51.h> //包含头文件reg51.h，定义了51单片机的专用寄存器
/*------宏定义及全局变量-----*/
#define keyPort P0                                                                                                              //定义P0口名称，键盘4*4的接口
#define displayPort P1                                                                                                          //定义P1口名称，数码管的接口
sbit LOCK = P3 ^ 0;                                                                                                             //用LED灯表示密码锁，=0时打开灯亮，=1时锁住灯灭
unsigned char keyCodeList[]={0xd7,0xee,0xde,0xbe,0xed,0xdd,0xbd,0xeb,0xdb,0xbb,0x7e,0x7d,0x7b,0xe7,0xb7,0x77};//根据九宫数字键调整后的4*4键盘码表
/*------函数声明------*/
void delay(unsigned int i);
char key_scan();       //键盘扫描函数声明
/*------主函数------*/ //延时函数声明
void main()            //主函数
{
    char button;                   //保存按键信息，有负值，要用char型
    unsigned char display[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e};
    //定义数组led，依次存储包括0～9,A~F的共阳极数码管显示码表
    unsigned char status[] = {0xbf, 0x8c, 0x86}; //状态码“-”、“P”、“E”
    displayPort = status[0];                     //数码管初始为“-”
    LOCK = 1;                                    //密码锁初始关闭
    keyPort = 0xff;                              //让按键初始为高电平
    while (1)                                    //系统循环监视
    {
        button = key_scan();  //存储键盘按下的编号
        if (button == -1)     //反馈值为-1，表示无任何按键按下
            continue;         //不执行下面的语句，直接开始下一轮循环
        else //
        {
            displayPort = display[button]; //数码管显示按下的键
            delay(50000);
        }
    }
}

/*-----延时函数------*/
void delay(unsigned int i)
{
    while (i--)
        ;
}

/*-----按键扫描函数，判断哪个按键按下，返回按键编号，因为有负值所以不能用unsigned char-----*/
char key_scan()
{
    char scan1, scan2;       //存储两次扫描的键值变量
    char keyCode, keySelect; //定义键值和返回键选值
    char i;                  //循环计数变量
    keySelect = -1;          //键选值初始为-1，表示没有按键按下
    keyPort = 0x0f;          //1、对矩阵键盘所接的8个引脚，其中4行写1，对其他4列写0
    scan1 = keyPort;         //2、读取此时的键盘状态
    if (scan1 != 0x0f)       //如果键盘不等于前面赋值，则表示有按键按下
    {
        delay(1000);       //消除按键抖动
        scan1 = keyPort;   //再次读取键盘状态，如果第一行的第一个s0按键按下，则scan1=00001110
        if (scan1 != 0x0f) //再次判断如果键盘状态改变，则表示有按键按下。
        {
            keyPort = 0xf0;          //3、对行列进行反转赋值，其中行写0，列写1
            scan2 = keyPort;         //4、由于s0按键按下，连接其的第1列引脚会被拉低为0，变成scan2=11100000
            keyCode = scan1 | scan2; //5、将两次扫描的值进行或操作，得到键盘值的扫描码，以便和键码表比选
            for (i = 0; i < 16; i++) //将此时的键码与键盘码表做对比，共16个值
            {
                if (keyCode == keyCodeList[i]) //当找到键盘码表数组的值与此时的按键键码一致时
                {
                    keySelect = i; //这时候的i值，就是对应的按键编号，就是按下的那个按键
                    break;
                }
            }
            while (keyPort != 0xf0)
                ; //判断按键是否松开，这样按键从按下到松开才算完成，避免重复
        }
    }
    return keySelect; //返回按键编号，范围在0~15之间，-1表示没有按键按下
}