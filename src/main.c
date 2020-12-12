//任务9-2矩阵按键-行列反转法
//功能：设置密码8，开机显示-，密码锁打开；输错显示E，如果输错3次则系统锁定
#include <reg51.h> //包含头文件reg51.h，定义了51单片机的专用寄存器
#include "lcd1602.h"
/*------宏定义及全局变量-----*/
#define keyPort P0                                                                                                              //定义P0口名称，键盘4*4的接口
sbit LOCK = P2 ^ 0;                                                                                                             //用LED灯表示密码锁，=0时打开灯亮，=1时锁住灯灭
unsigned char keyCodeList[] = {0xd7,0xeb,0xdb,0xbb,0xed,0xdd,0xbd,0xee,0xde,0xbe,0xe7,0xb7,0x7e,0x7d,0x7b,0x77}; //4x4键盘码表
unsigned char password[6] = {1, 2, 3, 4, 5, 6};                                                                                 //定义密码为123456
unsigned char input[10] = {0};                                                                                                  //定义输入密码存放数组，最多10个

/*------函数声明------*/
char key_scan();                            //键盘扫描函数声明
void char_display(unsigned char display[]); //字符串显示函数声明
void set_newPassword();                     //设置新密码
/*------主函数------*/                      //延时函数声明
void main()                                 //主函数
{
    unsigned char i;               //定义循环变量i
    unsigned char correct;         //密码正确与否标志，=1为正确,=0为错误
    char button;                   //保存按键信息，有负值，要用char型
    unsigned char pressNumber = 0; //统计按键按下次数
    LOCK = 1;                      //密码锁初始关闭
    keyPort = 0xff;                //让按键初始为高电平
    /*------LCD初始显示password:------*/
    lcd_int(); //调用LCD初始化函数
    delay(200);
    lcd_w_cmd(0x00 + 0x80); //设置起始显示位置在第一行第0列
    delay(200);
    char_display("password:"); //调用字符串显示函数，屏幕显示password字样
    while (1)                  //系统循环监视
    {
        button = key_scan(); //存储键盘按下的编号
        if (button != -1)    //反馈值为-1，表示无任何按键按下
        {
            if (button >= 0 && button <= 9) //把键盘按下的字母依次显示出来
            {
                delay(200);
                lcd_w_cmd(0x09 + 0x80 + pressNumber); //前面9位是留给“password:”字样的，所以每按一次，位置往后移1位
                lcd_w_dat(button + 0x30);    //数字在LCD中显示，其值要加上0x30才是ASCII字符表对应的数值。或者写lcd_w_dat('button')
                input[pressNumber] = button; //这里特别要注意，把按键编号存入到按下次数所在的下标的数组，而不能是input[button]。
                pressNumber++; //按键次数加1
            }
            else if (button == 11) //按键编号11位为“确认键”，按下，结束密码输入进行判断正确与否
            {
                correct = 1;          //初始密码假设为正确
                if (pressNumber != 6) //先判断输入次数是否刚好6次，如果不是，则密码错误
                    correct = 0;
                else //输入次数正确后，再判断密码是否一一对应
                {
                    for (i = 0; i < 6; i++)
                    {
                        if (input[i] != password[i]) //只要有一个没对应上，密码就错误
                        {
                            correct = 0; //判断正确的标志变成0，表示错误
                            break;       //跳出循环
                        }
                    }
                }
                if (correct == 1) //再次判断coreect标志是否为1
                {
                    lcd_w_cmd(0x4a + 0x80); //设置显示位置在第2行第10列
                    delay(200);
                    char_display("pass"); //在第二行显示pass字样
                    LOCK = 0;             //密码锁打开
                    while (1)
                    {
                        button = key_scan();
                        if (button != -1) //如果没有按键按下，则一直等待在这里
                            break;
                    }
                    keyPort = 0xf0;         //重新对按键进行高低电平，来判断是否有按键按下
                    while (keyPort != 0xf0) //如果按键松开，keyPort=0xf0，则跳出循环
                        ;
                    if (button == 15) //按下的按键刚好是15号键，则设置新密码
                    {
                        set_newPassword();         //设置新密码
                        delay(50000);              //延时500ms，以便观察输入完最后一个密码
                        lcd_w_cmd(0x01);           //清屏可以重新输入
                        lcd_w_cmd(0x00 + 0x80);    //光标重新定位到第1行第0列
                        char_display("password:"); //调用字符串显示函数，屏幕显示password字样
                        pressNumber = 0;           //复位按键按下的次数统计
                    }
                }
                else //coreect=0则密码错误
                {
                    lcd_w_cmd(0x4a + 0x80); //设置显示位置
                    delay(200);
                    char_display("error"); //在第二行显示error字样
                    LOCK = 1;              //密码锁关闭
                    while (key_scan() == -1)
                        ; //如果没有按键按下，则一直等待在这里
                    //如果是密码错误，则继续往下执行，重新清屏再次重来
                    lcd_w_cmd(0x01);           //清屏
                    lcd_w_cmd(0x00 + 0x80);    //光标重新定位到第1行第0列
                    char_display("password:"); //调用字符串显示函数，屏幕显示password字样
                    pressNumber = 0;           //复位按键按下的次数统计
                }
            }
        }
    }
}

