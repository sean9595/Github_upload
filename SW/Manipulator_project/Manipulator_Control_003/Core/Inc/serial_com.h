/*
 *  serial_com.h
 *  Created on: 2019. Nov. 14
 *  Author: Sean K.
 *
 */

#ifndef SERIAL_COM_H_
#define SERIAL_COM_H_


#define UART_RX_BUFF_SIZE (256);

uint8_t uart_hal_getchar();

void uart_hal_rx_buffer_init();
void packet_Receive();
void serial_Init();
void uart_hal_rx_monitor();

#endif /* SERIAL_COM_H_ */
