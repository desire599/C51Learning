#include <reg52.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int
//sbit LCD_RS=P2^6;
//sbit LCD_RW=P2^5;
//sbit LCD_EN=P2^7;
sbit SCL = P2 ^ 6;   //I2C时钟引脚
sbit SDA = P2 ^ 7;   //I2C数据输入输出引脚
bit bdata IIC_ERROR; //I2C错误标志位，其中IIC_ERROR为自定义变量名
//延时4微秒函数
void Delay4us()
{
    _nop_();
    _nop_();
    _nop_();
    _nop_();
}

//启动I2C总线
void IIC_Start()
{
    SDA = 1; //数据线高电平
    SCL = 1; //时钟线高电平
    Delay4us();
    SDA = 0; //在时钟线高电平时，数据线从高电平转为低电平则I2C通信开始启动
    Delay4us();
    SCL = 0; //复位时钟线为低电平，这样才能允许修改SDA传输的数据
}
//停止I2C总线
void IIC_Stop()
{
    SDA = 0; //数据线低电平
    SCL = 1; //时钟线高电平
    Delay4us();
    SDA = 1; //在时钟线高电平时，数据线从低电平转为高电平则I2C通信停止
    Delay4us();
    SCL = 0; //复位时钟线为低电平，这样才能允许修改SDA传输的数据
}
//发送一字节
void IIC_SendByte(uchar wd)
{
    uchar i;
    for (i = 0; i < 8; i++) //循环移入8位
    {
        SDA = (bit)(wd & 0x80); //此时前面SCL为低电平，再把要发送的数据的最高位存入SDA数据线
        _nop_();
        _nop_();
        SCL = 1;//再把SCL设为1，让SDA数据线上的数据稳定，完成进行发送
        Delay4us();//等待4个机器周期后，确保已经发送完毕
        SCL = 0;//复位SCL为0，这样下一次才能把数据存入SDA数据线中
        wd <<= 1;
    }
    Delay4us();
    SDA = 1; //释放总线并准备读取应答
    SCL = 1;
    Delay4us();
    IIC_ERROR = SDA; //把此时读取的SDA信号存入IIC_ERROR变量，如果IIC_ERROR存的值为1，表示出现应答错误，如果是0，则正常应答通信
    SCL = 0;
    Delay4us();
}

//发送一个8位数并DA转换成模拟量输出
void DAC_PCF8591(uchar CtrlByte, uchar wdata)
{
    IIC_Start();
    IIC_SendByte(0x90); // 发送写地址
    if (IIC_ERROR == 1)
        return 0;
    IIC_SendByte(CtrlByte); //发送控制字节
    if (IIC_ERROR == 1)
        return 0;
    IIC_SendByte(wdata);
    if (IIC_ERROR == 1)
        return 0;
    IIC_Stop(); //收到一个字节后发送一个非应答位
}
// 向 PCF8591发送若干字节进行DA转换并输出锯齿波
//主程序
void main()
{
    uchar i;
    while (1)
    {
        for (i = 0; i < 255; i++)
            DAC_PCF8591(0x40, i);
        for (i = 255; i > 0; i--)
            DAC_PCF8591(0x40, i);
    }
}