/*-----按键扫描函数，判断哪个按键按下，返回按键编号，因为有负值所以不能用unsigned char-----*/
char key_scan()
{
    char scan1, scan2;       //存储两次扫描的键值变量
    char keyCode, keySelect; //定义键值和返回键选值
    char i;                  //循环计数变量
    keySelect = -1;          //键选值初始为-1，表示没有按键按下
    keyPort = 0x0f;          //对矩阵键盘所接的8个引脚，其中4行写1，对其他4列写0
    scan1 = keyPort;         //读取此时的键盘状态
    if (scan1 != 0x0f)       //如果键盘不等于前面赋值，则表示有按键按下
    {
        delay(1000);       //消除按键抖动
        scan1 = keyPort;   //再次读取键盘状态，如果第一行的第一个s0按键按下，则scan1=00001110
        if (scan1 != 0x0f) //再次判断如果键盘状态改变，则表示有按键按下。
        {
            keyPort = 0xf0;          //对行列进行反转赋值，其中行写0，列写1
            scan2 = keyPort;         //由于s0按键按下，连接其的第1列引脚会被拉低为0，变成scan2=11100000
            keyCode = scan1 | scan2; //将两次扫描的值进行或操作，得到键盘值的扫描码，以便和键码表比选
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

/*------显示字符串文字的LCD1602函数---*/
//调用方法：char_display("QING YUAN");
void char_display(unsigned char display[]) //形参为字符串数组
{
    unsigned char i = 0;       //循环计数变量
    while (display[i] != '\0') //只要没到字符串的最后一个元素就执行，因为字符串数组自动在末尾加了\0
    {
        lcd_w_dat(display[i]); //每次把单个字符串元素写入到LCD
        i++;                   //依次累加
    }
}

/*------设置新密码------*/
void set_newPassword()
{
    char key, count = 0;             //key存储按键编号，因为有负数，一定要char型，count存储按键按下次数
    lcd_w_cmd(0x01);                 //清屏
    lcd_w_cmd(0x00 + 0x80);          //光标重新定位到第1行第0列
    char_display("password setup:"); //调用字符串显示函数，屏幕显示password setup:字样
    lcd_w_cmd(0x40 + 0x80);          //显示位置设在第2行第0列
    while (1)
    {
        key = key_scan(); //读取按键编号
        if (key != -1) //有按键按下则执行
        {
            lcd_w_dat('*'); //按下后就显示*
            if (key >= 0 && key <= 9) //如果是数字键0~9按下，则存储密码
            {
                password[count] = key; //这里特别要注意，把按键编号存入到按下次数所在的下标的数组
            }
            count++; //输入密码个数累加1
            if (count == 6)
                break; //输入6个密码后，就跳出循环
        }
    }
}