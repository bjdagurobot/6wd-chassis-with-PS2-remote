#include "Servo.h"


//-----------------------������Ʋ���------------------

uchar PWM_CR1=150;
uchar PWM_CR2=150;

void Servo_Init(void)
{
PWM_OUT1_out;   PWM_OUT1_clr;
PWM_OUT2_out;   PWM_OUT2_clr;

TCCR1A=2;       //9λ����PWMģʽ�����ܽ�δ����
OCR1A=312;      //�Ƚ��ж�ʱ�䣺1024/16M*312=20ms
TCCR1B=5;       //T1Ϊ1024�ķ�Ƶ��һ��64US�ļ���
TCNT1=0;

TIMSK1=0x00;
TIMSK1|=0x02;   //T1 �Ƚ��ж�
SREG|=0x80;     //ȫ���ж�ʹ��
}


#pragma interrupt_handler T1_fun:12    //T0�ж�
void T1_fun(void)       //20ms�ж�һ��
{
uchar i,k;
uchar ser_num=0;
uchar ser_end1=0;
uchar ser_end2=0;

SREG|=0x80;    //�ж�ʹ�ܣ��ж�Ƕ��
TCNT1=0;
PWM_OUT1_setb;
PWM_OUT2_setb;

/*
4�����16M�£�һ��ѭ��ʱ��Ϊ13us!
���� 75��Ϊ75*20=1500us
*/
for(k=0;k<250;k++)        //����ж�ʱ��
{
	for(i=0;i<25;i++);    //10US,16M�� for(i=0;i<x;i++) ��ʱ��Ϊ0.25 x uS 
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