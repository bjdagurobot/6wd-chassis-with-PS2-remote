
#ifndef _UART_nRF_Bridge_H_
#define _UART_nRF_Bridge_H_

#include <iom168v.h>
#include "AVRdef.h" 
#include "mytype.h"
#include   "PS2.H"
#include   "Servo.h"

#define  LED1_out   DDRC|=(1<<4)
#define  LED1_clr   PORTC&=~(1<<4)
#define  LED1_setb  PORTC|=(1<<4)

#define  LED2_out   DDRC|=(1<<5)
#define  LED2_clr   PORTC&=~(1<<5)
#define  LED2_setb  PORTC|=(1<<5)

extern uchar PWM_CR1;
extern uchar PWM_CR2;

void Bridge_Init(void);
uchar RxFinish(uchar *Buf);  //0£ºOK


#endif

