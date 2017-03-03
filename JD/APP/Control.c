#include "Control.h"

int chassis_data_order[4]={0};
int updownplatform_data_order=0;

void updownplatform_control(void)
{
	updownplatform_data_order=updown_order*20;
}
void chassiscontrol(void)
{  
	int  forwardbackflag=1;
  int  leftrightflag=1;
  int  rotationflag=1;	
  int  factorsum=0;
	
	const float forward_back_factor = 0.5f;      
	const float left_right_factor = 0.5f;        
	const float rotation_factor =1.0f;

//	chassis_data_order[0] = (int)(forwardback_order*forward_back_factor+ leftright_order *left_right_factor+ rotation_order*rotation_factor);     
//	chassis_data_order[1] = - (int)(forwardback_order*forward_back_factor-leftright_order*left_right_factor-rotation_order*rotation_factor);
//  chassis_data_order[2]=  -(int)(forwardback_order*forward_back_factor+leftright_order*left_right_factor-rotation_order*rotation_factor);
//	chassis_data_order[3]= (int)(forwardback_order*forward_back_factor-leftright_order *left_right_factor+rotation_order*rotation_factor);	

	chassis_data_order[0]= -(int)(forwardback_order*forward_back_factor+leftright_order*left_right_factor-rotation_order*rotation_factor);     
	chassis_data_order[1]=  (int)(forwardback_order*forward_back_factor-leftright_order *left_right_factor+rotation_order*rotation_factor);	
  chassis_data_order[2]= -(int)(forwardback_order*forward_back_factor-leftright_order*left_right_factor-rotation_order*rotation_factor);
	chassis_data_order[3]=  (int)(forwardback_order*forward_back_factor+ leftright_order *left_right_factor+ rotation_order*rotation_factor);   
	
	
	chassis_data_order[0]= chassis_data_order[0]*20.0f;      //14.8
	chassis_data_order[1]= chassis_data_order[1]*20.0f;
	chassis_data_order[2]= chassis_data_order[2]*20.0f;
	chassis_data_order[3]= chassis_data_order[3]*20.0f;
	
	if(chassis_data_order[0]>6600)
		chassis_data_order[0]=6600;
	else if(chassis_data_order[0]<-6600)
		chassis_data_order[0]=-6600;
	
		if(chassis_data_order[1]>6600)
		chassis_data_order[1]=6600;
	else if(chassis_data_order[1]<-6600)
		chassis_data_order[1]=-6600;
	
		if(chassis_data_order[2]>6600)
		chassis_data_order[2]=6600;
	else if(chassis_data_order[2]<-6600)
		chassis_data_order[2]=-6600;
	
		if(chassis_data_order[3]>6600)
		chassis_data_order[3]=6600;
	else if(chassis_data_order[3]<-6600)
		chassis_data_order[3]=-6600;
	
}

