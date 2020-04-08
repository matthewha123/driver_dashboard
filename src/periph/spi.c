#include "spi.h"
SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi3;

void spi_init(SPI_TypeDef *instance) {
  SPI_HandleTypeDef* handle;
  if(instance == SPI1) {
    handle = &hspi1;
    handle->Init.DataSize = SPI_DATASIZE_16BIT;
  }
  else if(instance == SPI3) {
    handle = &hspi3;
    handle->Init.DataSize = SPI_DATASIZE_8BIT;
  }
  else {
    handle = &hspi1;
    handle->Init.DataSize = SPI_DATASIZE_16BIT;
  }
  handle->Instance = instance;
  handle->Init.Mode = SPI_MODE_MASTER;
  handle->Init.Direction = SPI_DIRECTION_2LINES;
  handle->Init.CLKPolarity = SPI_POLARITY_LOW;
  handle->Init.CLKPhase = SPI_PHASE_1EDGE;
  handle->Init.NSS = SPI_NSS_SOFT;
  handle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  handle->Init.FirstBit = SPI_FIRSTBIT_MSB;
  handle->Init.TIMode = SPI_TIMODE_DISABLE;
  handle->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  handle->Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(handle) != HAL_OK) {
    Error_Handler();
  }
  deselect_nss(instance);
}

void select_nss(SPI_TypeDef *instance) {
  if(instance == SPI1) {
    HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);
  }
  else if(instance == SPI3) {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
  }
}

void deselect_nss(SPI_TypeDef *instance) {
  if(instance == SPI1) {
    HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);
  }
  else if(instance == SPI3) {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
  }
}

bool read_nss(SPI_TypeDef *instance) {
  if(instance == SPI1) {
    return Pin_Read(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin);
  }
  else {
    return false;
  }
}

HAL_StatusTypeDef spi_transmit(SPI_TypeDef *instance, uint8_t *data, size_t size, int delay) {
  SPI_HandleTypeDef* handle;
  if(instance == SPI1) {
    handle = &hspi1;
  }
  else if (instance == SPI3) {
    handle = &hspi3;
  } else {
    handle = &hspi1;
  }

  select_nss(handle->Instance);
  HAL_StatusTypeDef status = HAL_SPI_Transmit(handle, data, size, delay);
  deselect_nss(handle->Instance);
  return status;
}

HAL_StatusTypeDef spi_transmit_non_blocking(SPI_TypeDef *instance, uint8_t *data, size_t size) {

  if(read_nss(instance) == 1) {
    SPI_HandleTypeDef* handle;
    if(instance == SPI1) {
      handle = &hspi1;
    }

    select_nss(handle->Instance);
    HAL_StatusTypeDef status = HAL_SPI_Transmit_IT(handle, data, size);
    return status;
  }
  return HAL_BUSY;
}

void HAL_SPI_TxCpltCallback (SPI_HandleTypeDef *hspi) {

 SPI_TypeDef *instance = hspi->Instance;

 deselect_nss(instance);

}

