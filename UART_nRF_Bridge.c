#include "UART_nRF_Bridge.h" 
#include "PS2.H"


uchar IsPhone_STA=0;

uchar DIR_L;	
uchar SPD_L;	
uchar CHA_L;	
uchar DIR_R;	
uchar SPD_R;	
uchar CHA_R;  //������ƵĲ���


void Bridge_Init(void)
{
uchar i;
SREG |=0x80;      //ȫ���ж�ʹ��
PS2_Init();
}





/*
ָ������ͷβ
*/
void PointPacket(uchar *nRFBuf)
{
uchar i;
ulong ld=0;
	
/*
nRF_buf[1] �����ַ��� 0/1
nRF_buf[2] �������ٶ� 0~100
nRF_buf[3] �������ٶȱ仯��1~10; 20ms�仯�ļ���
			
nRF_buf[4] �����ַ��� 0/1
nRF_buf[5] �������ٶ� 0~100	
nRF_buf[6] �������ٶȱ仯��1~10;20ms�仯�ļ���
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

		   PS2���߿��Ʒ�ʽ

**************************************/


// ����ֵ��Ӧ��
#define Stated_DirU   _PS2_Key_Dir_U
#define Stated_DirD   _PS2_Key_Dir_D
#define Stated_DirL   _PS2_Key_Dir_L
#define Stated_DirR   _PS2_Key_Dir_R //������ƶ�Ӧ�������������

#define Stated_SpdU   _PS2_Key_SideL_1
#define Stated_SpdD   _PS2_Key_SideL_2 //�ٶȿ��ƶ�Ӧ���12

#define Stated_ChgU    0
#define Stated_ChgD    0  				//�仯��δʹ�ã��޶�Ӧ����

#define Stated_Ser1_U  _PS2_Key_Butt_L
#define Stated_Ser1_D  _PS2_Key_Butt_R
#define Stated_Ser2_U  _PS2_Key_Butt_U
#define Stated_Ser2_D  _PS2_Key_Butt_D //������ƶ�Ӧ������������

//�������С����ֵ���Լ����ֵ��100��

#define MotOperateValue_Min 70
#define MotOperateValue_Max 90

//��������ʱ����Ӧ�ٶȵ�����ݼ����ٶ�
#define GradualSpeed_SPD 2

uchar PS2_Last_Down=0;
uchar PS2buf[10];
uchar PS2_SPD=80;
uchar PS2_chg=8; //�̶�

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
	
	/*------�������--------------*/
    if(KeyValue & Stated_Ser1_U)  {PWM_CR1++;     if(PWM_CR1>PWM_CH1_MAX) PWM_CR1=PWM_CH1_MAX; }
	if(KeyValue & Stated_Ser1_D)  {if(PWM_CR1>PWM_CH1_MIN)  PWM_CR1--;  }
	
	if(KeyValue & Stated_Ser2_U) {PWM_CR2++;     if(PWM_CR2>PWM_CH2_MAX) PWM_CR2=PWM_CH2_MAX; } 
	if(KeyValue & Stated_Ser2_D) { if(PWM_CR2>PWM_CH2_MIN)  PWM_CR2--;    }
	

		if(k==1)  //�Ѿ�����
		{

		LED1_setb;

			switch(Dir)
			{
			case 1: //��
					DIR_L=0;SPD_L=PS2_SPD; CHA_L=PS2_chg;
					DIR_R=0;SPD_R=PS2_SPD; CHA_R=PS2_chg; PointPacket(OutP);break;

			case 2:  //��
					DIR_L=1;SPD_L=PS2_SPD; CHA_L=PS2_chg;
					DIR_R=1;SPD_R=PS2_SPD; CHA_R=PS2_chg; PointPacket(OutP);break;

			case 4: //��
					DIR_L=1;SPD_L=PS2_SPD; CHA_L=PS2_chg;
					DIR_R=0;SPD_R=PS2_SPD; CHA_R=PS2_chg; PointPacket(OutP);break;

			case 8: //��
					DIR_L=0;SPD_L=PS2_SPD; CHA_L=PS2_chg;
					DIR_R=1;SPD_R=PS2_SPD; CHA_R=PS2_chg; PointPacket(OutP);break;

			case 5:  //����
					DIR_L=0;SPD_L=0; CHA_L=PS2_chg;
					DIR_R=0;SPD_R=PS2_SPD; CHA_R=PS2_chg; PointPacket(OutP);break;

			case 9:  //����
	 				DIR_L=0;SPD_L=PS2_SPD; CHA_L=PS2_chg;
					DIR_R=0;SPD_R=0; CHA_R=PS2_chg; PointPacket(OutP);break;

			case 6: //����
					DIR_L=1;SPD_L=PS2_SPD; CHA_L=PS2_chg;
					DIR_R=1;SPD_R=0; CHA_R=PS2_chg; PointPacket(OutP);break;

			case 10: //����
					DIR_L=1;SPD_L=0; CHA_L=PS2_chg;
					DIR_R=1;SPD_R=PS2_SPD; CHA_R=PS2_chg; PointPacket(OutP);break;

			default:
					DIR_L=0;SPD_L=0; CHA_L=PS2_chg;
					DIR_R=0;SPD_R=0; CHA_R=PS2_chg; PointPacket(OutP);break;
			}

		PS2_Last_Down=1;//�ϴΰ�����
		return 0;
		}
		else 
		{
		LED1_clr;
			if(PS2_Last_Down)  //�ϴΰ����ˣ��ſ�����
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

	����0,�п���ָ����������Ҫ����buf������Ӧ����

************************************************************/
uchar RxFinish(uchar *Buf)
{
	if( Get_PS2Dat(PS2buf) )  //PS2ң��
	{
	IsPhone_STA=0;
	return IsPlayStation(PS2buf,Buf);//PS2ң�ش���
	}
	
	/*
	
	--�����������--
	
	*/
return 1;
}



