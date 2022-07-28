#include<REG52.H>
#define uchar unsigned char
#define uint unsigned int
sbit RS=P2^0;
sbit RW=P2^1;
sbit E=P2^2;
sbit TLC549_CLK=P2^3;
sbit TLC549_CS=P2^4;
sbit TLC549_DO=P2^5;
void delayms(uchar ms)          //毫秒级延时函数
{
    uchar i;
    while(ms--)
    for(i=0;i<123;i++);
}
void delayus(uchar us)          //微秒级延时函数
{while(us--);}
void w_com(uchar com)           //LCD1602的写指令寄存器函数
{
    RS=0;
    RW=0;
    E=1;
    P0=com;
    E=0;
    delayms(1);
}
void w_dat(uchar dat)           //LCD1602的写数据寄存器函数
{
    RS=1;
    RW=0;
    E=1;
    P0=dat;
    E=0;
    delayms(1);
}
void lcd_ini(void)          //LCD1602的初始化函数
{
    delayms(10);
    w_com(0x38);
    delayms(10);
    w_com(0x0c);
    delayms(10);
    w_com(0x06);
    delayms(10);
    w_com(0x10);
    delayms(10);
    w_com(0x38);
    delayms(10);
}
uchar TLC549_ADC(void)
{
    uchar i,temp;
    TLC549_CLK=0;
    TLC549_CS=0;
    for(i=0;i<8;i++)
    {
        temp<<=1;
        temp|=TLC549_DO;
        TLC549_CLK=1;
        TLC549_CLK=0;
    }
    TLC549_CS=1;
    delayms(20);
    return temp;
}
void main(void)
{
    uint temp;
    lcd_ini();
    while (1)
    {
        temp=TLC549_ADC();
        w_com(0x84);
        w_dat(temp*195/10000+0x30);
        w_dat('.');
        w_dat(temp*195%10000/1000+0x30);
        w_dat(temp*195%1000/100+0x30);
        w_dat('V');
    }
    
}