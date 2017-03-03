#include "timer3.h"
void TIM3_Configuration(void)    //20hz÷–∂œ
{
    TIM_TimeBaseInitTypeDef  tim;
    NVIC_InitTypeDef         nvic;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    
    nvic.NVIC_IRQChannel = TIM3_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    tim.TIM_Prescaler =8400;
    tim.TIM_CounterMode = TIM_CounterMode_Up;
    tim.TIM_ClockDivision = TIM_CKD_DIV1;
    tim.TIM_Period = 500-1;
    TIM_TimeBaseInit(TIM3,&tim);
	  TIM_Cmd(TIM3, ENABLE);	 
    TIM_ITConfig(TIM3, TIM_IT_Update,ENABLE);
}
void TIM3_IRQHandler(void)  
{
		
    if (TIM_GetITStatus(TIM3,TIM_IT_Update)!= RESET) 
		{   
			TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
      TIM_ClearFlag(TIM3, TIM_FLAG_Update);		 
			}
			
}