//任务8-1可调光台灯
//功能：利用PCF8591芯片进行DA转换，输出模拟电压控制台灯的亮度
#include <reg51.h>
#include <INTRINS.H>
sbit SDA = P2 ^ 7; //P2.7定义I2C数据线引脚
sbit SCL = P2 ^ 6; //P2.6定义I2C时钟线引脚
sbit S1 = P1 ^ 1;  //P1.1控制按键，亮度增加
sbit S2 = P1 ^ 0;  //P1.0控制按键，亮度减小
#define delayNOP() \
    ;              \
    {              \
        _nop_();   \
        _nop_();   \
        _nop_();   \
        _nop_();   \
    };                 //定义延时函数，其执行语句为4个空运行指令，从而实现延时
bit bdata SystemError; //从机错误标志位
//——————————PCF8591专用变量定义——————————————
#define PCF8591_WRITE 0x90 //器件写地址，具体参考芯片使用手册
#define PCF8591_READ 0x91  //器件读地址

//函数名：iic_start
//功能：启动I2C总线，即发送I2C起始条件，形参：无，返回值：无
void iic_start()
{
    EA = 0;  //关中断
    SDA = 1; //时钟保持高电平，数据线从高到低一次跳变，I2C通信开始
    SCL = 1;
    delayNOP(); //起始条件建立时间需要大于4.7μs，所以调用延时函数
    SDA = 0;
    delayNOP(); //起始条件锁定时间需要大于4.7μs，所以调用延时函数
    SCL = 0;    //钳住I2C总线，准备发送或接收数据
}

//函数名：iic_stop
//功能：停止I2C总线数据传送，形参：无，返回值：无
void iic_stop()
{
    SDA = 0; //时钟保持高，数据线从低到高一次跳变，I2C通信停止
    SCL = 1;
    delayNOP(); //起始条件建立时间需要大于4.7μs，所以调用延时函数
    SDA = 1;
    delayNOP(); //起始条件锁定时间需要大于4.7μs，所以调用延时函数
    SCL = 0;    //钳住I2C总线，准备发送或接收数据
}

//函数名：slave_ACK
//函数功能：从机发送应答位，形参：无，返回值：无
void slave_ACK()
{
    SDA = 0;
    SCL = 1;
    delayNOP();
    SDA = 1;
    SCL = 0;
}

//函数名：slave_NOACK
//函数功能：从机发送非应答位，迫使数据传输过程结束。形参：无。返回值：无
void slave_NOACK()
{
    SDA = 1;
    SCL = 1;
    delayNOP();
    SDA = 0;
    SCL = 0;
}

//函数名：check_ACK
//函数功能：主机应答位检查，迫使数据传输过程结束。形参：无。返回值：无。
void check_ACK()
{
    SDA = 1; //将I/O设置成输入，必须先向端口写1
    SCL = 1;
    F0 = 0;
    if（SDA==1) //若SDA=1表明非应答，置位非应答标志F0
    {
        F0 = 1;
    }
    SCL = 0;
}

//函数名：IICSendByte
//函数功能：发送一个字节（一个字节由8位二进制组成）。形参：要发送的数据。返回值：无。
void IICSendByte(unsigned char sData)
{
    unsigned char idata n = 8; //向SDA上发送一个字节数据，共8位
    while (n--)
    {
        if ((sData & 0x80) == 0x80) //判断要发送的数据最高位是否为1
        {
            SDA = 1; //如果满足，则发送位置为1
            SCL = 1;
            delayNOP();
            SDA = 0;
            SCL = 0;
        }
        else
        {
            SDA = 0; //如果不满足，则发送位置为0
            SCL = 1;
            delayNOP();
            SCL = 0;
        }
        sData = sData << 1; //把要传输的数据的位左移一位进行下一个循环判断
    }
}

//函数：IICReceiveByte
//函数功能：接收一个字节数据。形参：无。返回值：返回接收的数据
unsigned char IICReceiveByte()
{
    unsigned char idata n = 8; //从SDA线上读取一个字节数据，共8位
    unsigned char rData;       //定义存放数据的变量rData
    while (n--)
    {
        SDA = 1;
        SCL = 1;
        rData = rData << 1; //把接收数据的左移1位
        if (SDA == 1)
        {
            rData = rData | 0x01; //若接收到的位为1，则数据的最后一位置为1
        }
        else
        {
            rData = rData & 0xfe; //否则该数据的最后一位清零
        }
        SCL = 0;
    }
    return rData;
}

//函数名：DAC_PCF8591
//函数功能：发送n位数据
//形参：control为控制字存放变量，wData为要转换的数字量。返回值：无
void DAC_PCF8591(unsigned char controlByte, unsigned char wData)
{
    iic_start();                //启动I2C通信
    IICSendByte(PCF8591_WRITE); //发送地址位
    check_ACK();                //检查应答位
    if (F0 == 1)                //如果非应答标志为1，表示器件错误或已坏，置错误标志位SystemError为1
    {
        SystemError = 1;
        return; //此句是否可不写
    }
    IICSendByte(controlByte & 0x77); //0x77=0111 0111，屏蔽第4和第8位
    check_ACK();
    if (F0 == 1) //如果非应答标志为1，表示器件错误或已坏，置错误标志位SystemError为1
    {
        SystemError = 1;
        return; //此句是否可不写
    }
    IICSendByte(wData);
    check_ACK();
    if (F0 == 1) //如果非应答标志为1，表示器件错误或已坏，置错误标志位SystemError为1
    {
        SystemError = 1;
        return; //此句是否可不写
    }
    iic_stop();
    delayNOP();
    delayNOP();
    delayNOP();
    delayNOP();
}

//函数名：delay_ms
//功能：采用定时器T1延时t毫秒，采用工作方式1，定时器初始值为64536，每次计时1ms（即1000次）
//形参：延时毫秒数（小于255ms）。返回值：无
void delay_ms(unsigned char t)
{
    unsigned char i; //循环计数变量
    TMOD = 0x10;     //设置T1定时器，工作方式为1，16位计数器
    for (i = 0; i < t; i++)
    {
        TH1 = (65536 - 1000) / 256; //设置定时器（计数器）初始值
        TL1 = (65536 - 1000) % 256;
        TR1 = 1; //启动定时器T1
        while (!TF1)
            ;    //查询计数是否溢出，否则空运行等待
        TF1 = 0; //1ms定时到了，对溢出标志位清零
    }
}

void main()
{
    unsigned char voltage; //输出电压值的变量，数字量0对应0.0v，255对应+5.0v
    voltage = 125;         //初始电压值约为2.5V，这样LED灯有初始亮度
    while (1)
    {
        DAC_PCF8591(0x40, voltage); //对控制字写0100 0000表示允许模拟量输出，并写入数字量voltage
        if (S1 == 0)                //按键S1按下，台灯亮度增加
        {
            delay_ms(10); //按键延时消抖
            if (S1 == 0)
            {
                if (voltage == 255)
                    voltage = 125; //当电压变到最大时，又恢复中间值
                else
                    voltage = voltage + 5; //每次增加5
            }
        }
        if (S2 == 0) //按键S2按下，台灯亮度减小
        {
            delay_ms(10); //按键延时消抖
            if (S2 == 0)
            {
                if (voltage == 0)
                    voltage = 125; //当电压变到最大时，又恢复中间值
                else
                    voltage = voltage - 5; //每次减小5
            }
        }
        delay_ms(1);//每次延时1ms
    }
}