/*汽车转向灯控制
要求：按下左转开关，左转灯一直闪烁；按下右转开关，右转灯一直闪烁；拨到中间，两灯熄灭
*/
#include <REG52.H>
sbit L_LED = P1 ^ 0;//左转向灯
sbit R_LED = P1 ^ 1;//右转向灯
sbit L_SW = P3 ^ 0;//左转灯开关
sbit R_SW = P3 ^ 1;//右转灯开关
void delay(unsigned int i) //延时函数
{
    while (i--);
}
void main()
{
    while (1)
    {
        if(L_SW==0) //当左转开关打开（由于这种开关是一直接通的，可以不再增加while语句）
        {
            L_LED = ~L_LED;//对左转向灯状态取反
            delay(1000);//延时约100ms
        }
        else if(R_SW==0)//当右转开关打开
        {
            R_LED = ~R_LED;//对右转向灯状态取反
            delay(1000);//延时约100ms
        }
        else //其余状态两个灯熄灭
            L_LED = R_LED = 1;
    }
}