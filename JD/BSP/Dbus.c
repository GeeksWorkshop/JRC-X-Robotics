#include "Dbus.h"
unsigned char sbus_rx_buffer[18];
int16_t sbus_decode_buffer[15];
int16_t subus_decode_keboard_buffer[8];  //keyboard data
int  Right_State[2]={0};
int  Left_State=0;
int16_t forwardback_order,leftright_order,rotation_order,updown_order;
int dbus_time; // 记录完成dma的时间，用于放置接受不到遥控器数据
//Dbus初始化
void Dbus_config()
{
	USART_InitTypeDef usart1;
	GPIO_InitTypeDef  gpio;
	NVIC_InitTypeDef  nvic;
	DMA_InitTypeDef   dma;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_DMA2 ,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 ,ENABLE);
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7 ,GPIO_AF_USART1);
	
	gpio.GPIO_Pin = GPIO_Pin_7 ;
	gpio.GPIO_Mode = GPIO_Mode_AF;
  gpio.GPIO_OType = GPIO_OType_PP;
  gpio.GPIO_Speed = GPIO_Speed_100MHz;
  gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB,&gpio);
    
  USART_DeInit(USART1);
	usart1.USART_BaudRate = 100000;   //SBUS 100K baudrate
	usart1.USART_WordLength = USART_WordLength_8b;
	usart1.USART_StopBits = USART_StopBits_1;
	usart1.USART_Parity = USART_Parity_Even;
	usart1.USART_Mode = USART_Mode_Rx;
  usart1.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1,&usart1);
    
	USART_Cmd(USART1,ENABLE);
  USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
    
  nvic.NVIC_IRQChannel = DMA2_Stream2_IRQn;
  nvic.NVIC_IRQChannelPreemptionPriority = 0;
  nvic.NVIC_IRQChannelSubPriority = 1;
  nvic.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvic);
    
  DMA_DeInit(DMA2_Stream2);
  dma.DMA_Channel= DMA_Channel_4;
  dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);
  dma.DMA_Memory0BaseAddr = (uint32_t)sbus_rx_buffer;
  dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
  dma.DMA_BufferSize = 18;
  dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
  dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  dma.DMA_Mode = DMA_Mode_Circular;
  dma.DMA_Priority = DMA_Priority_VeryHigh;
  dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
  dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
  dma.DMA_MemoryBurst = DMA_Mode_Normal;
  dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream2,&dma);

  DMA_ITConfig(DMA2_Stream2,DMA_IT_TC,ENABLE);
  DMA_Cmd(DMA2_Stream2,ENABLE);
	
}
	
void DMA2_Stream2_IRQHandler(void)
{
	int dma2_i;
	if(DMA_GetITStatus(DMA2_Stream2, DMA_IT_TCIF2))
	{
		dbus_time=time4_now;
		
		DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2);
		DMA_ClearITPendingBit(DMA2_Stream2, DMA_IT_TCIF2);
		
		//右摇杆横向  范围+-660
		sbus_decode_buffer[0]= (sbus_rx_buffer[0]| (sbus_rx_buffer[1] << 8)) & 0x07ff; //!< Channel 0
		sbus_decode_buffer[0] -=1024;
		//右摇杆纵向   范围+-660
		sbus_decode_buffer[1] = ((sbus_rx_buffer[1] >> 3) | (sbus_rx_buffer[2] << 5)) & 0x07ff; //!< Channel 1
		sbus_decode_buffer[1] -=1024;
		//左摇杆横向   范围+-660
		sbus_decode_buffer[2]= ((sbus_rx_buffer[2] >> 6) | (sbus_rx_buffer[3] << 2) | (sbus_rx_buffer[4] << 10)) & 0x07ff; //!< Channel 2
		sbus_decode_buffer[2] -=1024;
		//左摇杆纵向   范围+-660
		sbus_decode_buffer[3] = ((sbus_rx_buffer[4] >> 1) | (sbus_rx_buffer[5] << 7)) & 0x07ff; //!< Channel 3
		sbus_decode_buffer[3] -=1024;
		//左边开关  123 上中下
		sbus_decode_buffer[4] = ((sbus_rx_buffer[5] >> 4)& 0x000C) >> 2; //!< Switch left
		//右边开关  123上中下
		sbus_decode_buffer[5] = ((sbus_rx_buffer[5] >> 4)& 0x0003); //!< Switch right9 / 9
		//鼠标x  范围+-32767
		sbus_decode_buffer[6] = sbus_rx_buffer[6] | (sbus_rx_buffer[7] << 8); //!< Mouse X axis
		//鼠标y  范围+-32767
		sbus_decode_buffer[7]= sbus_rx_buffer[8] | (sbus_rx_buffer[9] << 8); //!< Mouse Y axis
		//鼠标z  范围+-32767
		sbus_decode_buffer[8] = sbus_rx_buffer[10] | (sbus_rx_buffer[11] << 8); //!< Mouse Z axis
		//鼠标左键   按下1 没按0
		sbus_decode_buffer[9] = sbus_rx_buffer[12]; //!< Mouse Left Is Press ?
		//鼠标右键   按下1 没按0
		sbus_decode_buffer[10]= sbus_rx_buffer[13]; //!< Mouse Right Is Press ?
		//键盘
		sbus_decode_buffer[11]= sbus_rx_buffer[14] | (sbus_rx_buffer[15] << 8); //!< KeyBoard value
	
		// 判断是否丢数据
		if((abs(sbus_decode_buffer[0])>660)|(abs(sbus_decode_buffer[1])>660)|(abs(sbus_decode_buffer[2])>660)|(abs(sbus_decode_buffer[3])>660)|(abs(sbus_decode_buffer[4])>3)|(abs(sbus_decode_buffer[5])>3))
		{
		 for(dma2_i=0;dma2_i<18;dma2_i++)
			 {sbus_rx_buffer[dma2_i]=0x00;}
		 for(dma2_i=0;dma2_i<15;dma2_i++)
			 {sbus_decode_buffer[dma2_i]=0;} 
		 for(dma2_i=0;dma2_i<8;dma2_i++)
			 {subus_decode_keboard_buffer[dma2_i]=0;}  
				forwardback_order=0;
				leftright_order=0;
				rotation_order=0;
			  Dbus_config();
		}	
		else
		{ 
			switch (sbus_decode_buffer[4])
				{case 1:
					forwardback_order=sbus_decode_buffer[1]+(subus_decode_keboard_buffer[0]-subus_decode_keboard_buffer[1])*440;
					leftright_order=sbus_decode_buffer[0]+(subus_decode_keboard_buffer[3]-subus_decode_keboard_buffer[2])*440;
					rotation_order=sbus_decode_buffer[2]+sbus_decode_buffer[6]*30;
					updown_order=0;
					break;
				 case 2:
					forwardback_order=0;
					leftright_order=0;
					rotation_order=0;
					updown_order=sbus_decode_buffer[1];
					 break;
				 default:
					forwardback_order=0;
					leftright_order=0;
					rotation_order=0;
					updown_order=0;
					 break;
			  }
			}
		//死区为+-5
		if((forwardback_order<5)&&(forwardback_order>-5)) 
			forwardback_order=0;
		if((leftright_order<5)&&(leftright_order>-5))     
			leftright_order=0;
		if((rotation_order<5)&&(rotation_order>-5))       
			rotation_order=0;			
		if((updown_order<5)&&(updown_order>-5))       
			updown_order=0;			
	}
}



