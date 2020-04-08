#ifndef __UART_H
#define __UART_H

#include "common.h"
#include "pins.h"
#include "gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct UART_Struct {
  UART_HandleTypeDef handle;
} UART_Struct;

#define UART_DELAY 100

void uart_init(void);
HAL_StatusTypeDef uart_transmit(char* data);
HAL_StatusTypeDef uart_transmit_ln(char* data);
HAL_StatusTypeDef uart_receive(uint8_t* data);
HAL_StatusTypeDef uart_transmit_num(uint32_t a, uint8_t base);

#ifdef __cplusplus
}
#endif
#endif //__UART_H

