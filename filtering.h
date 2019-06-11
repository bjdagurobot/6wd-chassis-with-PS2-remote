#ifndef __FILTering_H_
#define __FILTering_H_

#include <iom168v.h>
#include <AVRdef.h>
#include  "mytype.h"
#include  "MotDriver.h"

#define Buf_Number 2    //���������� 2��



//extern uchar Mean_Num; //��ƽ�����˲��ĸ�����>=1ȡֵ


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



/*
���嶯̬������
GoalVal:     ��Ҫת����Ŀ��ֵ������ֵ��
FinVal��     ��̬�仯��ֵ

GapTime��    ת���ٶ�(speed)������ļ��ʱ�䣡
TallyTimeing ��λʱ����ۼӣ�

pixel��      ��ת1��ķֱ���
RunEN:       Ϊ1����GoalValing��=GoalVal��������˶������Խ���ͣ
RunSAT:      ����������״̬��0���˶���ɣ�1�������˶��У�

��������GapTime�ٶȱ仯��ÿ�α仯pixel����λ��ֱ��GoalValΪֹ��
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
extern struct RunParam RunParam_Reg; //����һ��ȫ�ֱ���
void   BufRuning(void);// ������£�Ҫ��ÿGapTime ���ж��У�����һ��

/*
�ƶ��������з�ʽ
SerNoͨ����SPD�ٶȣ�PIX�ķֱ��ʣ�ת����Deg����ֹͣ
����һ��PIX��ʱ��=SPD*��λʱ�䣨10ms��
һ��ת������PIX�����λ��
*/
void  BufSetRun(uchar BufNo,uchar Deg ,uchar SPD,uchar PIX);
/*
��һ��������Ҫʹ�ܣ�����Ҫ�����м��Goingֵ
����������ת��MIDΪ128��ʵ���Ͼ�������
��һ�ε���BufRuning��������ʼֵ
*/
void SetRunEn(uchar BufNo,uchar RunEn,uchar MID);


#endif






