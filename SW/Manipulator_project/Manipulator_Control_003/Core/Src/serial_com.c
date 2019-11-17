/*
 * serial_com.c
 *
 *  Created on: Nov. 14, 2019
 *      Author: Sean Kim
 *
 */

#include "usart.h"
#include "serial_com.h"

//UART_HandleTypeDef* uartHandle; //이렇게 선언을 하면 안된다.

uint16_t DLE = 0x10;
uint16_t STX = 0x02;
uint16_t ETX = 0x03;

/*Operation code set*/
uint16_t velo_set = 0x100;
uint16_t posi_set = 0x101;
uint16_t mov_dist_set = 0x102;
uint16_t ques_curr_velo = 0x200;
uint16_t ques_curr_posi = 0x201;



/* Ring buffer CODE BEGIN  */
typedef struct {
	uint8_t buffer_01 [UART_RX_BUFF_SIZE_01]; //수신 데이터 단순 저장용(1024개의 1byte짜리 데이터가 저장된다.)
	uint8_t buffer_02 [UART_RX_BUFF_SIZE_02]; //수신 데이터 중에서 유의미한 데이터만 저장하는 버퍼

	volatile uint8_t temp; //수신 이후 바로 이 변수에 저장됨.
	volatile uint8_t rxd;  //
    volatile uint8_t pre_scnd; //

	/*pointer 변수는 그 값이 유동적인 변수이므로 volatile로 선언하여 변수의 최적화를 하지 않도록 한다(실행시간감소목적).*/
	volatile uint16_t str_p_01; //현재 수신받은 데이터의 버퍼 내의 위치를 뜻함. store pointer
	volatile uint16_t scn_p_01; //현재 처리한 데이터의 버퍼 내의 위치를 나타냄. scan pointer 이 변수를 store_p 변수와 비교하여 새로 수신한 데이터가 있는지 체크.
	volatile uint16_t str_p_02;
	volatile uint16_t scn_p_02;
} uart_rx_type; //링버퍼 선언 (구조체형태)

/*링버퍼수신변수 선언 및 초기화*/
uart_rx_type uart_rx;

void uart_serial_rx_buffer_init()
{
	uart_rx.str_p_01 = 0;
	uart_rx.scn_p_01 = 0;
	uart_rx.str_p_02 = 0;
	uart_rx.scn_p_02 = 0;
}

/* Ring buffer CODE END */



void uart_serial_packet_Receive(UART_HandleTypeDef* uartHandle)
{
if(uartHandle->Instance == USART3)
    {
	uart_rx.buffer_01[uart_rx.str_p_01++] = uart_rx.temp;

	    if (uart_rx.str_p_01 >= 1024)
	    {
	    	uart_rx.str_p_01 = 0;
	    }
	    HAL_UART_Receive_IT(&huart3, &uart_rx.temp, 1);
    }
}

void uart_serial_Init()
{
	HAL_UART_Receive_IT(&huart3, &uart_rx.temp, 1);
}

/**/
uint8_t uart_serial_rx_check()
{
	if(uart_rx.str_p_01 == uart_rx.scn_p_01)
	{
		return 0; //모니터링을 하지 않게 한다.
	}

	uart_rx.buffer_01[uart_rx.scn_p_01-1] = uart_rx.pre_scnd; //이전의 데이터를 처리하기 위함

	uart_rx.rxd = uart_rx.buffer_01[uart_rx.scn_p_01++]; //처리 이후에 포인터의 값이 1 상승(postfix).

	if(uart_rx.scn_p_01 >= UART_RX_BUFF_SIZE_01)
	{
		uart_rx.scn_p_01 = 0;
	}

	return 1; //모니터링을 하게 한다.
}

void uart_serial_rx_monitor() //수신받은 데이터를 가공하는 작업을 해주는 함수
{
	while (uart_serial_rx_check() != 0)
	{
		if(uart_rx.rxd == DLE)
		{
          break;
		}

		if(uart_rx.rxd == STX)
		{
			if(uart_rx.pre_scnd == DLE)
			{
                //start of data
				break;
			}

			else if(uart_rx.pre_scnd != DLE)
			{
				//rxd에 담긴 데이터는 그냥 데이터
			}
		}

		if(uart_rx.rxd == ETX)
		{
			if(uart_rx.pre_scnd == DLE)
			{
			    //End of data
			}

			else if(uart_rx.pre_scnd != DLE)
			{
				//rxd에 담긴 데이터는 그냥 데이터
			}
		}


	}
}

void uart_serial_rx_process() //가공된 데이터를 처리하는 함수
{

}

