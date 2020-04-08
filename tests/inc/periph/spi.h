#ifndef __SPI_H
#define __SPI_H

#include <stdint.h>

typedef struct {

} SPI_HandleTypeDef;

enum {
	SPI1,
	SPI2,
	SPI3
};

void spi_init(int *instance);
int spi_transmit(int *instance, uint8_t *data, int size, int delay);
int spi_transmitreceive(int* instance, uint8_t *data_out, uint8_t *data_in, int size, int delay);
int spi_receive(int * instance, uint8_t *data, int size, int delay);
int spi_transmit_non_blocking(int *instance, uint8_t *data, uint8_t size);

#endif //__SPI_H
