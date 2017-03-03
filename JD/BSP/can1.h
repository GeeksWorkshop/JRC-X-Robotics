#ifndef __CAN1_H__
#define __CAN1_H__

#include <main.h>

void CAN1_Configuration(void);
void CAN1_TX_IRQHandler(void);
void CAN1_RX0_IRQHandler(void);
void GYRO_RST(void);
void UpDown_Platform (int16_t data);
extern int UpDownPlatform_Motor[2];
#endif 
