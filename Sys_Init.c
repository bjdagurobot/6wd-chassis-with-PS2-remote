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

EECR=0x04;           //��EEMWNд1,������Դ//��EEMWNд0
EECR|=0x02;          //��EEWNд1������д
while(EECR&0x02);    //�ȴ���д���
}


uchar eeprom_R (uint add)
{
//uchar i;
EEARH=(add>>8)&0X0F;
EEARL=add;
while(EECR&0x02);    //�ȴ���д���
EECR=0x01;          //��EERNд1��������
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
//----------δ�õĽ������ڲ�����---------

}



void ADC_Init(void)
{

//-AD��ʼ��
ADMUX  =0x60;  //AVCCΪ�ο���ѹ��  ѡADC0ΪADͨ��

ADCSRA=0;
ADCSRA|=0x04;  //ת��ʱ�ӷ�Ƶϵ��
ADCSRA|=1<<5;  //Ϊ1������ת��
ADCSRA|=1<<7;  //����ADC��Դ
ADCSRA|=1<<6;  //����ת��

//һЩ��Ҫ���ղ�����ADC��
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
UART ��ʼ��
��ڣ������ʣ�1 �����ж�ʹ�ܣ�1 �����ж�ʹ��
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



/*uchar UART_Send(void)  //���ڷ��ͣ�����0���
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
UDR0=d;                       //����UART
while(!(UCSR0A&(1<<UDRE0)));    //---�ȴ��ӷ������
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
