#include "timer4.h"
long int time4_now;
void TIM4_Configuration(void)    //1msÖĞ¶Ï
{
    TIM_TimeBaseInitTypeDef  tim;
    NVIC_InitTypeDef         nvic;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
    
    nvic.NVIC_IRQChannel = TIM4_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    tim.TIM_Prescaler =840-1;
    tim.TIM_CounterMode = TIM_CounterMode_Up;
    tim.TIM_ClockDivision = TIM_CKD_DIV1;
    tim.TIM_Period = 100-1;
    TIM_TimeBaseInit(TIM4,&tim);
	  TIM_Cmd(TIM4, ENABLE);	 
    TIM_ITConfig(TIM4, TIM_IT_Update,ENABLE);
}
void TIM4_IRQHandler(void)  
{
		
    if (TIM_GetITStatus(TIM4,TIM_IT_Update)!= RESET) 
		{   TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
        TIM_ClearFlag(TIM4, TIM_FLAG_Update);		  
			time4_now++;
			if((time4_now-dbus_time)>30)
			{
			chassis_data_order[0]=0;
			chassis_data_order[1]=0;
			chassis_data_order[2]=0;
			chassis_data_order[3]=0;
			}
			}
			
}