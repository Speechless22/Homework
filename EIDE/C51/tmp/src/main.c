#include<reg51.h>
#define uchar unsigned char 
sbit S1=P1^0;
uchar code seg[]=
{0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
void delayms(uchar ms)
{
   uchar i;
   while(ms--)
   for(i=0;i<123;i++);
}
uchar key_scan(void)
{
   static kp=0;
   if((P1&0x01)!=0x01)
   {
        delayms(10);
		if(((P1&0x01)!=0x01)&&(kp==0))
		{
			kp=1;
			return 1;
		}
   }
   else kp=0;
   return 0;
}
void main(void)
{
    uchar key_val;
	uchar num=0;
	while(1)
	{
	    key_val=key_scan();
		if(key_val==1)
		{
			if(++num==10) num=0;
		}
		P0=seg[num];
	}
}