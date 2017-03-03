#ifndef __MAIN_H
#define __MAIN_H 			

#define FALSE 0
#define TRUE 1

#define abs(x) ((x)>0? (x):(-(x)))

#include "stm32f4xx.h"
#include <sys.h>	  
#include "usart.h"
#include "delay.h"
#include "can1.h"
#include "can2.h" 
#include "Dbus.h"
#include "chassispid.h"
#include "control.h"
#include "timer7.h"
#include "timer4.h"
#include "timer3.h"

void BSP_Init(void);

extern int chassis_data[4];
extern int UpDownPlatform_data;

#endif