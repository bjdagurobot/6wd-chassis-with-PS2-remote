//--------------------------����6�ֳ�������--------------------//
#include "MotDriver.h"


void PWM_RST(void)
{
MotDirL_Out; MotDirL_clr;
MotPwmL_Out; MotPwmL_clr;
MotBrkL_Out; MotBrkL_setb;  //Ϊ1��ɲ��

MotDirR_Out; MotDirR_clr;
MotPwmR_Out; MotPwmR_clr;
MotBrkR_Out; MotBrkR_setb; //Ϊ1��ɲ��

TCCR2A=0xf1;        //ѡ��8λ����ģʽ
TCCR2B=0x00;

TCNT2=0X0000;  //������ֵ
OCR2A=0X00EF;  //A�Ƚϵ�ֵ
OCR2B=0X00EF;  //B�Ƚϵ�ֵ

TIFR2 =0xff;  //�������жϱ�־λ
TIMSK2=0X00;  //�����ж�ʹ�ܹر�
}



void PWM_EN(uchar PWM_ON)
{
if(PWM_ON==0)   //�ر�PWM
{
  MotBrkL_setb;
  MotBrkR_setb;  //Ϊ1��ɲ��
 TCCR2B=0x00;
 TCNT2=0X0000;  //������ֵ
 OCR2A=0X000F;  //A�Ƚϵ�ֵ
 OCR2B=0X000F;  //B�Ƚϵ�ֵ
} 
else
{
	MotBrkL_clr;
	MotBrkR_clr;

	TCNT2=0X0000;   //������ֵ
	OCR2A=0X00fa;  //A�Ƚϵ�ֵ
	OCR2B=0X00fa;  //B�Ƚϵ�ֵ
	TCCR2B=0x01;  //8��Ƶ
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