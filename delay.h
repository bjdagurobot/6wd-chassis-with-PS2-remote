#ifndef _DELAY_H_
#define _DELAY_H_


#define xtal 8 //��MHzΪ��λ����ͬ��ϵͳʱ��Ҫ�޸ġ�


void delay_nms(unsigned int num) 
 { 
	unsigned int i,j;
	for(i=0;i<num;i++) 
	{
	for(j=0;j<(unsigned int)(xtal*143-2);j++);	
	}
}

#endif
