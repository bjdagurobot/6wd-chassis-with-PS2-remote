#include "Servo.h"


//-----------------------舵机控制部分------------------

uchar PWM_CR1=150;
uchar PWM_CR2=150;

void Servo_Init(void)
{
PWM_OUT1_out;   PWM_OUT1_clr;
PWM_OUT2_out;   PWM_OUT2_clr;

TCCR1A=2;       //9位快速PWM模式，但管脚未连接
OCR1A=312;      //比较中断时间：1024/16M*312=20ms
TCCR1B=5;       //T1为1024的分频！一个64US的计数
TCNT1=0;

TIMSK1=0x00;
TIMSK1|=0x02;   //T1 比较中断
SREG|=0x80;     //全局中断使能
}


#pragma interrupt_handler T1_fun:12    //T0中断
void T1_fun(void)       //20ms中断一次
{
uchar i,k;
uchar ser_num=0;
uchar ser_end1=0;
uchar ser_end2=0;

SREG|=0x80;    //中断使能，中断嵌套
TCNT1=0;
PWM_OUT1_setb;
PWM_OUT2_setb;

/*
4个舵机16M下，一次循环时间为13us!
所以 75次为75*20=1500us
*/
for(k=0;k<250;k++)        //最大判断时间
{
	for(i=0;i<25;i++);    //10US,16M下 for(i=0;i<x;i++) 的时间为0.25 x uS 
	ser_num++;
	if((ser_end1==1)&&(ser_end2==1)) break;
	
	if(ser_num>PWM_CR1)
	{PWM_OUT1_clr; ser_end1=1;}
	
	if(ser_num>PWM_CR2)
	{PWM_OUT2_clr; ser_end2=1;}

}

PWM_OUT1_clr;
PWM_OUT2_clr;

BufRuning();

return;
}

//-----------------------------------------------------------------



void Servo_Cont(uchar num,uchar Ag)
{

if(num==1)
{

//PWM_CR1=(uint)Ag*14/18+80;
PWM_CR1=Ag+60;

}

else if(num==2)
{

PWM_CR2=Ag+60;

}


}