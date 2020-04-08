#ifndef __PINS_H_
#define __PINS_H_

#define PIN_OLED_SCLK GPIO_PIN_10
#define PIN_OLED_SDI GPIO_PIN_12
#define PIN_OLED_CS GPIO_PIN_2
#define PIN_OLED_RS GPIO_PIN_2
#define PIN_OLED_RW GPIO_PIN_1
#define PIN_OLED_E GPIO_PIN_0
#define PIN_OLED_RS2 GPIO_PIN_11

#define OLED_GPIO_PORT GPIOC
#define OLED_CS_PORT GPIOA

#define DEBUG_LED GPIO_PIN_0
#define DEBUG_LED_PORT GPIOB

#define DEBUG_LED_2 GPIO_PIN_1

#define PIN_LED_DRIVER_GSCLK        GPIO_PIN_8
		//Other good pins for gsclk - PC9 func MCO_2
                                    //PA8 func MCO_1
		//MCO1 pins can source from HSI, LSE, HSE, PLL
		//MCO2 pins can source from HSE, PLL, SYSCLK, PLLI2S
#define PIN_LED_DRIVER_GSCLK_PORT   GPIOA
#define PIN_LED_DRIVER_LAT_PORT     GPIOA
#define PIN_LED_DRIVER_LAT          GPIO_PIN_6

    /**SPI1 GPIO Configuration
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI
    */
#define PIN_LED_DRIVER_SCK        GPIO_PIN_5
#define PIN_LED_DRIVER_SCK_PORT   GPIOA
#define PIN_LED_DRIVER_SIN        GPIO_PIN_7
#define PIN_LED_DRIVER_SIN_PORT   GPIOA

#define LED_SPI SPI1

#define RIGHT_BUTTON_PIN GPIO_PIN_11
#define RIGHT_BUTTON_PORT GPIOA

#define FAR_RIGHT_BUTTON_PIN GPIO_PIN_10
#define FAR_RIGHT_BUTTON_PORT GPIOA

#define LEFT_BUTTON_PIN GPIO_PIN_3
#define LEFT_BUTTON_PORT GPIOC

#define FAR_LEFT_BUTTON_PIN GPIO_PIN_15
#define FAR_LEFT_BUTTON_PORT GPIOA

#define BUZZER_PIN GPIO_PIN_6
#define BUZZER_PORT GPIOC

#define BMS_FLT_Pin GPIO_PIN_1
#define BMS_FLT_GPIO_Port GPIOA

#define IMD_FLT_Pin GPIO_PIN_0
#define IMD_FLT_GPIO_Port GPIOB

#define TS_ACTIVE_Pin GPIO_PIN_1
#define TS_ACTIVE_GPIO_Port GPIOB

#define RTD_Pin GPIO_PIN_8
#define RTD_GPIO_Port GPIOC

#define WHEEL_1_Pin GPIO_PIN_12
#define WHEEL_1_GPIO_Port GPIOA
#define WHEEL_2_Pin GPIO_PIN_7
#define WHEEL_2_GPIO_Port GPIOC
#define WHEEL_3_Pin GPIO_PIN_3
#define WHEEL_3_GPIO_Port GPIOA
#define WHEEL_4_Pin GPIO_PIN_4
#define WHEEL_4_GPIO_Port GPIOA
#define WHEEL_5_Pin GPIO_PIN_15
#define WHEEL_5_GPIO_Port GPIOB
#define WHEEL_6_Pin GPIO_PIN_13
#define WHEEL_6_GPIO_Port GPIOB

#endif

