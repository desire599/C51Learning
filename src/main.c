//����8-1�ɵ���̨��
//���ܣ�����PCF8591оƬ����DAת�������ģ���ѹ����̨�Ƶ�����
#include <reg51.h>
#include <INTRINS.H> //����_nop_()����
sbit SDA = P2 ^ 7;     //P2.7����I2C����������
sbit SCL = P2 ^ 6;     //P2.6����I2Cʱ��������
sbit S1 = P1 ^ 1;      //P1.1���ư�������������
sbit S2 = P1 ^ 0;      //P1.0���ư��������ȼ�С
bit bdata SystemError; //�ӻ������־λ
//��������������������PCF8591ר�ñ������塪��������������������������
#define PCF8591_WRITE 0x90 //����д��ַ������ο�оƬʹ���ֲ�
#define PCF8591_READ 0x91  //��������ַ

void delayNOP()//������ʱ��������ִ�����Ϊ4��������ָ��Ӷ�ʵ����ʱ4���������ڣ���12MHz������Լ4��s
{
  _nop_();//������һ������ʱ��Ϊ1����������
  _nop_();
  _nop_();
  _nop_();
} 
//��������iic_start
//���ܣ�����I2C���ߣ�������I2C��ʼ�������βΣ��ޣ�����ֵ����
void iic_start()
{
  EA = 0;  //���ж�
  SDA = 1; //ʱ�ӱ��ָߵ�ƽ�������ߴӸߵ���һ�����䣬I2Cͨ�ſ�ʼ
  SCL = 1;
  delayNOP(); //��ʼ��������ʱ����Ҫ����4.7��s�����Ե�����ʱ����
  SDA = 0;
  delayNOP(); //��ʼ��������ʱ����Ҫ����4.7��s�����Ե�����ʱ����
  SCL = 0;    //ǯסI2C���ߣ�׼�����ͻ��������
}

//��������iic_stop
//���ܣ�ֹͣI2C�������ݴ��ͣ��βΣ��ޣ�����ֵ����
void iic_stop()
{
  SDA = 0; //ʱ�ӱ��ָߣ������ߴӵ͵���һ�����䣬I2Cͨ��ֹͣ
  SCL = 1;
  delayNOP(); //��ʼ��������ʱ����Ҫ����4.7��s�����Ե�����ʱ����
  SDA = 1;
  delayNOP(); //��ʼ��������ʱ����Ҫ����4.7��s�����Ե�����ʱ����
  SCL = 0;    //ǯסI2C���ߣ�׼�����ͻ��������
}

//��������slave_ACK
//�������ܣ��ӻ�����Ӧ��λ���βΣ��ޣ�����ֵ����
void slave_ACK()
{
  SDA = 0;
  SCL = 1;
  delayNOP();
  SDA = 1;
  SCL = 0;
}

//��������slave_NOACK
//�������ܣ��ӻ����ͷ�Ӧ��λ����ʹ���ݴ�����̽������βΣ��ޡ�����ֵ����
void slave_NOACK()
{
  SDA = 1;
  SCL = 1;
  delayNOP();
  SDA = 0;
  SCL = 0;
}

//��������check_ACK
//�������ܣ�����Ӧ��λ��飬��ʹ���ݴ�����̽������βΣ��ޡ�����ֵ���ޡ�
void check_ACK()
{
  SDA = 1; //��I/O���ó����룬��������˿�д1
  SCL = 1;
  F0 = 0;
  if (SDA == 1) //��SDA=1������Ӧ����λ��Ӧ���־F0
  {
    F0 = 1;
  }
  SCL = 0;
}

//��������IICSendByte
//�������ܣ�����һ���ֽڣ�һ���ֽ���8λ��������ɣ����βΣ�Ҫ���͵����ݡ�����ֵ���ޡ�
void IICSendByte(unsigned char sData)
{
  unsigned char idata n = 8; //��SDA�Ϸ���һ���ֽ����ݣ���8λ
  while (n--)
  {
    if ((sData & 0x80) == 0x80) //�ж�Ҫ���͵��������λ�Ƿ�Ϊ1
    {
      SDA = 1; //������㣬����λ��Ϊ1
      SCL = 1;
      delayNOP();
      SDA = 0;
      SCL = 0;
    }
    else
    {
      SDA = 0; //��������㣬����λ��Ϊ0
      SCL = 1;
      delayNOP();
      SCL = 0;
    }
    sData = sData << 1; //��Ҫ��������ݵ�λ����һλ������һ��ѭ���ж�
  }
}

