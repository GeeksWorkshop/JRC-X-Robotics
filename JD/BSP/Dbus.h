#ifndef _Dbus_H
#define _Dbus_H


#include "main.h"
void Dbus_config(void);
void DMA2_Stream2_IRQHandler(void);

extern unsigned char sbus_rx_buffer[18];
extern int16_t sbus_decode_buffer[15];
extern int16_t subus_decode_keboard_buffer[8];  //keyboard data
extern int Right_State[2];
extern int  Left_State;
extern int16_t forwardback_order,leftright_order,rotation_order,updown_order;
extern int dbus_time;

#endif /*_Dbus_H*/