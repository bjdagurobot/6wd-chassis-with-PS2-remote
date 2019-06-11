#ifndef __FILTering_H_
#define __FILTering_H_

#include <iom168v.h>
#include <AVRdef.h>
#include  "mytype.h"
#include  "MotDriver.h"

#define Buf_Number 2    //缓冲器数量 2个



//extern uchar Mean_Num; //求平均数滤波的个数。>=1取值


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



/*
缓冲动态参数！
GoalVal:     将要转到的目标值（最终值）
FinVal：     动态变化的值

GapTime：    转动速度(speed)，两点的间隔时间！
TallyTimeing 单位时间的累加！

pixel：      运转1格的分辨率
RunEN:       为1，且GoalValing！=GoalVal，舵机将运动，可以紧急停
RunSAT:      缓冲器运行状态，0：运动完成，1：正在运动中！

缓冲器以GapTime速度变化，每次变化pixel个单位，直到GoalVal为止！
*/

struct RunParam
{
uchar GoalVal      [Buf_Number];
uchar GoalValing   [Buf_Number];
uchar GapTime      [Buf_Number];
uchar TallyTimeing [Buf_Number];
uchar pixel        [Buf_Number];
uchar RunSAT       [Buf_Number];
uchar RunEN        [Buf_Number];
};
extern struct RunParam RunParam_Reg; //定义一个全局变量
void   BufRuning(void);// 缓冲更新，要求每GapTime 秒中断中，调用一次

/*
制定缓冲运行方式
SerNo通道以SPD速度，PIX的分辨率，转动到Deg处后停止
更新一次PIX的时间=SPD*单位时间（10ms）
一次转动的是PIX个相对位置
*/
void  BufSetRun(uchar BufNo,uchar Deg ,uchar SPD,uchar PIX);
/*
第一次运行需要使能，且需要设置中间的Going值
比如电机正反转中MID为128，实际上就是设置
第一次调用BufRuning（）的起始值
*/
void SetRunEn(uchar BufNo,uchar RunEn,uchar MID);


#endif






