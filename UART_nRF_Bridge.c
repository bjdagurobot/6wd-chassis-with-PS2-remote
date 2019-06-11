#include "UART_nRF_Bridge.h" 
#include "PS2.H"


uchar IsPhone_STA=0;

uchar DIR_L;	
uchar SPD_L;	
uchar CHA_L;	
uchar DIR_R;	
uchar SPD_R;	
uchar CHA_R;  //电机控制的参数


void Bridge_Init(void)
{
uchar i;
SREG |=0x80;      //全局中断使能
PS2_Init();
}





/*
指针封包，头尾
*/
void PointPacket(uchar *nRFBuf)
{
uchar i;
ulong ld=0;
	
/*
nRF_buf[1] ：左轮方向 0/1
nRF_buf[2] ：左轮速度 0~100
nRF_buf[3] ：左轮速度变化率1~10; 20ms变化的级数
			
nRF_buf[4] ：右轮方向 0/1
nRF_buf[5] ：右轮速度 0~100	
nRF_buf[6] ：右轮速度变化率1~10;20ms变化的级数
*/	
	
nRFBuf[0]=0x5a;	

nRFBuf[1]=DIR_L;	
nRFBuf[2]=SPD_L;	
nRFBuf[3]=CHA_L;	

nRFBuf[4]=DIR_R;	
nRFBuf[5]=SPD_R;	
nRFBuf[6]=CHA_R;	

nRFBuf[7]=0xa5;	
}



/*************************************

		   PS2无线控制方式

**************************************/


// 按键值对应表：
#define Stated_DirU   _PS2_Key_Dir_U
#define Stated_DirD   _PS2_Key_Dir_D
#define Stated_DirL   _PS2_Key_Dir_L
#define Stated_DirR   _PS2_Key_Dir_R //方向控制对应方向键上下左右

#define Stated_SpdU   _PS2_Key_SideL_1
#define Stated_SpdD   _PS2_Key_SideL_2 //速度控制对应侧键12

#define Stated_ChgU    0
#define Stated_ChgD    0  				//变化率未使用，无对应按键

#define Stated_Ser1_U  _PS2_Key_Butt_L
#define Stated_Ser1_D  _PS2_Key_Butt_R
#define Stated_Ser2_U  _PS2_Key_Butt_U
#define Stated_Ser2_D  _PS2_Key_Butt_D //舵机控制对应按键上下左右

//电机的最小启动值，以及最大值（100）

#define MotOperateValue_Min 70
#define MotOperateValue_Max 90

//按键不放时，相应速度递增或递减的速度
#define GradualSpeed_SPD 2

uchar PS2_Last_Down=0;
uchar PS2buf[10];
uchar PS2_SPD=80;
uchar PS2_chg=8; //固定

