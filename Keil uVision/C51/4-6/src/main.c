#include<REG51.H>
#define uchar unsigned char
uchar code seg[]=
{0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07};
uchar dis[]={0,1,2,3,4,5,6,7};
void delayms(uchar ms)
{
    uchar i;
    while(ms--)
    for(i=0;i<123;i++);
}
void main()
{
    uchar i;
    while(1)
    {
        for(i=0;i<8;i++)
        {
            P2=dis[i];
            P0=seg[i];
            delayms(1);
        }
    }
}