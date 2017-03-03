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
   motordata[x][0]  ��е��
   motordata[x][1]  ʵ��ת�ص�������ֵ
   motordata[x][2]  ת�ظ�������ֵ
   motordata[x][3]  ��������ֵ
*/

#endif 
