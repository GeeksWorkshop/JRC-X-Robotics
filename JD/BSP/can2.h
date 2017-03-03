#ifndef __CAN2_H__
#define __CAN2_H__

#include <main.h>
void CAN2_Configuration(void);
void CAN2_TX_IRQHandler(void);
void CAN2_RX0_IRQHandler(void);
void chassis_Data (int16_t data201,int16_t data202,int16_t data203,int16_t data204);
extern CanRxMsg can2_rx_message;
extern CanTxMsg can2_tx_message;
extern int motordata[4][2];
/* 
   motordata[x][0]  机械角
   motordata[x][1]  实际转矩电流测量值
   motordata[x][2]  转矩给定电流值
   motordata[x][3]  霍尔开关值
*/

#endif 
