#ifndef __TIMER4_H
#define	__TIMER4_H

#include "main.h"
void TIM4_Configuration(void)  ;
void TIM4_IRQHandler(void);
extern long int time4_now;
#endif /* __TIMER4_H */