/*
 *  serial_com.h
 *  Created on: 2019. Nov. 14
 *  Author: Sean K.
 *
 */

#ifndef SERIAL_COM_H_
#define SERIAL_COM_H_


#define UART_RX_BUFF_SIZE_01 1024 //선언시에 ();를 적으면 이 또한 변수에 포함된다.
#define UART_RX_BUFF_SIZE_02 1024
uint8_t uart_serial_rx_check();

void uart_serial_rx_buffer_init();
void uart_serial_packet_Receive(UART_HandleTypeDef* uartHandle);
void uart_serial_Init();
void uart_serial_rx_monitor();
void uart_serial_rx_process();

#endif /* SERIAL_COM_H_ */
