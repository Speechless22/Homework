#include<reg51.h>
#define uchar unsigned int
uchar code seg[]=
{0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
void delayms(uchar ms)
{
    uchar i;
    while(ms--)
    for(i=0;i<123;i++);
}

void s()
{
    uchar i;
    for(i=0;i<10;i++)
    {
        P0=seg[i];
        delayms(300);
    }
}

void main()
{
    while(1)
    {
        s();
    }
}