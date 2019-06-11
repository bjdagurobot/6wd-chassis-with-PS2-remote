#ifndef _168_AVR_PWM_H_
#define _168_AVR_PWM_H_

#include <iom168v.h>
#include <AVRdef.h> 
#include "mytype.h"

#define  MotDirL_Out   DDRB|=(1<<2)
#define  MotDirL_clr   PORTB&=~(1<<2)
#define  MotDirL_setb  PORTB|=(1<<2)

#define  MotPwmL_Out   DDRB|=(1<<3)
#define  MotPwmL_clr   PORTB&=~(1<<3)
#define  MotPwmL_setb  PORTB|=(1<<3)

#define  MotBrkL_Out   DDRB|=(1<<1)
#define  MotBrkL_clr   PORTB&=~(1<<1)
#define  MotBrkL_setb  PORTB|=(1<<1)

#define  MotDirR_Out   DDRD|=(1<<2)
#define  MotDirR_clr   PORTD&=~(1<<2)
#define  MotDirR_setb  PORTD|=(1<<2)

#define  MotPwmR_Out   DDRD|=(1<<3)
#define  MotPwmR_clr   PORTD&=~(1<<3)
#define  MotPwmR_setb  PORTD|=(1<<3)

#define  MotBrkR_Out   DDRD|=(1<<4)
#define  MotBrkR_clr   PORTD&=~(1<<4)
#define  MotBrkR_setb  PORTD|=(1<<4)

void PWM_RST(void);
void PWM_EN(uchar PWM_ON);               //也可以用PWM占空比控制！
void motorL_reg(uchar DIR,uchar Empty);
void motorR_reg(uchar DIR,uchar Empty);

void motorL_Cont(int Empty);
void motorR_Cont(int Empty);


#endif