/*
 * author: Mehmet ASLAN
 * date: February 7, 2017
 *
 * no warranty, no licence agreement
 * use it at your own risk
 */

#ifndef __SPI1_H
#define __SPI1_H

#include <stdint.h>

void spi1_init(void);
uint8_t spi1_send(uint8_t data);
void spi1_nss(uint8_t state);

#endif
