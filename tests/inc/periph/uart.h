#ifndef __UART_H
#define __UART_H

#include <stdint.h>
#include "stm32f4xx_hal.h"

#define UART_DELAY 100

void uart_init(void);

#endif //__UART_H