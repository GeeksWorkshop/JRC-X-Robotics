#include "chassispid.h"

GeneralPID chassispid[4]={ChassisSpeedPid_Default,ChassisSpeedPid_Default,ChassisSpeedPid_Default,ChassisSpeedPid_Default};
GeneralPID updownplatformpid=ChassisSpeedPid_Default;
void PidCalc(GeneralPID *pid)
{
	pid->Err=pid->SetPoint-pid->Input;
	pid->ErrSum +=pid->Err;
	// 积分限赋
  if(pid->ErrSum>pid->ErrSumMax)
		pid->ErrSum=pid->ErrSumMax;
	else if(pid->ErrSum<pid->ErrSumMin)
		pid->ErrSum=pid->ErrSumMin;
	pid->Output=pid->Kp*pid->Err+pid->Ki*pid->ErrSum+pid->Kd*(pid->Err-pid->ErrLast);
	// 输出限赋
  if(pid->Output>pid->OutputMax)
		pid->Output=pid->OutputMax;
	else if(pid->Output<pid->OutputMin)
		pid->Output=pid->OutputMin;
	pid->ErrLast=pid->Err;
}


//平滑滤波
int  Curve1(float x_0, float y_0, float x_1, float y_1, float ratio, float x)
{ 
	int n=3;
	float p[3],q[3],r[3],s[3];
	float xdata[3],ydata[3];
	float zz,h_0,h_1;
	int fg,i;
	float z;
	xdata[0]=x_0;
	xdata[1]=ratio*x_1+(1.0-ratio)*x_0;
	xdata[2]=x_1;
	
	ydata[0]=y_0;
	ydata[1]=(y_1+y_0)/2.0;
	ydata[2]=y_1;

	h_0=xdata[1]-xdata[0];
	h_1=xdata[2]-xdata[1];
	
	r[1]=3.0*(ydata[2]-ydata[1])/(h_1*(h_1+h_0))-3.0*(ydata[1]-ydata[0])/(h_0*(h_1+h_0));
	s[0]=r[1]/(2.0*h_0)-(ydata[1]-ydata[0])/(2.0*h_0*h_0*h_0);
	s[1]=-r[1]/(2.0*h_1)-(ydata[1]-ydata[0])/(2.0*h_1*h_1*h_1);
	r[0]=r[1]-3.0*s[0]*h_0;
	q[1]=-2.0*r[1]*h_1-3.0*s[1]*h_1*h_1;
	q[0]=0.0;
	p[0]=ydata[0];
	p[1]=ydata[1];

	fg=0;
	for( i=0; i<n-1; i++){
		if((x<xdata[i+1])&&(fg==0)){
			fg=1;
			zz=x-xdata[i];
			z=p[i]+zz*(q[i]+zz*(r[i]+zz*s[i]));
		}
	}

	if(x>=x_1) z=y_1;
	if(x<=x_0) z=y_0;
	
	return z;
}