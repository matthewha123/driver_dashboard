#include "gpio.h"

void GPIO_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;
    HAL_GPIO_WritePin(GPIOA, BMS_FLT_Pin|PIN_OLED_CS|WHEEL_3_Pin|WHEEL_4_Pin 
                          |PIN_LED_DRIVER_LAT|WHEEL_1_Pin, GPIO_PIN_RESET);
    
    HAL_GPIO_WritePin(GPIOC, BUZZER_PIN|WHEEL_2_Pin 
                          |RTD_Pin|PIN_OLED_RS, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(GPIOB, IMD_FLT_Pin|TS_ACTIVE_Pin|WHEEL_6_Pin|WHEEL_5_Pin 
                      , GPIO_PIN_RESET);
    
    OLED_Pin_Init();

    GPIO_InitStruct.Pin = PIN_LED_DRIVER_GSCLK;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF0_MCO;
    HAL_GPIO_Init(PIN_LED_DRIVER_GSCLK_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = PIN_LED_DRIVER_LAT;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(PIN_LED_DRIVER_LAT_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LEFT_BUTTON_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LEFT_BUTTON_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = RIGHT_BUTTON_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(RIGHT_BUTTON_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = FAR_LEFT_BUTTON_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(FAR_LEFT_BUTTON_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = FAR_RIGHT_BUTTON_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(FAR_RIGHT_BUTTON_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = BMS_FLT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(BMS_FLT_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = IMD_FLT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(IMD_FLT_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = TS_ACTIVE_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(TS_ACTIVE_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = RTD_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(RTD_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = BUZZER_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(BUZZER_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = WHEEL_3_Pin|WHEEL_4_Pin|WHEEL_1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void Pin_Write(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState) {
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState);
}

bool Pin_Read(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
    return HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
}
void OLED_Pin_Init() {

    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = PIN_OLED_SCLK;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(OLED_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = PIN_OLED_SDI;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(OLED_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = PIN_OLED_CS;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(OLED_CS_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = PIN_OLED_RS;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(OLED_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = PIN_OLED_RS2;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(OLED_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = PIN_OLED_RW;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(OLED_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = PIN_OLED_E;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(OLED_GPIO_PORT, &GPIO_InitStruct);
}

void OLED_Data_Pins_Init() {
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = PIN_OLED_SCLK;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(OLED_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = PIN_OLED_SDI;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(OLED_GPIO_PORT, &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);

    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

