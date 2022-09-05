#include <reg51.h>
#define uchar unsigned char
#define uint unsigned int
sbit L1=P1^0;
sbit L2=P1^1;
sbit L3=P1^2;
sbit L4=P1^3;          // 定义4根列线 //
 uchar  jyw[16]={0xc0,0xf9,0xa4,0xb0,            //数码管显示值０－Ｆ对应的段码值//
                 0x99,0x92,0x82,0xf8,
                 0x80,0x90,0x88,0x83,
                 0xc6,0xa1,0x86,0x8e};

void delayms(uchar ms)                   //延时子程序//
 {
   uint j;
   while(ms--)
   for(j=0;j<123;j++);
 }

   main()                      //主程序//
   {
    uchar temp;
    uchar i;
    while(1)
   {
          	  	  P1=0xef;             //行扫描初值//       1110 1111
                  for(i=0;i<=3;i++)  //按键阵行扫描,一共四行 //
                {  
				   if (L1==0) P0=jyw[i*4+0];        //检测第一列有无按键按下，若有，对应的键值分别为0（一行）,4（二行）,8（三行），C（四行）//
                   if (L2==0) P0=jyw[i*4+1];        //检测第二列有无按键按下，若有，对应的键值分别为1（一行）,5（二行）,9（三行），d（四行）//
                   if (L3==0) P0=jyw[i*4+2];        //检测第三列有无按键按下，若有，对应的键值分别为2（一行）,6（二行）,A（三行），E（四行）//
                   if (L4==0) P0=jyw[i*4+3];        //检测第四列有无按键按下，若有，对应的键值分别为3（一行）,7（二行）,b（三行），F（四行）//
                   delayms(10);
                   temp=P1;							//temp=1110 1111
                   temp=temp|0x0f;					//temp=1110 1111
                   temp=temp<<1;					// temp=1101 1110
                   temp=temp|0x0f;					// temp=1101 1111
                   P1=temp;                       //下一行行扫描的值，将P1.0-P1.3的值左移一位//	P1=1101 1111

                }

            }
}
