#include "Sys_Init.h"

static const uchar Sys_String[]="Flag iBasic";


void delay(uchar time)  //1MS
{
uchar i,k,j;
for(k=0;k<time;k++)
{for(j=0;j<16;j++)
{for(i=0;i<248;i++);}}
}

//--------------------------------------------------------------
void eeprom_W(uchar w_dat,uint add)
{
EEARH=(add>>8)&0X0F;
EEARL=add;
EEDR=w_dat;

EECR=0x04;           //对EEMWN写1,启动电源//对EEMWN写0
EECR|=0x02;          //对EEWN写1，启动写
while(EECR&0x02);    //等待的写完成
}


uchar eeprom_R (uint add)
{
//uchar i;
EEARH=(add>>8)&0X0F;
EEARL=add;
while(EECR&0x02);    //等待的写完成
EECR=0x01;          //对EERN写1，启动读
//for(i=0;i<10;i++);
return(EEDR);
}

void IO_Init(void)
{
DDRB=0;
DDRC=0;
DDRD=0;
PORTB=0xff;
PORTC=0xff;
PORTD=0xff;
//----------未用的脚上拉内部电阻---------

}



void ADC_Init(void)
{

//-AD初始化
ADMUX  =0x60;  //AVCC为参考电压，  选ADC0为AD通道

ADCSRA=0;
ADCSRA|=0x04;  //转化时钟分频系数
ADCSRA|=1<<5;  //为1则连续转化
ADCSRA|=1<<7;  //开启ADC电源
ADCSRA|=1<<6;  //启动转化

//一些需要悬空采样的ADC脚
DDRC&=~(1<<0);  PORTC&=~(1<<0);
DDRC&=~(1<<1);  PORTC&=~(1<<1);
DDRC&=~(1<<2);  PORTC&=~(1<<2);
DDRC&=~(1<<3);  PORTC&=~(1<<3);
DDRC&=~(1<<4);  PORTC&=~(1<<4);
DDRC&=~(1<<5);  PORTC&=~(1<<5);

//DDRC&=~(1<<BIT6);  PORTC&=~(1<<BIT6);
//DDRC&=~(1<<BIT7);  PORTC&=~(1<<BIT7);
//--------------------------------
}

void ADC_CH(uchar CH)
{
ADMUX&=0x60;
ADMUX|=(CH&0x07);
}

/*
UART 初始化
入口：波特率，1 发射中断使能，1 接收中断使能
*/
void UART_Init(uint Bual,uchar TXinteEN,uchar RXinteEN)
{
uint i=0;
i=SYS_fosc/16/Bual-1;
UBRR0H  =(i>>8);
UBRR0L  = i;

UCSR0B = (1<<RXEN0) | (1<<TXEN0);
if(TXinteEN!=0)  UCSR0B|=1<<5;
if(RXinteEN!=0)  UCSR0B|=1<<7;
}



/*uchar UART_Send(void)  //串口发送，返回0完成
{
static uchar onlyone=0;
if( onlyone==0)
{
UDR0=UART_TxData;
onlyone=1;
}

if(UCSR0A&(1<<UDRE0))
{onlyone=0;return(0);}
else
return(1);
}*/

void UART_Send_Wait(uchar d)
{
UDR0=d;                       //启动UART
while(!(UCSR0A&(1<<UDRE0)));    //---等待接发射完成
}

/*uchar UART_Rece(void)
{
if(UCSR0A&(1<<RXC0))
{
UART_RxData=UDR0;
return(0);
}
return(1);
}*/
