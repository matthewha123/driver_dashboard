#ifndef __TLC5955__H_
#define __TLC5955__H_

#include "common.h"
#include "pins.h"
#include <assert.h> 
#include "spi.h"
#include "uart.h"
#include "gpio.h"
#include "micro_dwt_delay.h"
#include "can.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NUM_CHANNELS 48
#define NUM_DRIVERS 2
#define NUM_LEDS_PER_CHIP 16
#define NUM_COLOR_CHANNELS 3

#define CONTROL_WRITE_COUNT 2

#define NUM_BATTERY_LEVEL_LEDS 16

#define MAX_CURR_RED 0x1
#define MAX_CURR_BLUE 0x1
#define MAX_CURR_GREEN 0x1
#define BRIGHT_CORR_RED 0x1
#define BRIGHT_CORR_BLUE 0x1
#define BRIGHT_CORR_GREEN 0x1

#define DC_VAL 0x7F

extern int first_led_ordering[16]; //mapping indices of GS_Data to the ordering of led's left to right
extern int second_led_ordering[16];

//Channels for Maximum Current and Brightness Control
enum {
    RED, GREEN, BLUE
};

typedef enum {
    LEFT_BAR = 0,
    RIGHT_BAR = 1,
} LED_BAR;

//Bit fields for function control data latch
enum {
    DSPRPT, //Auto display repeat mode enable bit, 
            //1, "output repeats PWM control every 65,536 GSCLKS"
    TMGRST, //Displaly timing reset mode enable bit, 
            //0, GS counter not reset, outpts arent't forced off for GS data write
            //1, GS counter is reset to 0 and outputs are forced off with GS data write, then PWM control resumes
    RFRESH, //Auto data refresh mode enable bit
            //0, data in common shfit copied to GS data latch on next LAT rising edge. DC data in control data latch copied from Control data also
            //1, this copying happens after the 65,536 GSCLK
    ESPWM,  //ES PWM enable bit
            //0 for conventional pwm
            //1 for espwm
    LSDVLT  //LSD detection voltage selection
            //0, short detection threshold voltage is 70% of VCC
            //1, short detection threshold voltage is 90% of VCC
};

//good configuration:
    //DSPRT = 1; Want the pwm settings to repeat
    //TMGRST = 0; Turn off/reset GS counter with lat rising edge and gray scale write
    //RFRESH = 0; Want to control when writes to GSCLK happens
    //ESPWM =  1 or 0;
    //LSDVLT = Don't really care about led shorts

typedef struct {
    //adjusting brightness with pwm frequency values for each channel
    uint16_t GS_Data[NUM_DRIVERS][NUM_LEDS_PER_CHIP][NUM_COLOR_CHANNELS];

    //adjusting current between 26.2-100% for each channel. Precentage of current set by MC and BC;
    uint8_t DC_Data[NUM_DRIVERS][NUM_LEDS_PER_CHIP][NUM_COLOR_CHANNELS];

    //adjusting maximum current of the 3 different types of channels
    uint8_t MC_Data[3];

    //adjusting current between 10-100% of max value (defined by MC_data) of each channel
    uint8_t BC_Data[3];

    //5 bits long function control data latch
    bool FC_Data[5];
}TLC5955;

//Array that is filled with the shift register data to be sent, either GS or Control Data
uint16_t driver_data[NUM_CHANNELS * NUM_DRIVERS + 1];

// Setting and Sending Data
void Set_All_GS_Data(TLC5955* led_driver, uint16_t pwm_val);
void Set_GS_Data(TLC5955* led_driver, int driver, int led, int color, uint16_t pwm_val);

void Send_Control_Data(TLC5955* led_driver);
void Send_GS_Data(TLC5955* led_driver);

// Initialization
void Init_LED_Driver(TLC5955* led_driver);

// Setting Function Settings
void Set_DC_Data(TLC5955* led_driver, int driver, int led, int color, uint16_t dc_val);
void Set_MC_BC(TLC5955* led_driver, uint8_t MC_RED, uint8_t MC_BLUE, uint8_t MC_GREEN, uint8_t BC_RED, uint8_t BC_GREEN, uint8_t BC_BLUE);

void Set_Percentage(TLC5955* led_driver, float battery_percentage);
void Set_All_Color(TLC5955* led_driver, uint16_t pwm_val, uint8_t color);
void Set_All_Led_Driver(TLC5955* led_driver, int driver, uint16_t pwm_val);

uint16_t Get_GS_Data(TLC5955* led_driver, int driver, int led, int color);

void transmit_driver_data(TLC5955* led_driver, bool non_blocking);

// colorful functions

void pretty(TLC5955* led_driver);
void cycle_RGB(TLC5955* led_driver);
void handle_led_bars(TLC5955* led_driver);

#ifdef __cplusplus
}
#endif
#endif

