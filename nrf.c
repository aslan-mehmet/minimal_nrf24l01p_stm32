/*
 * author: Mehmet ASLAN
 * date: February 7, 2017
 *
 * no warranty, no licence agreement
 * use it at your own risk
 */

#include "nrf.h"
#include "spi1.h"
#include "delay.h"
#include "stm32f0xx.h"

#define HIGH 1
#define LOW 0
#define nrf_set_csn(v) spi1_nss(v)
#define nrf_ce_high() GPIOA->BSRR = GPIO_BSRR_BS_3
#define nrf_ce_low() GPIOA->BSRR = GPIO_BSRR_BR_3
static void nrf_tx_init(void);
static void nrf_rx_init(void);

void nrf_read_reg(uint8_t reg_addr, uint8_t *buf, uint8_t len)
{
	/* every new cmd starts with csn high to low transmission */
	nrf_set_csn(LOW);
	/* spi R_REGISTER and register addr */
	spi1_send(R_REGISTER_MASK & reg_addr);
	
	/* push dummy data to read */
	for (uint8_t i = 0; i < len; ++i)
		buf[i] = spi1_send(0xff);
	
	/* csn high */
	nrf_set_csn(HIGH);
}

/* just for ease of use */
uint8_t nrf_read_reg_byte(uint8_t reg_addr)
{
	uint8_t data;
	nrf_read_reg(reg_addr, &data, 1);
	return data;
}

void nrf_write_reg(uint8_t reg_addr, uint8_t *buf, uint8_t len)
{
	nrf_set_csn(LOW);
	reg_addr &= W_REGISTER_MASK;
	spi1_send(reg_addr | W_REGISTER_ORMASK);

	for (uint8_t i = 0; i < len; ++i)
		spi1_send(buf[i]);

	nrf_set_csn(HIGH);
}

void nrf_write_reg_byte(uint8_t reg_addr, uint8_t data)
{
	nrf_write_reg(reg_addr, &data, 1);
}

void nrf_read_rx_payload(uint8_t *buf, uint8_t len)
{
	nrf_set_csn(LOW);
	spi1_send(R_RX_PAYLOAD);

	for (uint8_t i = 0; i < len; ++i)
		buf[i] = spi1_send(0xff);

	nrf_set_csn(HIGH);
}

void nrf_write_tx_payload(uint8_t *buf, uint8_t len)
{
	nrf_set_csn(LOW);
	spi1_send(W_TX_PAYLOAD);

	for (uint8_t i = 0; i < len; ++i)
		spi1_send(buf[i]);

	nrf_set_csn(HIGH);
}

/* to save space
 * use command name that has no data byte
 * specified in header file
 */
void nrf_cmd_no_data(uint8_t cmd_name)
{
	nrf_set_csn(LOW);
	spi1_send(cmd_name);
	nrf_set_csn(HIGH);
}

/* read rx fifo width */
uint8_t nrf_read_rx_payload_width(void)
{
	uint8_t r;
	nrf_set_csn(LOW);
	spi1_send(R_RX_PL_WID);
	r = spi1_send(0xff);
	nrf_set_csn(HIGH);
	return r;
}

void nrf_write_ack_payload(uint8_t pipe_number, uint8_t *buf, uint8_t len)
{
	pipe_number &= W_ACK_PAYLOAD_MASK;
	nrf_set_csn(LOW);
	spi1_send(W_ACK_PAYLOAD_ORMASK | pipe_number);

	for (uint8_t i = 0; i < len; ++i)
		spi1_send(buf[i]);

	nrf_set_csn(HIGH);
}

void nrf_write_tx_payload_no_ack(uint8_t *buf, uint8_t len)
{
	nrf_set_csn(LOW);
	spi1_send(W_TX_PAYLOAD_NO_ACK);

	for (uint8_t i = 0; i < len; ++i)
		spi1_send(buf[i]);

	nrf_set_csn(HIGH);
}

void nrf_init(uint8_t mode)
{
	/* common setting */
	/* spi1 init */
	spi1_init();
	nrf_set_csn(HIGH);
	/* delay init for ce timing requirements */
	delay_init(uSEC);
	/* ce hardware init, pa3 */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_InitTypeDef g;
	g.GPIO_Pin = GPIO_Pin_3;
	g.GPIO_Mode = GPIO_Mode_OUT;
	g.GPIO_Speed = GPIO_Speed_50MHz;
	g.GPIO_OType = GPIO_OType_PP;
	g.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &g);
	/* irq hardware init, pa0, active low so falling edge */
	g.GPIO_Pin = GPIO_Pin_0;
	g.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOA, &g);
	EXTI->IMR |= EXTI_IMR_MR0;
	EXTI->FTSR |= EXTI_FTSR_TR0;
	
	NVIC_InitTypeDef n;
	n.NVIC_IRQChannel = EXTI0_1_IRQn;
	n.NVIC_IRQChannelPriority = 1;
	n.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&n);

	nrf_write_reg_byte(STATUS, 0xff);
        /* clear any previous status bit
	 * so it doesnt mess up with my interrupts
	 */

