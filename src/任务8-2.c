#include <reg52.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int
//sbit LCD_RS=P2^6;
//sbit LCD_RW=P2^5;
//sbit LCD_EN=P2^7;
sbit SCL = P1 ^ 7;   //I2C时钟引脚
sbit SDA = P1 ^ 6;   //I2C数据输入输出引脚
bit bdata IIC_ERROR; //I2C错误标志位
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
    SDA = 1;
    SCL = 1;
    Delay4us();
    SDA = 0;
    Delay4us();
    SCL = 0;
}
//停止I2C总线
void IIC_Stop()
{
    SDA = 0;
    SCL = 1;
    Delay4us();
    SDA = 1;
    Delay4us();
    SCL = 0;
}
//发送一字节
void IIC_SendByte(uchar wd)
{
    uchar i;
    for (i = 0; i < 8; i++) //循环移入8位
    {
        SDA = (bit)(wd & 0x80);
        _nop_();
        _nop_();
        SCL = 1;
        Delay4us();
        SCL = 0;
        wd <<= 1;
    }
    Delay4us();
    SDA = 1; //释放总线并准备读取应答
    SCL = 1;
    Delay4us();
    IIC_ERROR = SDA; //IIC_ERROR=1表示无应答
    SCL = 0;
    Delay4us();
}

//发送一个8位数并DA转换成模拟量输出
void DAC_PCF8591(uchar CtrlByte, uchar wdata)
{
    IIC_Start();
    IIC_SendByte(0x90); // 发送写地址
    if (IIC_ERROR == 1)
        return;
    IIC_SendByte(CtrlByte); //发送控制字节
    if (IIC_ERROR == 1)
        return;
    IIC_SendByte(wdata);
    if (IIC_ERROR == 1)
        return;
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
