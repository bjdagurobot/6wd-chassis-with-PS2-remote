//--------------------------用于6轮车驱动！--------------------//
#include "MotDriver.h"


void PWM_RST(void)
{
MotDirL_Out; MotDirL_clr;
MotPwmL_Out; MotPwmL_clr;
MotBrkL_Out; MotBrkL_setb;  //为1，刹车

MotDirR_Out; MotDirR_clr;
MotPwmR_Out; MotPwmR_clr;
MotBrkR_Out; MotBrkR_setb; //为1，刹车

TCCR2A=0xf1;        //选择8位相修模式
TCCR2B=0x00;

TCNT2=0X0000;  //计数器值
OCR2A=0X00EF;  //A比较的值
OCR2B=0X00EF;  //B比较的值

TIFR2 =0xff;  //清所有中断标志位
TIMSK2=0X00;  //所有中断使能关闭
}



void PWM_EN(uchar PWM_ON)
{
if(PWM_ON==0)   //关闭PWM
{
  MotBrkL_setb;
  MotBrkR_setb;  //为1，刹车
 TCCR2B=0x00;
 TCNT2=0X0000;  //计数器值
 OCR2A=0X000F;  //A比较的值
 OCR2B=0X000F;  //B比较的值
} 
else
{
	MotBrkL_clr;
	MotBrkR_clr;

	TCNT2=0X0000;   //计数器值
	OCR2A=0X00fa;  //A比较的值
	OCR2B=0X00fa;  //B比较的值
	TCCR2B=0x01;  //8分频
}
}

void motorL_reg(uchar DIR,uchar Empty)
{
uchar i=0;
if(DIR==0)
MotDirL_clr;
else
MotDirL_setb;

i=255-Empty;
//if(i<20) i=0;
OCR2A=i;
}

void motorR_reg(uchar DIR,uchar Empty)
{
uchar i=0;
if(DIR==0)
MotDirR_setb;
else
MotDirR_clr;



i=255-Empty;
//if(i<20) i=0;
OCR2B=i;
}


/*-255 ~ + 255*/
void motorL_Cont(int Empty)
{
if(Empty>255)  Empty=255;
if(Empty<-255) Empty=-255;

if(Empty<0)
motorL_reg(0,-Empty);
else
motorL_reg(1,Empty);
}


/*-255 ~ + 255*/
void motorR_Cont(int Empty)
{

if(Empty>255)  Empty=255;
if(Empty<-255) Empty=-255;

if(Empty<0)
motorR_reg(0,-Empty);
else
motorR_reg(1,Empty);
}