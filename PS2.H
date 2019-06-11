#ifndef __PS2_H__
#define __PS2_H__			    

#include  <iom168v.h>
#include  <AVRdef.h> 
#include  "mytype.h"



#define PS2_sck_setb   		PORTB|=(1<<4)
#define PS2_sck_clr    		PORTB&=~(1<<4)
#define PS2_sck_outmode     DDRB|=(1<<4) 

#define PS2_CS_out_setb   PORTB|=(1<<0)
#define PS2_CS_out_clr    PORTB&=~(1<<0)
#define PS2_CS_outmode    DDRB|=(1<<0) 


#define  PS2_mosi_setb  	PORTD|=(1<<7)
#define  PS2_mosi_clr    	PORTD&=~(1<<7)
#define  PS2_mosi_outmode   DDRD|=(1<<7) 

#define  PS2_miso_setb   	 (PORTD|=(1<<6))
#define  PS2_miso_inmode     (DDRD&=~(1<<6))
#define  PS2_miso_indat 	 (PIND&(1<<6))


//-------------按键值定义-------------

#define _PS2_Key_Dir_U  0x0010
#define _PS2_Key_Dir_D  0x0040
#define _PS2_Key_Dir_L  0x0080
#define _PS2_Key_Dir_R  0x0020

#define _PS2_Key_Butt_U  0x1000
#define _PS2_Key_Butt_D  0x4000
#define _PS2_Key_Butt_L  0x8000
#define _PS2_Key_Butt_R  0x2000

#define _PS2_Key_SideL_1 0x0400
#define _PS2_Key_SideL_2 0x0100

#define _PS2_Key_SideR_1 0x0800
#define _PS2_Key_SideR_2 0x0200

#define _PS2_Key_SELECT  0x0001
#define _PS2_Key_MODE    0x0008

void   PS2_Init(void);          
uchar  Get_PS2Dat(uchar *buf);   
uint  PS2_ChangeKey(uchar *inBuf);
#endif


