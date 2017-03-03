#ifndef __CHASSISPID_H__
#define __CHASSISPID_H__

#include <main.h>

typedef struct GeneralPID_t
{
	float Kp;
	float Ki;
	float Kd;
	float SetPoint;
	float Input;  // Ref
	float Err;
	float ErrLast;
	float ErrSum;
	int   Output;
	float ErrSumMax;
	float ErrSumMin;
	int   OutputMax;
	int   OutputMin;
	void (*Calc)(struct GeneralPID_t *pid);//函数指针
}GeneralPID;

// 初始化轮子速度环PID
#define ChassisSpeedPid_Default \
{\
	4,\
	0,\
	0,\
	0,\
	0,\
	0,\
	0,\
	0,\
	0,\
	5000,\
	-5000,\
	10000,\
	-10000,\
	&PidCalc,\
}

void PidCalc(GeneralPID *pid);
float smoothingfilter(int input);

extern GeneralPID chassispid[4];
extern GeneralPID updownplatformpid;
#endif 