//������IICReceiveByte
//�������ܣ�����һ���ֽ����ݡ��βΣ��ޡ�����ֵ�����ؽ��յ�����
unsigned char IICReceiveByte()
{
  unsigned char idata n = 8; //��SDA���϶�ȡһ���ֽ����ݣ���8λ
  unsigned char rData;       //���������ݵı���rData
  while (n--)
  {
    SDA = 1;
    SCL = 1;
    rData = rData << 1; //�ѽ������ݵ�����1λ
    if (SDA == 1)
    {
      rData = rData | 0x01; //�����յ���λΪ1�������ݵ����һλ��Ϊ1
    }
    else
    {
      rData = rData & 0xfe; //��������ݵ����һλ����
    }
    SCL = 0;
  }
  return rData;
}

//��������DAC_PCF8591
//�������ܣ�����nλ����
//�βΣ�controlΪ�����ִ�ű�����wDataΪҪת����������������ֵ����
void DAC_PCF8591(unsigned char controlByte, unsigned char wData)
{
  iic_start();                //����I2Cͨ��
  IICSendByte(PCF8591_WRITE); //���͵�ַλ
  check_ACK();                //���Ӧ��λ
  if (F0 == 1)                //�����Ӧ���־Ϊ1����ʾ����������ѻ����ô����־λSystemErrorΪ1
  {
    SystemError = 1;
    return; //�˾��Ƿ�ɲ�д
  }
  IICSendByte(controlByte & 0x77); //0x77=0111 0111�����ε�4�͵�8λ
  check_ACK();
  if (F0 == 1) //�����Ӧ���־Ϊ1����ʾ����������ѻ����ô����־λSystemErrorΪ1
  {
    SystemError = 1;
    return; //�˾��Ƿ�ɲ�д
  }
  IICSendByte(wData);
  check_ACK();
  if (F0 == 1) //�����Ӧ���־Ϊ1����ʾ����������ѻ����ô����־λSystemErrorΪ1
  {
    SystemError = 1;
    return; //�˾��Ƿ�ɲ�д
  }
  iic_stop();
  delayNOP();
  delayNOP();
  delayNOP();
  delayNOP();
}

//��������delay_ms
//���ܣ����ö�ʱ��T1��ʱt���룬���ù�����ʽ1����ʱ����ʼֵΪ64536��ÿ�μ�ʱ1ms����1000�Σ�
//�βΣ���ʱ��������С��255ms��������ֵ����
void delay_ms(unsigned char t)
{
  unsigned char i; //ѭ����������
  TMOD = 0x10;     //����T1��ʱ����������ʽΪ1��16λ������
  for (i = 0; i < t; i++)
  {
    TH1 = (65536 - 1000) / 256; //���ö�ʱ��������������ʼֵ
    TL1 = (65536 - 1000) % 256;
    TR1 = 1; //������ʱ��T1
    while (!TF1)
      ;      //��ѯ�����Ƿ��������������еȴ�
    TF1 = 0; //1ms��ʱ���ˣ��������־λ����
  }
}

void main()
{
  unsigned char voltage; //�����ѹֵ�ı�����������0��Ӧ0.0v��255��Ӧ+5.0v
  voltage = 125;         //��ʼ��ѹֵԼΪ2.5V������LED���г�ʼ����
  while (1)
  {
    DAC_PCF8591(0x40, voltage); //�Կ�����д0100 0000��ʾ����ģ�����������д��������voltage
    if (S1 == 0)                //����S1���£�̨����������
    {
      delay_ms(10); //������ʱ����
      if (S1 == 0)
      {
        if (voltage == 255)
          voltage = 125; //����ѹ�䵽���ʱ���ָֻ��м�ֵ
        else
          voltage = voltage + 5; //ÿ������5
      }
    }
    if (S2 == 0) //����S2���£�̨�����ȼ�С
    {
      delay_ms(10); //������ʱ����
      if (S2 == 0)
      {
        if (voltage == 0)
          voltage = 125; //����ѹ�䵽���ʱ���ָֻ��м�ֵ
        else
          voltage = voltage - 5; //ÿ�μ�С5
      }
    }
    delay_ms(1); //ÿ����ʱ1ms
  }
}