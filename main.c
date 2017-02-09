/*
 * author: Mehmet ASLAN
 * date: February 7, 2017
 *
 * no warranty, no licence agreement
 * use it at your own risk
 */

#include "stm32f0xx.h"
#include "board.h"
#include "nrf.h"
#include "delay.h"

void main()
{
        board_led_init();

        /* define these macro directly to compiler */
#ifdef DEBUG_FIRST_TRANSMIT
	nrf_init(NRF_PTX);
	nrf_set_tx_addr(0x12345678);
	nrf_send32(0x87654321);
#endif

#ifdef DEBUG_FIRST_RECEIVE
	nrf_init(NRF_PRX);
	nrf_set_rx_addr(0x12345678);
#endif
	while (1)
		;
}