#ifdef NRF_DEBUG_READ_WRITE_REGISTER
	/* checked
	 * careful if mcu works before you debug
	 * default value of specified register will be wrong
	 * because I have written them
	 */
	/* try to read TX_ADDR default:0xe7e7e7e7e7*/
	uint8_t arr[5];
	nrf_read_reg(TX_ADDR, arr, 5);

	for (uint8_t i = 0; i < 5; ++i)
		arr[i] = i;
	/* write to that addr */
	nrf_write_reg(TX_ADDR, arr, 5);
	/* read again if it is change */
	nrf_read_reg(TX_ADDR, arr, 5);
	/* also test byte versions config default:0x08 */
	arr[0] = nrf_read_reg_byte(CONFIG);
	nrf_write_reg_byte(CONFIG, 0x96);
	arr[0] = nrf_read_reg_byte(CONFIG);
#endif

#ifdef NRF_TEST_CE_IRQ_PIN
	delay_init(mSEC);
	while (1) {
		nrf_ce_high();
		delay(1000);
		nrf_ce_low();
		delay(1000);
	}
#endif
	
	/* choose prx ptx */
	if (mode == NRF_PTX)
		nrf_tx_init();
	else
		nrf_rx_init();
}

static void nrf_tx_init(void)
{
	nrf_ce_low();
	/* pwr up in tx mode all interrupts open*/
	nrf_write_reg_byte(CONFIG, PWR_UP | CRCO | EN_CRC);
	/* wait the slow chip */
	delay(10000);
	/* enable ack */
	nrf_write_reg_byte(EN_AA, ENAA_P0);
	/* enable rx pipe0 for receiving ack */
	nrf_write_reg_byte(EN_RXADDR, ERX_P0);
	/* setup addr width to 4 */
	nrf_write_reg_byte(SETUP_AW, AW1);
	/* retransmission thing */
	uint8_t tmp = 0x02; 	/* 750us delay interval between retransmission */
	tmp = tmp << ARD_OFFSET;
	tmp |= 0x05; 		/* retransmit 5 times */
	nrf_write_reg_byte(SETUP_RETR, tmp);
	/* set radio freq:1a */
	nrf_write_reg_byte(RF_CH, 0x1a);
	/* rf speed :1Mbps*/
	nrf_write_reg_byte(RF_SETUP, RF_PWR1);
	/* tx rx addr will be set in another func */
	
#ifdef NRF_DEBUG_PTX_NO_ACK
	/* tested verified awesome */
	/* disable ack, it dont intterupt when no ack received */
	nrf_write_reg_byte(EN_AA, 0x00);
	/* set tx addr so it doesnt give me crap */
	nrf_set_tx_addr(0x12345678);
        /* send without ack, see TX_DS interrupt */
	nrf_send32_no_ack(0x12345678);
	while (1)
		;
#endif
	
}

static void nrf_rx_init(void)
{
	/* ce high */
	nrf_ce_high();
	/* power up rx mode */
	nrf_write_reg_byte(CONFIG, PRIM_RX | PWR_UP | CRCO | EN_CRC);
	/* wait to power up */
	delay(10000);
	/* en ack pipe that runs shockburst */
	nrf_write_reg_byte(EN_AA, ENAA_P1);
	/* enable rx data pipe to receive */
	nrf_write_reg_byte(EN_RXADDR, ERX_P1);
	/* setup addr width */
	nrf_write_reg_byte(SETUP_AW, AW1); /* it may not be necessary */
	/* rf freq put same with transmitter */
	nrf_write_reg_byte(RF_CH, 0x1a);
	/* rf setup again same */
	nrf_write_reg_byte(RF_SETUP, RF_PWR1);
	/* set rx pipe addr */
	/* it will be set by user */
	/* payload width 4 */
	nrf_write_reg_byte(RX_PW_P1, 4);
}


void nrf_set_tx_addr(uint32_t tx_addr)
{
	/* set tx addr obviously */
	nrf_write_reg(TX_ADDR, ((uint8_t *) &tx_addr), 4);
	/* set rx pipe 0 to receive ack */
	nrf_write_reg(RX_ADDR_P0, ((uint8_t *) &tx_addr), 4);
}
void nrf_set_rx_addr(uint32_t rx_addr)
{
        /* dont change while you receiving data */
	nrf_write_reg(RX_ADDR_P1, ((uint8_t *) &rx_addr), 4);
}

void nrf_send32(uint32_t data)
{
        nrf_write_tx_payload(((uint8_t *) &data), 4);
        nrf_ce_high();
	delay(12);
	nrf_ce_low();
}

void nrf_send32_no_ack(uint32_t data)
{
	/* write payload without ack */
	nrf_write_tx_payload_no_ack(((uint8_t *) &data), 4);
	/* give ten usec ce pulse */
	nrf_ce_high();
	delay(12);
	nrf_ce_low();
}
uint32_t nrf_receive32(void)
{
	uint32_t r;
	nrf_read_rx_payload(((uint8_t *) &r), 4);
	nrf_write_reg_byte(STATUS, RX_DR);
	return r;
}
