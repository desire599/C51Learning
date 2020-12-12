/*------显示字符串文字的LCD1602函数---*/
//调用方法：char_display("QING YUAN");
void char_display(unsigned char display[]) //形参为字符串数组
{
    unsigned char i=0;//循环计数变量
    while(display[i]!='\0')//只要没到字符串的最后一个元素就执行，因为字符串数组自动在末尾加了\0
    {
        lcd_w_dat(display[i]);//每次把单个字符串元素写入到LCD
        i++;//依次累加
    }
}