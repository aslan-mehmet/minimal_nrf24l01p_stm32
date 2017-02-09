/*
 * author: Mehmet ASLAN
 * date: February 7, 2017
 *
 * no warranty, no licence agreement
 * use it at your own risk
 */

#include "board.h"

void board_led_init(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

	GPIOC->MODER |= GPIO_MODER_MODER9_0 | GPIO_MODER_MODER8_0;
	GPIOC->OSPEEDR |= GPIO_OSPEEDR_OSPEEDR9 | GPIO_OSPEEDR_OSPEEDR8;
}
