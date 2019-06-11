#ifndef _SYS_DRIVER_H_
#define _SYS_DRIVER_H_

#include <iom168v.h>
#include <AVRdef.h>
#include "mytype.h"



#define SYS_fosc 16000000


//extern uchar UART_RxData;
//extern uchar UART_TxData;

void eeprom_W(uchar w_dat,uint add);
uchar eeprom_R (uint add);

void  delay(uchar time);
void  ADC_Init(void);
void  ADC_CH(uchar CH);
void  IO_Init(void);

void  UART_Init(uint Bual,uchar TXinteEN,uchar RXinteEN);
uchar UART_Send(void);  //串口发送，返回0完成
uchar UART_Rece(void);

void UART_Send_Wait(uchar d);


#endif
