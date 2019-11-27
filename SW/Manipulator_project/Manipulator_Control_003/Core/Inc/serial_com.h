/*
 *  serial_com.h
 *
 *   Created on: Nov. 14, 2019
 *    Edited on: Nov. 17, 2019
 *       Author: Sean Kim
 *
 */

#ifndef SERIAL_COM_H_
#define SERIAL_COM_H_


#define UART_RX_BUFF_SIZE_01 1024 //선언시에 ();를 적으면 이 또한 변수에 포함된다.
#define UART_RX_BUFF_SIZE_02 1024
uint8_t uart_serial_rx_check_01();
uint8_t uart_serial_rx_check_02();

void uart_serial_rx_buffer_init();
void uart_serial_packet_Receive(UART_HandleTypeDef* uartHandle);
void uart_serial_Init();
void uart_serial_rx_monitor();
void uart_serial_rx_save();
void uart_serial_rx_process();

#endif /* SERIAL_COM_H_ */
