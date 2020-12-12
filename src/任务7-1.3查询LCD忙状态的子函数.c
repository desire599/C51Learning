#define dataPort P1 //用dataPort来表示P1数据通信口

void check_busy(void)
{
    unsigned char status;//存储P1数据口的状态
    RW = 1;  //读取
    delay(1); //短暂延时可以不写，因为单片机比LCD执行速度要快，所以程序中要人为变慢单片机的执行速度
    RS = 0;  //此时为命令寄存器
    delay(1);
    do
    {
        dataPort = 0x00;
        E = 1; //在E端为高电平时，读操作有效
        status = dataPort; //读取数据口的状态（即LCD状态）存入status变量
        E = 0;//E端产生下降沿，LCD执行
        status = status & 0x80;//屏蔽低7位，只检查最高位（BF）
    } while (status==1);//如果status=1，则表示LCD正在忙，继续do查询，直到LCD不忙时，status自动变成0然后退出
}