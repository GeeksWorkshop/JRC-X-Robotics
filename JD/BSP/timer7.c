#include "timer7.h"

void TIM7_Configuration(void)    //2ms÷–∂œ
{
    TIM_TimeBaseInitTypeDef  tim;
    NVIC_InitTypeDef         nvic;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);
    
    nvic.NVIC_IRQChannel = TIM7_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    tim.TIM_Prescaler =840-1;
    tim.TIM_CounterMode = TIM_CounterMode_Up;
    tim.TIM_ClockDivision = TIM_CKD_DIV1;
    tim.TIM_Period = 200-1;
    TIM_TimeBaseInit(TIM7,&tim);
	  TIM_Cmd(TIM7, ENABLE);	 
    TIM_ITConfig(TIM7, TIM_IT_Update,ENABLE);
}

float input  =0 ;

void TIM7_IRQHandler(void)  
{
		
    if (TIM_GetITStatus(TIM7,TIM_IT_Update)!= RESET) 
		{
        TIM_ClearITPendingBit(TIM7,TIM_IT_Update);
        TIM_ClearFlag(TIM7, TIM_FLAG_Update);	
			
			  chassiscontrol();
			  updownplatform_control();
			
			  chassispid[0].Input=motordata[0][1];
			  chassispid[1].Input=motordata[1][1];
			  chassispid[2].Input=motordata[2][1];
			  chassispid[3].Input=motordata[3][1];
			  updownplatformpid.Input=UpDownPlatform_Motor[1];
			
			  chassispid[0].SetPoint=chassis_data_order[0];
			  chassispid[1].SetPoint=chassis_data_order[1];
			  chassispid[2].SetPoint=chassis_data_order[2];
			  chassispid[3].SetPoint=chassis_data_order[3];
			  updownplatformpid.SetPoint=updownplatform_data_order;
			
			  PidCalc(&chassispid[0]);
			  PidCalc(&chassispid[1]);
			  PidCalc(&chassispid[2]);
			  PidCalc(&chassispid[3]);
			  PidCalc(&updownplatformpid);
				
			  chassis_data[0]=chassispid[0].Output;
				chassis_data[1]=chassispid[1].Output;
				chassis_data[2]=chassispid[2].Output;
				chassis_data[3]=chassispid[3].Output;
				UpDownPlatform_data=updownplatformpid.Output;
			}
			
}
	

		