/* 
 * date: 2.7.2017
 */

#ifndef __NRF24L0P_H
#define __NRF24L0P_H

#include "bits.h"

/*
 * Register Map
 * nrf24l01+ 
 */
/* checked */
#define CONFIG ((uint8_t) 0x00) /* configuration register */

#define MASK_RX_DR BIT8_6
#define MASK_TX_DS BIT8_5
#define MASK_MAX_RT BIT8_4
#define EN_CRC BIT8_3
#define CRCO BIT8_2
#define PWR_UP BIT8_1
#define PRIM_RX BIT8_0

/* checked */
#define EN_AA ((uint8_t) 0x01) 	/* enable auto ack */

#define ENAA_P5 BIT8_5
#define ENAA_P4 BIT8_4
#define ENAA_P3 BIT8_3
#define ENAA_P2 BIT8_2
#define ENAA_P1 BIT8_1
#define ENAA_P0 BIT8_0

/* checked */
#define EN_RXADDR ((uint8_t) 0x02) /* enable rx addr */

#define ERX_P5 BIT8_5
#define ERX_P4 BIT8_4
#define ERX_P3 BIT8_3
#define ERX_P2 BIT8_2
#define ERX_P1 BIT8_1
#define ERX_P0 BIT8_0

/* checked */
#define SETUP_AW ((uint8_t) 0x03) /* addr width (common for all data pipes) */

#define AW0 BIT8_0
#define AW1 BIT8_1

/* checked */
#define SETUP_RETR ((uint8_t) 0x04) /* automatic retransmission */

/* auto retransmit delay */
#define ARD_MASK ((uint8_t) 0x0f)
#define ARD_OFFSET ((uint8_t) 4)
/* auto retransmit count */
#define ARC_MASK ((uint8_t) 0x0f)

/* checked */
#define RF_CH ((uint8_t) 0x05) 	/* rf channel */

/* checked */
#define RF_SETUP ((uint8_t) 0x06) /* rf setup reg */

#define CONT_WAVE BIT8_7
#define RF_DR_LOW BIT8_5
#define PLL_LOCK BIT8_4
#define RF_DR_HIGH BIT8_3
#define RF_PWR1 BIT8_2
#define RF_PWR0 BIT8_1

/* checked */
#define STATUS ((uint8_t) 0x07)	/* status reg */

#define RX_DR BIT8_6
#define TX_DS BIT8_5
#define MAX_RT BIT8_4
#define RX_P_NO_MASK ((uint8_t) 0x0e)
#define RX_P_NO_OFFSET ((uint8_t) 1)
#define STATUS_TX_FULL BIT8_0

/* checked */
#define OBSERVE_TX ((uint8_t) 0x08) /* transmit observe reg */

#define PLOS_CNT_MASK ((uint8_t) 0xf0)
#define PLOS_CNT_OFFSET ((uint8_t) 4)
#define ARC_CNT_MASK ((uint8_t) 0x0f)

/* checked */
#define RPD ((uint8_t) 0x09)	/* received power detector */

#define RPD0 BIT8_0

/* checked */
#define RX_ADDR_P0 ((uint8_t) 0x0A) /* received addr data pipe */
#define RX_ADDR_P1 ((uint8_t) 0x0B)
#define RX_ADDR_P2 ((uint8_t) 0x0C)
#define RX_ADDR_P3 ((uint8_t) 0x0D)
#define RX_ADDR_P4 ((uint8_t) 0x0E)
#define RX_ADDR_P5 ((uint8_t) 0x0F)

/* checked */
#define TX_ADDR ((uint8_t) 0x10) /* transmit addr */

/* checked */
#define RX_PW_P0 ((uint8_t) 0x11)
#define RX_PW_P1 ((uint8_t) 0x12)
#define RX_PW_P2 ((uint8_t) 0x13)
#define RX_PW_P3 ((uint8_t) 0x14)
#define RX_PW_P4 ((uint8_t) 0x15)
#define RX_PW_P5 ((uint8_t) 0x16)

/* checked */
#define FIFO_STATUS ((uint8_t) 0x17) /* fifo status reg */

#define TX_REUSE BIT8_6
#define FIFO_TX_FULL BIT8_5
#define TX_EMPTY BIT8_4
#define RX_FULL BIT8_1
#define RX_EMPTY BIT8_0

/* checked */
#define DYNPD ((uint8_t) 0x1c)	     /* enable dynamic payload length */

#define DPL_P5 BIT8_5
#define DPL_P4 BIT8_4
#define DPL_P3 BIT8_3
#define DPL_P2 BIT8_2
#define DPL_P1 BIT8_1
#define DPL_P0 BIT8_0

#define FEATURE ((uint8_t) 0x1d) /* feature reg */

#define EN_DPL BIT8_2
#define EN_ACK_PAY BIT8_1
#define EN_DYN_ACK BIT8_0

/* 
 * Commands
 */

/* checked */
#define R_REGISTER_MASK ((uint8_t) 0x1f)
#define W_REGISTER_MASK ((uint8_t) 0x1f)
#define W_REGISTER_ORMASK ((uint8_t) 0x20) /* have to apply ormask before using */
#define R_RX_PAYLOAD ((uint8_t) 0x61)
#define W_TX_PAYLOAD ((uint8_t) 0xa0)
#define FLUSH_TX ((uint8_t) 0xe1)
#define FLUSH_RX ((uint8_t) 0xe2)
#define REUSE_TX_PL ((uint8_t) 0xe3)
#define R_RX_PL_WID ((uint8_t) 0x60)
#define W_ACK_PAYLOAD_MASK ((uint8_t) 0x07)
#define W_ACK_PAYLOAD_ORMASK ((uint8_t) 0xa8)
#define W_TX_PAYLOAD_NO_ACK ((uint8_t) 0xb0)
#define NOP ((uint8_t) 0xff)
	
#endif
