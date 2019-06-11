#include <iom168v.h>
#include "AVRdef.h" 
#include "mytype.h"
#include "MotDriver.h" 
#include "Sys_Init.h"
#include "Servo.h"
#include "filtering.h"
#include "UART_nRF_Bridge.h" 

#define  LED1_out   DDRC|=(1<<4)
#define  LED1_clr   PORTC&=~(1<<4)
#define  LED1_setb  PORTC|=(1<<4)

#define  LED2_out   DDRC|=(1<<5)
#define  LED2_clr   PORTC&=~(1<<5)
#define  LED2_setb  PORTC|=(1<<5)

struct RunParam RunParam_Reg; //定义一个全局变量

uchar UART_Buf[5]; 
uchar RxFinish_Mark=0;

uchar a;

//PS2变量
uint CNT=0;
uchar ContBuf[32];
uchar buf[32];

void main(void)
{
uint i=0;
uint j=0;

IO_Init();

LED1_out; LED1_clr;
LED2_out; LED2_clr;

CLI();

//电机初始化
PWM_RST();
PWM_EN(1);

motorL_Cont(0);
motorR_Cont(0);

//舵机初始化
Servo_Init();

//缓冲器初始化
SetRunEn(0,1,128);
SetRunEn(1,1,128);
BufSetRun(0,128,1,1);
BufSetRun(1,128,1,1);

//串口初始化
UART_Init(9600,0,1);

//PS2遥控器初始化
Bridge_Init();

SEI();


while(1)
{

	//PS2遥控
	if(CNT++>20000)
	{
	CNT=0;
		if(RxFinish(buf)==0) //判断接收完成？
		{
		 for(i=0;i<8;i++) ContBuf[i]=buf[i];
		 
			if( (ContBuf[0]==0x5a)  &&  (ContBuf[7]==0xa5) ) //有效的头和尾
			{
				/*
				ContBuf[1] ：左轮方向 0/1
				ContBuf[2] ：左轮速度 0~100
				ContBuf[3] ：左轮速度变化率1~10; 10ms变化的级数
				
				ContBuf[4] ：右轮方向 0/1
				ContBuf[5] ：右轮速度 0~100	
				ContBuf[6] ：右轮速度变化率1~10;10ms变化的级数
				*/
				if(ContBuf[2]>100) ContBuf[2]=100;
				if(ContBuf[5]>100) ContBuf[5]=100;
				
				if(ContBuf[3]==0) ContBuf[3]=1;
				if(ContBuf[3]>10) ContBuf[3]=10;
				
				if(ContBuf[6]==0) ContBuf[6]=1;
				if(ContBuf[6]>10) ContBuf[6]=10;			
				
				ContBuf[2]=((ulong)ContBuf[2] * 116)/100;  //比例变化，变化后范围：0~125
				ContBuf[5]=((ulong)ContBuf[5] * 116)/100;  //比例变化，变化后范围：0~125
				
				BufSetRun(0,(ContBuf[1])?(ContBuf[2]+128):(128-ContBuf[2]),1,ContBuf[3]); 
				BufSetRun(1,(ContBuf[4])?(ContBuf[5]+128):(128-ContBuf[5]),1,ContBuf[6]); 
			}	                    		
		}
	
	
	}
	
	
 }
 
}
uchar buf_FIFO[5];
void FIFO(uchar inD)
{

buf_FIFO[0]=buf_FIFO[1];
buf_FIFO[1]=buf_FIFO[2];
buf_FIFO[2]=buf_FIFO[3];
buf_FIFO[3]=buf_FIFO[4];
buf_FIFO[4]=inD;

}

#pragma interrupt_handler UART0_RX_Inte:19   //UART接收完成中断
void UART0_RX_Inte(void)
{
uchar i;
uchar rd=UDR0;

FIFO(rd);

if( (buf_FIFO[0]==0xff) && (buf_FIFO[4]==0xff))
{
RxFinish_Mark=1;

for(i=0;i<5;i++) UART_Buf[i]=buf_FIFO[i];

}


}