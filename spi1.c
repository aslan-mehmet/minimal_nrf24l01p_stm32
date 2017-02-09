/*
 * author: Mehmet ASLAN
 * date: February 7, 2017
 *
 * no warranty, no licence agreement
 * use it at your own risk
 */

#include "stm32f0xx.h"

void spi1_init(void)
{
	/* clock */
	/* gpioa, spi1, apb2:48Mhz */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	/* gpio */
	/* pa4-nss
	 * pa5-sck
	 * pa6-miso
	 * pa7-mosi
	 */
	GPIO_InitTypeDef g;
	g.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	g.GPIO_Mode = GPIO_Mode_AF;
	g.GPIO_Speed = GPIO_Speed_50MHz;
	g.GPIO_OType = GPIO_OType_PP;
	g.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOA, &g);
	
	g.GPIO_Pin = GPIO_Pin_4;
	g.GPIO_Mode = GPIO_Mode_OUT;

	GPIO_Init(GPIOA, &g);
	
	/* alternate func 0 */
	GPIO_PinAFConfig(GPIOA, GPIO_Pin_5 |
			 GPIO_Pin_6 | GPIO_Pin_7, GPIO_AF_0);

	/* master, msbit first, byte, below 10 Mhz, first rising edge */
	SPI_InitTypeDef s;
	s.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	s.SPI_Mode = SPI_Mode_Master;
	s.SPI_DataSize = SPI_DataSize_8b;
	s.SPI_CPOL = SPI_CPOL_Low;
	s.SPI_CPHA = SPI_CPHA_1Edge;
	s.SPI_NSS = SPI_NSS_Soft;
	s.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	s.SPI_FirstBit = SPI_FirstBit_MSB;
	
	/* software control nss */
	/* why nss not csn */
	SPI_Init(SPI1, &s);

	/* rxne generated if byte exist */
	SPI_RxFIFOThresholdConfig(SPI1, SPI_RxFIFOThreshold_QF);
	
	/* enable */
	SPI_Cmd(SPI1, ENABLE);
}

uint8_t spi1_send(uint8_t data)
{
	/* send */
	SPI_SendData8(SPI1, data);

	/* wait transmission */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
		;
	
	/* return what you get */
	return SPI_ReceiveData8(SPI1);
}

void spi1_nss(uint8_t state)
{
	if (state)
		GPIOA->BSRR |= GPIO_BSRR_BS_4;
	else
		GPIOA->BSRR |= GPIO_BSRR_BR_4;
		
}
