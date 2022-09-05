#include<REG51.H>
#define uchar unsigned char
#define uint unsigned int
sbit RS=P1^0;
sbit RW=P1^1;
sbit E=P1^2;
uchar code str[]="0~99 stopwatch:";
uchar sec=0;
uchar con=0;
void delayms(uint ms)           //延时子函数
{
    uchar i;
    while(ms--)
    for(i=0;i<123;i++);
}
void w_com(uchar com)           //写指令寄存器函数
{
    RS=0;
    RW=0;
    E=1;
    P0=com;
    E=0;
    delayms(1);
}
void w_dat(uchar dat)           //写数据寄存器函数
{
    RS=1;
    RW=0;
    E=1;
    P0=dat;
    E=0;
    delayms(1);
}
void lcd_ini(void)          //LCD初始化函数
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
void main(void)
{
    uchar i,num;
    lcd_ini();
    w_com(0x80);
    for(i=0;str[i]!='\0';i++)
    w_dat(str[i]);
    while(1)
    {
        w_com(0xc7);
        w_dat(num/10+0x30);
        w_dat(num%10+0x30);
        delayms(1000);
        if(++num==100)num=0;
    }
}