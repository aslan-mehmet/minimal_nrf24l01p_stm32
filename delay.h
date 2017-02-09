/*
 * author: Mehmet ASLAN
 * date: February 7, 2017
 *
 * no warranty, no licence agreement
 * use it at your own risk
 */

#ifndef __DELAY_H
#define __DELAY_H

#include <stdint.h>

#define mSEC 1
#define uSEC 2

void delay_init(uint8_t unit);
void delay(uint16_t dur);

#endif
