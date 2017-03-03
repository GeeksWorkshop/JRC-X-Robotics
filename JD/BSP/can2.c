#include "can2.h"
CanRxMsg can2_rx_message;
CanTxMsg can2_tx_message;
int motordata[4][2];
/* 
   motordata[x][0]  机械角
   motordata[x][1]  实际转矩电流测量值
   motordata[x][2]  转矩给定电流值
   motordata[x][3]  霍尔开关值
*/


/*----CAN2_TX-----PB13----*/
/*----CAN2_RX-----PB12----*/

void CAN2_Configuration(void)
{
    CAN_InitTypeDef        can;
    CAN_FilterInitTypeDef  can_filter;
    GPIO_InitTypeDef       gpio;
    NVIC_InitTypeDef       nvic;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_CAN2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_CAN2);

    gpio.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    GPIO_Init(GPIOB, &gpio);
    
    nvic.NVIC_IRQChannel = CAN2_RX0_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 2;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
    
    nvic.NVIC_IRQChannel = CAN2_TX_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 2;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);    
    
    CAN_DeInit(CAN2);
    CAN_StructInit(&can);
    
    can.CAN_TTCM = DISABLE;
    can.CAN_ABOM = DISABLE;
    can.CAN_AWUM = DISABLE;
    can.CAN_NART = DISABLE;
    can.CAN_RFLM = DISABLE;
    can.CAN_TXFP = ENABLE;
    can.CAN_Mode = CAN_Mode_Normal;
    can.CAN_SJW  = CAN_SJW_1tq;
    can.CAN_BS1 = CAN_BS1_9tq;
    can.CAN_BS2 = CAN_BS2_4tq;
    can.CAN_Prescaler = 3;   //CAN BaudRate 42/(1+9+4)/3=1Mbps
    CAN_Init(CAN2, &can);

	can_filter.CAN_FilterNumber=1;
	can_filter.CAN_FilterMode=CAN_FilterMode_IdMask;
	can_filter.CAN_FilterScale=CAN_FilterScale_32bit;
	can_filter.CAN_FilterIdHigh=0x0000;
	can_filter.CAN_FilterIdLow=0x0000;
	can_filter.CAN_FilterMaskIdHigh=0x0000;
	can_filter.CAN_FilterMaskIdLow=0x0000;
	can_filter.CAN_FilterFIFOAssignment=0;
	can_filter.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&can_filter);
    
    CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);
    CAN_ITConfig(CAN2,CAN_IT_TME,ENABLE); 
		
		//user code
}

void CAN2_TX_IRQHandler(void)
{
    if (CAN_GetITStatus(CAN2,CAN_IT_TME)!= RESET) 
	{
	   CAN_ClearITPendingBit(CAN2,CAN_IT_TME);
    }
}


void CAN2_RX0_IRQHandler(void)
{  
    if (CAN_GetITStatus(CAN2,CAN_IT_FMP0)!= RESET)
	{
        CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);
        CAN_Receive(CAN2, CAN_FIFO0, &can2_rx_message);
			     if(can2_rx_message.StdId==0x201)
					 { motordata[0][0] =(int16_t)((can2_rx_message.Data[0]<<8)|(can2_rx_message.Data[1]));
					   motordata[0][1] =(int16_t)((can2_rx_message.Data[2]<<8)|(can2_rx_message.Data[3]));}
					 			     else if(can2_rx_message.StdId==0x202)
										 {motordata[1][0] =(int16_t)((can2_rx_message.Data[0]<<8)|(can2_rx_message.Data[1]));
					            motordata[1][1] =(int16_t)((can2_rx_message.Data[2]<<8)|(can2_rx_message.Data[3]));}
										 		else	  if(can2_rx_message.StdId==0x203)
												{motordata[2][0] =(int16_t)((can2_rx_message.Data[0]<<8)|(can2_rx_message.Data[1]));
					               motordata[2][1] =(int16_t)((can2_rx_message.Data[2]<<8)|(can2_rx_message.Data[3]));}
												else 		 if(can2_rx_message.StdId==0x204)
												{motordata[3][0] =(int16_t)((can2_rx_message.Data[0]<<8)|(can2_rx_message.Data[1]));
					               motordata[3][1] =(int16_t)((can2_rx_message.Data[2]<<8)|(can2_rx_message.Data[3]));}
    }
	
}

void chassis_Data (int16_t data201,int16_t data202,int16_t data203,int16_t data204)
{
	  can2_tx_message.IDE = 0;    //标准帧
    can2_tx_message.RTR = 0;  //数据帧
    can2_tx_message.DLC = 8;          //帧长度为1
		can2_tx_message.StdId = 0x200;        //ID:00
	  can2_tx_message.Data[0]=  (u8)(data201>>8);
	  can2_tx_message.Data[1]=  data201;
		can2_tx_message.Data[2] = (u8)(data202>>8);
		can2_tx_message.Data[3] = data202;
	  can2_tx_message.Data[4]=(u8)(data203>>8);
   	can2_tx_message.Data[5]=data203;
	  can2_tx_message.Data[6]=(u8)(data204>>8);
	  can2_tx_message.Data[7]=data204;
	  CAN_Transmit(CAN2,&can2_tx_message);
}