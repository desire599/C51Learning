//声明调用函数
extern unsigned char lcd_r_start();       //1、读状态函数，带返回值
extern void lcd_w_cmd(unsigned char com); //2、写命令字函数
extern void lcd_w_dat(unsigned char dat); //3、写数据函数
extern void lcd_int();                    //4、LCD初始化函数
extern void delay_3us();	                //延时3个机器周期
extern void delay(unsigned int i);        //延时函数