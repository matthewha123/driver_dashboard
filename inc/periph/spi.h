#ifndef __SPI_H
#define __SPI_H

#include "common.h"
#include "gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SPI1_NSS_Pin GPIO_PIN_4
#define SPI1_NSS_GPIO_Port GPIOA

void spi_init(SPI_TypeDef *instance);

// Sets/Clears NSS pin
void select_nss(SPI_TypeDef *instance);
void deselect_nss(SPI_TypeDef *instance);

HAL_StatusTypeDef spi_transmit(SPI_TypeDef *instance, uint8_t *data, size_t size, int delay);
HAL_StatusTypeDef spi_transmit_non_blocking(SPI_TypeDef *instance, uint8_t *data, size_t size);

void HAL_SPI_TxCpltCallback (SPI_HandleTypeDef *hspi);  // function called by HAL after HAL_SPI_Transmit_IT is done

#ifdef __cplusplus
}
#endif

#endif //__SPI_H

