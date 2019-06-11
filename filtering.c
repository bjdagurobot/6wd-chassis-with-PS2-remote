#include  "filtering.h"


/*
缓冲器功能：控制从状A 到状态B 变化过程
控制参数有： 控制通道号，控制间隙时间，控制单位尺度
调用方法：
1：将实际的控制目标与void VaryFun(uchar BufNo,uchar Val)挂钩
2：BufSetRun(uchar BufNo,uchar Deg ,uchar SPD,uchar PIX)；
3：要求每GapTime 秒中断中，调用一次void BufRuning(void)
4：根据RunSAT得知各个通道缓冲完成的状态
5：下次再BufSetRun(uchar BufNo,uchar Deg ,uchar SPD,uchar PIX)；
*/

void VaryFun(uchar BufNo,uchar Val)  //实际舵机/电机的运转函数！挂钩函数！
{

	switch(BufNo)
	{
	case 0: motorL_Cont(((int)Val-(int)128)*2);  break;
	case 1: motorR_Cont(((int)Val-(int)128)*2);  break;
	//case 2: Servo_Cont(1,Val);  break;
	//case 3: Servo_Cont(2,Val);  break;
	}
}

//运行一次，0 :已经转到了，1：未到
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
一下函数每单位时间调用一次！（10ms）
根据

舵机运动特性 动态参数！
GoalVal:     将要转到的目标值（最终值）
FinVal：     动态变化的值

GapTime：    转动速度(speed)，两点的间隔时间！
TallyTimeing 单位时间的累加！
pixel：      运转1格的分辨率
RunSAT:      状态指示，为1，且GoalValing！=GoalVal，舵机将运动，可以紧急停

缓冲器以GapTime速度变化，每次变化pixel个单位，直到GoalVal为止！

struct RunParam
{
uchar GoalVal      [Buf_Number];
uchar GoalValing   [Buf_Number];
uchar GapTime      [Buf_Number];
uchar TallyTimeing [Buf_Number];
uchar pixel        [Buf_Number];
uchar RunSAT       [Buf_Number];
};
struct SerRunParam RunParam_Reg; //定义一个全局变量
*/

void BufRuning(void)
{
uchar i,k;

for(i=0;i<Buf_Number;i++)  //检测Buf_Number次
{
	if(RunParam_Reg.RunEN[i]!=0)  //运行是否使能
	{
		if(RunParam_Reg.RunSAT[i]!=0)  //继续运行？
		{
			if((++RunParam_Reg.TallyTimeing[i]) == RunParam_Reg.GapTime[i] )  //更新时间到了？ =GapTime*单元时间（0.01S）
			{
			RunParam_Reg.TallyTimeing[i]=0;
			k=BufferRun(i);            //改变SerRunParam_Reg.SerDegree[i]
			RunParam_Reg.RunSAT[i]=k;  ////运行到位置后关闭使能！也可不用
			VaryFun(i,RunParam_Reg.GoalValing [i]);  //实际舵机的运转函数！挂钩函数！
			}
		}
	}
}
}

/*
制定缓冲运行方式
SerNo通道以SPD速度，PIX的分辨率，转动到Deg处后停止
更新一次PIX的时间=SPD*单位时间（10ms）
一次转动的是PIX个相对位置
*/
void BufSetRun(uchar BufNo,uchar Deg ,uchar SPD,uchar PIX)
{
RunParam_Reg.GoalVal      [BufNo]=Deg;
RunParam_Reg.GapTime      [BufNo]=SPD;
RunParam_Reg.TallyTimeing [BufNo]=0;
RunParam_Reg.pixel        [BufNo]=PIX;
RunParam_Reg.RunSAT       [BufNo]=1;//开启舵机控制允许，到点后自动清0
}


void SetRunEn(uchar BufNo,uchar RunEn,uchar MID)
{
RunParam_Reg.GoalValing [BufNo]=MID;
RunParam_Reg.RunEN [BufNo]=RunEn;
}

