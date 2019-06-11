#ifndef _DELAY_H_
#define _DELAY_H_


#define xtal 8 //以MHz为单位，不同的系统时钟要修改。


void delay_nms(unsigned int num) 
 { 
	unsigned int i,j;
	for(i=0;i<num;i++) 
	{
	for(j=0;j<(unsigned int)(xtal*143-2);j++);	
	}
}

#endif
