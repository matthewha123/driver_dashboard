#include "uart.h"

char str[10];
UART_Struct uart1;

void uart_init(void) {

  uart1.handle.Instance = USART1;
  uart1.handle.Init.BaudRate = 115200;
  uart1.handle.Init.WordLength = UART_WORDLENGTH_8B;
  uart1.handle.Init.StopBits = UART_STOPBITS_1;
  uart1.handle.Init.Parity = UART_PARITY_NONE;
  uart1.handle.Init.Mode = UART_MODE_TX_RX;
  uart1.handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  uart1.handle.Init.OverSampling = UART_OVERSAMPLING_16;

  if (HAL_UART_Init(&(uart1.handle)) != HAL_OK) {
    _Error_Handler(__FILE__, __LINE__);
  }
}

HAL_StatusTypeDef uart_transmit(char* data) {
  return HAL_UART_Transmit(&(uart1.handle), (uint8_t *)data, strlen((const char*)data), UART_DELAY);
}

HAL_StatusTypeDef uart_transmit_ln(char* data) {
  if(uart_transmit(data) != HAL_OK) {
    Pin_Write(DEBUG_LED_PORT, DEBUG_LED, true);
    Pin_Write(DEBUG_LED_PORT, DEBUG_LED_2, true);
  }
  return uart_transmit("\r\n");
}

HAL_StatusTypeDef uart_transmit_num(uint32_t a, uint8_t base) {
  itoa(a,str, base);
  return uart_transmit(str);
}

HAL_StatusTypeDef uart_transmit_pf(uint8_t* data, int len) {
  return HAL_UART_Transmit(&(uart1.handle), data, len, UART_DELAY);
}

int _write(int file, char *data, int len) {
    UNUSED(file);
    HAL_StatusTypeDef status = uart_transmit_pf((uint8_t *) data, len);
    return status == HAL_OK ? len : 0;
}

