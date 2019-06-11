#ifndef _168_AVR_PWM_H_
#define _168_AVR_PWM_H_

#include <iom168v.h>
#include <AVRdef.h> 
#include "mytype.h"

#define  PWM_OUT1_out   DDRD|=(1<<5)
#define  PWM_OUT1_clr   PORTD&=~(1<<5)
#define  PWM_OUT1_setb  PORTD|=(1<<5)

#define  PWM_OUT2_out   DDRB|=(1<<5)
#define  PWM_OUT2_clr   PORTB&=~(1<<5)
#define  PWM_OUT2_setb  PORTB|=(1<<5)


#define PWM_CH1_MAX  240
#define PWM_CH1_MIN  60
 
#define PWM_CH2_MAX  240
#define PWM_CH2_MIN  60


void Servo_Init(void);
void Servo_Cont(uchar num,uchar Ag);
#endif
