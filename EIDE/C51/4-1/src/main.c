#include<REG51.H>
#define uchar unsigned char
sbit LED=P2^0;
sbit S1=P1^0;
sbit S2=P1^1;
void delayms (int ms)
{
    uchar i;
        while (ms--)
        for(i=0;i<123;i++);
}
void main(void)
{
    LED=0;
    while (1)
    {
        P1=0xff;
        if (S1==0)
        {
            delayms (10);
                if (S1==0)
                {
                    LED= ~LED;
                    delayms(100);
                }
        }
        if (S2==0)
        {
            delayms(10);
            if(S2==0)
            LED=1;
        }
    }
}