uchar IsPlayStation(uchar *inP,uchar *OutP)
{
static uchar CNT=0;
uchar  Dir=0;
uchar  k=0;
uint KeyValue=0;

KeyValue=PS2_ChangeKey(inP);

	if(KeyValue & Stated_DirU) {Dir|=0x01;k=1;}
	if(KeyValue & Stated_DirD) {Dir|=0x02;k=1;}
	if(KeyValue & Stated_DirL) {Dir|=0x04;k=1;}
	if(KeyValue & Stated_DirR) {Dir|=0x08;k=1;}

	if(KeyValue & Stated_SpdU) { if(CNT++>GradualSpeed_SPD) {CNT=0; PS2_SPD++;   if(PS2_SPD>MotOperateValue_Max) PS2_SPD=MotOperateValue_Max; }}
	if(KeyValue & Stated_SpdD) { if(CNT++>GradualSpeed_SPD) {CNT=0;if(PS2_SPD>MotOperateValue_Min)  {PS2_SPD--;}         	  }}
	
	if(KeyValue & Stated_ChgU) {if(CNT++>3) {CNT=0;PS2_chg++; 	  if(PS2_chg>10) PS2_chg=10;   }}
	if(KeyValue & Stated_ChgD) {if(CNT++>3) {CNT=0;if(PS2_chg>1)   PS2_chg--; 				   }}
	
	/*------舵机控制--------------*/
    if(KeyValue & Stated_Ser1_U)  {PWM_CR1++;     if(PWM_CR1>PWM_CH1_MAX) PWM_CR1=PWM_CH1_MAX; }
	if(KeyValue & Stated_Ser1_D)  {if(PWM_CR1>PWM_CH1_MIN)  PWM_CR1--;  }
	
	if(KeyValue & Stated_Ser2_U) {PWM_CR2++;     if(PWM_CR2>PWM_CH2_MAX) PWM_CR2=PWM_CH2_MAX; } 
	if(KeyValue & Stated_Ser2_D) { if(PWM_CR2>PWM_CH2_MIN)  PWM_CR2--;    }
	

		if(k==1)  //已经按下
		{

		LED1_setb;

			switch(Dir)
			{
			case 1: //上
					DIR_L=0;SPD_L=PS2_SPD; CHA_L=PS2_chg;
					DIR_R=0;SPD_R=PS2_SPD; CHA_R=PS2_chg; PointPacket(OutP);break;

			case 2:  //下
					DIR_L=1;SPD_L=PS2_SPD; CHA_L=PS2_chg;
					DIR_R=1;SPD_R=PS2_SPD; CHA_R=PS2_chg; PointPacket(OutP);break;

			case 4: //左
					DIR_L=1;SPD_L=PS2_SPD; CHA_L=PS2_chg;
					DIR_R=0;SPD_R=PS2_SPD; CHA_R=PS2_chg; PointPacket(OutP);break;

			case 8: //右
					DIR_L=0;SPD_L=PS2_SPD; CHA_L=PS2_chg;
					DIR_R=1;SPD_R=PS2_SPD; CHA_R=PS2_chg; PointPacket(OutP);break;

			case 5:  //左上
					DIR_L=0;SPD_L=0; CHA_L=PS2_chg;
					DIR_R=0;SPD_R=PS2_SPD; CHA_R=PS2_chg; PointPacket(OutP);break;

			case 9:  //右上
	 				DIR_L=0;SPD_L=PS2_SPD; CHA_L=PS2_chg;
					DIR_R=0;SPD_R=0; CHA_R=PS2_chg; PointPacket(OutP);break;

			case 6: //左下
					DIR_L=1;SPD_L=PS2_SPD; CHA_L=PS2_chg;
					DIR_R=1;SPD_R=0; CHA_R=PS2_chg; PointPacket(OutP);break;

			case 10: //右下
					DIR_L=1;SPD_L=0; CHA_L=PS2_chg;
					DIR_R=1;SPD_R=PS2_SPD; CHA_R=PS2_chg; PointPacket(OutP);break;

			default:
					DIR_L=0;SPD_L=0; CHA_L=PS2_chg;
					DIR_R=0;SPD_R=0; CHA_R=PS2_chg; PointPacket(OutP);break;
			}

		PS2_Last_Down=1;//上次按下了
		return 0;
		}
		else 
		{
		LED1_clr;
			if(PS2_Last_Down)  //上次按下了，放开按键
			{
			PS2_Last_Down=0;

			DIR_L=0;SPD_L=0; CHA_L=PS2_chg;   
			DIR_R=0;SPD_R=0; CHA_R=PS2_chg; PointPacket(OutP);	
			return 0;
			}
		}	
		
		return 1;
}
















/************************************************************

	返回0,有控制指令来到，需要读出buf进行相应动作

************************************************************/
uchar RxFinish(uchar *Buf)
{
	if( Get_PS2Dat(PS2buf) )  //PS2遥控
	{
	IsPhone_STA=0;
	return IsPlayStation(PS2buf,Buf);//PS2遥控处理
	}
	
	/*
	
	--添加其他控制--
	
	*/
return 1;
}



