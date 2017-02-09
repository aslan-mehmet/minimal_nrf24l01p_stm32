#ifndef __NRF_H
#define __NRF_H

#include "nrf24l01p.h"
#include <stdint.h>

/* commands */
void nrf_read_reg(uint8_t reg_addr, uint8_t *buf, uint8_t len);
uint8_t nrf_read_reg_byte(uint8_t reg_addr);
void nrf_write_reg(uint8_t reg_addr, uint8_t *buf, uint8_t len);
void nrf_write_reg_byte(uint8_t reg_addr, uint8_t data);
void nrf_read_rx_payload(uint8_t *buf, uint8_t len);
void nrf_write_tx_payload(uint8_t *buf, uint8_t len);
/* commands with no data bytes
 * FLUSH_TX
 * FLUSH_RX
 * REUSE_TX_PL
 * NOP
 */
void nrf_cmd_no_data(uint8_t cmd_name);
uint8_t nrf_read_rx_payload_width(void);
/* used in rx, write payload to be transmitted together ack */
void nrf_write_ack_payload(uint8_t pipe_number, uint8_t *buf, uint8_t len);
/* used in tx, disables ack on this specific packet */
void nrf_write_tx_payload_no_ack(uint8_t *buf, uint8_t len);

/* application functions */

#define NRF_PTX 1
#define NRF_PRX 2

void nrf_init(uint8_t mode);
void nrf_set_tx_addr(uint32_t tx_addr);
void nrf_set_rx_addr(uint32_t rx_addr);
void nrf_send32(uint32_t data);
void nrf_send32_no_ack(uint32_t data);
uint32_t nrf_receive32(void);

#endif
