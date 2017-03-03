#ifndef __LED_H__
#define __LED_H__

#include <main.h>

#define LED1_ON()       GPIO_ResetBits(GPIOC,GPIO_Pin_1)
#define LED1_OFF()      GPIO_SetBits(GPIOC,GPIO_Pin_1)
#define LED1_TOGGLE()   (GPIOB->ODR) ^= GPIO_Pin_1//Blue

#define LED2_ON()       GPIO_ResetBits(GPIOC,GPIO_Pin_2)
#define LED2_OFF()      GPIO_SetBits(GPIOC,GPIO_Pin_2)
#define LED2_TOGGLE()   (GPIOB->ODR) ^= GPIO_Pin_2//Red


void LED_Configuration(void);


#endif 
