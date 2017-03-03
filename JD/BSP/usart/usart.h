#ifndef __USART_H
#define __USART_H

#define READ_BUFFER_SIZE      28

#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 

	
void uart_init(u32 bound);
void USART3_IRQHandler(void);
//void DMA1_Stream1_IRQHandler(void);

#endif


