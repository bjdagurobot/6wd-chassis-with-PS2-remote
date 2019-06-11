#include  "filtering.h"


/*
���������ܣ����ƴ�״A ��״̬B �仯����
���Ʋ����У� ����ͨ���ţ����Ƽ�϶ʱ�䣬���Ƶ�λ�߶�
���÷�����
1����ʵ�ʵĿ���Ŀ����void VaryFun(uchar BufNo,uchar Val)�ҹ�
2��BufSetRun(uchar BufNo,uchar Deg ,uchar SPD,uchar PIX)��
3��Ҫ��ÿGapTime ���ж��У�����һ��void BufRuning(void)
4������RunSAT��֪����ͨ��������ɵ�״̬
5���´���BufSetRun(uchar BufNo,uchar Deg ,uchar SPD,uchar PIX)��
*/

void VaryFun(uchar BufNo,uchar Val)  //ʵ�ʶ��/�������ת�������ҹ�������
{

	switch(BufNo)
	{
	case 0: motorL_Cont(((int)Val-(int)128)*2);  break;
	case 1: motorR_Cont(((int)Val-(int)128)*2);  break;
	//case 2: Servo_Cont(1,Val);  break;
	//case 3: Servo_Cont(2,Val);  break;
	}
}

//����һ�Σ�0 :�Ѿ�ת���ˣ�1��δ��
uchar BufferRun(uchar BufNo)
{
if( RunParam_Reg.GoalValing [BufNo]  >  RunParam_Reg.GoalVal[BufNo] )
{
	if( RunParam_Reg.GoalValing [BufNo]-RunParam_Reg.pixel[BufNo]  <=  RunParam_Reg.GoalVal[BufNo])
	{RunParam_Reg.GoalValing [BufNo]  =  RunParam_Reg.GoalVal[BufNo];  return 0;}
	else
	{RunParam_Reg.GoalValing [BufNo] -= RunParam_Reg.pixel[BufNo];return 1;}

}
else
{
	if(RunParam_Reg.GoalValing [BufNo]+RunParam_Reg.pixel[BufNo]  >=  RunParam_Reg.GoalVal[BufNo])
	{RunParam_Reg.GoalValing [BufNo]  =  RunParam_Reg.GoalVal[BufNo];  return 0;}
	else
	 {RunParam_Reg.GoalValing [BufNo] +=  RunParam_Reg.pixel[BufNo]; return 1;}
}
}


/*
һ�º���ÿ��λʱ�����һ�Σ���10ms��
����

����˶����� ��̬������
GoalVal:     ��Ҫת����Ŀ��ֵ������ֵ��
FinVal��     ��̬�仯��ֵ

GapTime��    ת���ٶ�(speed)������ļ��ʱ�䣡
TallyTimeing ��λʱ����ۼӣ�
pixel��      ��ת1��ķֱ���
RunSAT:      ״ָ̬ʾ��Ϊ1����GoalValing��=GoalVal��������˶������Խ���ͣ

��������GapTime�ٶȱ仯��ÿ�α仯pixel����λ��ֱ��GoalValΪֹ��

struct RunParam
{
uchar GoalVal      [Buf_Number];
uchar GoalValing   [Buf_Number];
uchar GapTime      [Buf_Number];
uchar TallyTimeing [Buf_Number];
uchar pixel        [Buf_Number];
uchar RunSAT       [Buf_Number];
};
struct SerRunParam RunParam_Reg; //����һ��ȫ�ֱ���
*/

void BufRuning(void)
{
uchar i,k;

for(i=0;i<Buf_Number;i++)  //���Buf_Number��
{
	if(RunParam_Reg.RunEN[i]!=0)  //�����Ƿ�ʹ��
	{
		if(RunParam_Reg.RunSAT[i]!=0)  //�������У�
		{
			if((++RunParam_Reg.TallyTimeing[i]) == RunParam_Reg.GapTime[i] )  //����ʱ�䵽�ˣ� =GapTime*��Ԫʱ�䣨0.01S��
			{
			RunParam_Reg.TallyTimeing[i]=0;
			k=BufferRun(i);            //�ı�SerRunParam_Reg.SerDegree[i]
			RunParam_Reg.RunSAT[i]=k;  ////���е�λ�ú�ر�ʹ�ܣ�Ҳ�ɲ���
			VaryFun(i,RunParam_Reg.GoalValing [i]);  //ʵ�ʶ������ת�������ҹ�������
			}
		}
	}
}
}

/*
�ƶ��������з�ʽ
SerNoͨ����SPD�ٶȣ�PIX�ķֱ��ʣ�ת����Deg����ֹͣ
����һ��PIX��ʱ��=SPD*��λʱ�䣨10ms��
һ��ת������PIX�����λ��
*/
void BufSetRun(uchar BufNo,uchar Deg ,uchar SPD,uchar PIX)
{
RunParam_Reg.GoalVal      [BufNo]=Deg;
RunParam_Reg.GapTime      [BufNo]=SPD;
RunParam_Reg.TallyTimeing [BufNo]=0;
RunParam_Reg.pixel        [BufNo]=PIX;
RunParam_Reg.RunSAT       [BufNo]=1;//���������������������Զ���0
}


void SetRunEn(uchar BufNo,uchar RunEn,uchar MID)
{
RunParam_Reg.GoalValing [BufNo]=MID;
RunParam_Reg.RunEN [BufNo]=RunEn;
}

