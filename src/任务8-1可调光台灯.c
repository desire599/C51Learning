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
//函数功能：