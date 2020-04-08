#include "uart.h"

char str[10];

void uart_init(void) {

  printf("uart_init called\r\n");
}

int uart_transmit(uint8_t* data) {
  // printf("uart_transmit called");
  return 1;
}

int uart_transmit_ln(uint8_t* data) {
	// printf("uart_transmit_ln called");
	return 1;
}

int uart_transmit_num(uint32_t a, uint8_t base) {
	// printf("%d", a);
	return 1;
}