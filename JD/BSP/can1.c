#include "can1.h"
CanRxMsg can1_rx_message;
CanTxMsg can1_tx_message;
/*----CAN1_TX-----PA12----*/
/*----CAN1_RX-----PA11----*/
int UpDownPlatform_Motor[2];
/* 
  UpDownPlatform_Motor[0]  机械角
  UpDownPlatform_Motor[1]  实际转矩电流测量值
*/
void CAN1_Configuration(void)
{
    CAN_InitTypeDef        can;
    CAN_FilterInitTypeDef  can_filter;
    GPIO_InitTypeDef       gpio;
    NVIC_InitTypeDef       nvic;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1);

    gpio.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_11;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    GPIO_Init(GPIOA, &gpio);
    
    nvic.NVIC_IRQChannel = CAN1_RX0_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 3;
    nvic.NVIC_IRQChannelSubPriority = 2;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
    
    nvic.NVIC_IRQChannel = CAN1_TX_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 3;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);    
    
    CAN_DeInit(CAN1);
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
    CAN_Init(CAN1, &can);

	can_filter.CAN_FilterNumber=14;
	can_filter.CAN_FilterMode=CAN_FilterMode_IdMask;
	can_filter.CAN_FilterScale=CAN_FilterScale_32bit;
	can_filter.CAN_FilterIdHigh=0x0000;
	can_filter.CAN_FilterIdLow=0x0000;
	can_filter.CAN_FilterMaskIdHigh=0x0000;
	can_filter.CAN_FilterMaskIdLow=0x0000;
	can_filter.CAN_FilterFIFOAssignment=0;
	can_filter.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&can_filter);
    
    CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
    CAN_ITConfig(CAN1,CAN_IT_TME,ENABLE); 
		
		//user code
}

void CAN1_TX_IRQHandler(void)
{
    if (CAN_GetITStatus(CAN1,CAN_IT_TME)!= RESET) 
	{
	   CAN_ClearITPendingBit(CAN1,CAN_IT_TME);
    }
}

void GYRO_RST(void)
{
    CanTxMsg tx_message;
    
    tx_message.StdId = 0x404;
    tx_message.IDE = CAN_Id_Standard;
    tx_message.RTR = CAN_RTR_Data;
    tx_message.DLC = 0x08;
    
    tx_message.Data[0] = 0x00;
    tx_message.Data[1] = 0x01;
    tx_message.Data[2] = 0x02;
    tx_message.Data[3] = 0x03;
    tx_message.Data[4] = 0x04;
    tx_message.Data[5] = 0x05;
    tx_message.Data[6] = 0x06;
    tx_message.Data[7] = 0x07;
    
    CAN_Transmit(CAN1,&tx_message);
}
void CAN1_RX0_IRQHandler(void)
{  
    if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET)
	{
        CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
        CAN_Receive(CAN1, CAN_FIFO0, &can1_rx_message);
		if(can1_rx_message.StdId==0x201)
					 { UpDownPlatform_Motor[0] =(int16_t)((can1_rx_message.Data[0]<<8)|(can1_rx_message.Data[1]));
					   UpDownPlatform_Motor[1] =(int16_t)((can1_rx_message.Data[2]<<8)|(can1_rx_message.Data[3]));}
    }
	
}

void UpDown_Platform (int16_t data)
{
	  can1_tx_message.IDE = 0;    //标准帧
    can1_tx_message.RTR = 0;  //数据帧
    can1_tx_message.DLC = 8;          //帧长度为1
		can1_tx_message.StdId = 0x200;        //ID:00
	  can1_tx_message.Data[0]=  (u8)(data>>8);
	  can1_tx_message.Data[1]=  data;
		can1_tx_message.Data[2]=0;
		can1_tx_message.Data[3]=0;
	  can1_tx_message.Data[4]=0;
   	can1_tx_message.Data[5]=0;
	  can1_tx_message.Data[6]=0;
	  can1_tx_message.Data[7]=0;
	  CAN_Transmit(CAN1,&can1_tx_message);
}
