#include "main.h"

int chassis_data[4];
int UpDownPlatform_data;

int main(void)
{
	BSP_Init();
	while(1)
	{
		chassis_Data (chassis_data[0],chassis_data[1],chassis_data[2],chassis_data[3]);
		UpDown_Platform (UpDownPlatform_data);
		delay_ms(2);
	}
}

void BSP_Init(void)
{
  delay_init(168);
	uart_init(115200);
	CAN1_Configuration();
	delay_ms(2);
  CAN2_Configuration();	
	delay_ms(2);
	Dbus_config();
	TIM4_Configuration();
	TIM7_Configuration(); 
}

