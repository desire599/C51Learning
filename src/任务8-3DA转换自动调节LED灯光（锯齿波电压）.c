//任务8-3单片机通过DA转换控制LED-最终的程序
#include <reg52.h>
#include <intrins.h> //包含nop函数
sbit SCL = P1 ^ 7;   //I2C时钟引脚
sbit SDA = P1 ^ 6;   //I2C数据输入输出引脚
bit bdata IIC_ERROR; //I2C应答错误标志位，其中IIC_ERROR为自定义变量名，bdata：定义的变量在20H~2FH的RAM，16byte范围，变量可读写。不写bdata也可以，由系统随机分配内存空间
//延时4微秒函数。每个nop函数执行一个机器周期，所以12MHz的晶振产生1μs的机器周期
void Delay4us()
{
    _nop_();
    _nop_();
    _nop_();
    _nop_();
}

//1、函数名：iic_start
//功能：启动I2C总线，即发送I2C起始条件，形参：无，返回值：无
void IIC_Start()
{
    SDA = 1;    //数据线高电平
    SCL = 1;    //时钟线高电平
    Delay4us(); //起始条件建立时间需要大于4.7μs，所以调用延时函数
    SDA = 0;    //在时钟线高电平时，SDA数据线从高电平转为低电平一次跳变，则I2C通信开始启动
    Delay4us();
    SCL = 0; //复位时钟线为低电平，这样才能允许接下来修改SDA传输的数据，进行读写数据
}

//5、函数名：iic_stop
//功能：停止I2C总线数据传送，形参：无，返回值：无
void IIC_Stop()
{
    SDA = 0;    //数据线低电平
    SCL = 1;    //时钟线高电平
    Delay4us(); //起始条件建立时间需要大于4.7μs，所以调用延时函数
    SDA = 1;    //在时钟线高电平时，SDA数据线从低电平转为高电平一次跳变，则I2C通信结束
    Delay4us();
    SCL = 0; //复位时钟线为低电平，这样才能允许接下来修改SDA传输的数据，进行读写数据
}

//2、函数名：IICSendByte
//函数功能：主机（即单片机）发送一个字节（一个字节由8位二进制组成）给I2C设备。形参：要发送的数据。返回值：无。
void IIC_SendByte(unsigned char sendData)
{
    unsigned char i = 8;    //由于要向SDA上发送一个字节数据，共8位，所以定义循环计数变量i
    for (i = 0; i < 8; i++) //循环移入8位
    {
        SDA = (bit)(sendData & 0x80); //此时前面IIC_start时，SCL为低电平，这样才可以放入数据到SDA
        //因为SDA首先传送的是最高位，所以要屏蔽剩下的低7位，与0x80做“&与运算”
        //（bit）为强制将发送数据与运算之后的结果转换为位（即为1或0），最后把其存入SDA数据线
        SCL = 1;        //再把SCL设为1，让SDA数据线上的数据稳定，完成数据的发送
        Delay4us();     //等待4个机器周期后，确保已经发送完毕
        SCL = 0;        //复位SCL为0，这样下一次才能把数据存入SDA数据线中
        sendData <<= 1; //把要发送的数据左移1位，继续与0x80做与运算，这样实现逐个把数据（即8位二进制）分8次完成发送
    }
    Delay4us();
}

//4、函数名：check_ACK
//函数功能：主机（即单片机）在发完一节数据后要读取的I2C设备反馈的应答信号。形参：无。返回值：无。
void check_ACK()
{
    SDA = 1;         //此时I2C设备强制设为读取状态，即单片机读取I2C的应答信号，所以向SDA写入1。而SDA写入0，是写入数据到PCF8591芯片
    SCL = 1;         //将时钟线置为1，可以稳定SDA的数据，进行数据读取
    Delay4us();      //等待读取数据的完成
    IIC_ERROR = SDA; //把此时读取的SDA信号存入IIC_ERROR变量，如果IIC_ERROR存的值为1，表示出现应答错误，如果是0，则正常应答通信
    SCL = 0;         //复位时钟线为低电平，这样才能允许接下来修改SDA传输的数据
    Delay4us();
}

//发送一个8位数并DA转换成模拟量输出
void DAC_PCF8591(unsigned char controlByte, unsigned char writeData)
{
    IIC_Start();               //1、启动通信
    IIC_SendByte(0x90);        //2、发送写地址
    check_ACK();               //3、每次发送一个字节就要检查应答位
    if (IIC_ERROR == 1)        //根据应答信号的反馈值判断是否应答失败，如果错误变量为1
        return;                //则返回0，结束整个系统程序
    IIC_SendByte(controlByte); //4、发送控制字节
    check_ACK();               //每次发送一个字节就要检查应答位
    if (IIC_ERROR == 1)
        return;
    IIC_SendByte(writeData); //5、发送数字量
    check_ACK();             //每次发送一个字节就要检查应答位
    if (IIC_ERROR == 1)
        return;
    IIC_Stop(); //6、结束通信
}

// 向 PCF8591发送若干字节进行DA转换并输出锯齿波
//主程序
void main()
{
    unsigned char i;
    unsigned int j;
    while (1)
    {
        for (i = 0; i < 255; i++)
        {
            DAC_PCF8591(0x40, i); //把控制字和数字量赋值给DA转换的函数
            for (j = 0; j < 3000; j++)
                ; //每次延时约30ms
        }
        for (i = 255; i > 0; i--)
        {
            DAC_PCF8591(0x40, i);
            for (j = 0; j < 3000; j++)
                ; //每次延时约30ms
        }
    }
}