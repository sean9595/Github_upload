/*
 * serial_com.c
 *
 *  Created on: Nov. 14, 2019
 *      Author: Sean Kim
 *
 */

#include "usart.h"
#include "serial_com.h"

UART_HandleTypeDef* uartHandle;
/* Ring buffer CODE BEGIN  */

typedef struct {
	//uint8_t buffer [UART_RX_BUFF_SIZE]; //처리안되는 이유 체크
	uint8_t buffer[1024];
	uint8_t temp;
	uint8_t rxd;
	volatile uint16_t input_p; //현재 수신받은 데이터의 버퍼 내의 위치를 뜻함.
	volatile uint16_t output_p; //현재 처리한 데이터의 버퍼 내의 위치를 나타냄. 이 변수를 input_p 변수와 비교하여 새로 수신한 데이터가 있는지 체크.
} uart_hal_rx_type; //링버퍼 선언 (구조체형태)

/*링버퍼수신변수 선언 및 초기화*/
uart_hal_rx_type uart_hal_rx;

void uart_hal_rx_buffer_init()
{
	uart_hal_rx.input_p = 0;
	uart_hal_rx.output_p = 0;

}

/* Ring buffer CODE END */



void packet_Receive()
{
if(uartHandle->Instance == USART3)
    {
	    uart_hal_rx.buffer[uart_hal_rx.input_p ++] = uart_hal_rx.temp;

	    if (uart_hal_rx.input_p >= 1024)
	    {
	    	uart_hal_rx.input_p = 0;
	    }
	    HAL_UART_Receive_IT(&huart3, &uart_hal_rx.temp, 1);
    }
}


void serial_Init()
{
	HAL_UART_Receive_IT(&huart3, &uart_hal_rx.temp, 1);
}

void uart_hal_rx_monitor()
{
	while (uart_hal_getchar() != 0)
	{
		if(uart_hal_rx.rxd == 'sth')
		{
			//여기에 rxd값이 sth일때 처리하는 것을 작성.
		}
	}
}

uint8_t uart_hal_getchar()
{
	if(uart_hal_rx.input_p == uart_hal_rx.output_p)
	{
		return 0; //모니터링을 하지 않게 한다.
	}

	uart_hal_rx.rxd = uart_hal_rx.buffer[uart_hal_rx.output_p++];

	if(uart_hal_rx.output_p >= 1024)
	{
		uart_hal_rx.output_p = 0;
	}

	return 1; //모니터링을 하게 한다.
}

