#include "TLC5955.h"
#include "can.h"

static int current_bit_position = 0;

int left_led_ordering[16] = {7,3,6,2,1,5,0,4,11,15,10,14,13,9,12,8}; //mapping indices of GS_Data to the ordering of led's left to right
int right_led_ordering[16] = {4,0,5,1,2,6,3,7,8,12,9,13,14,10,15,11};

// colorful function global vars
int color = RED;
int zero_color_1 = BLUE;
int zero_color_2 = GREEN;

uint8_t pretty_color_offset = 0;
uint16_t pretty_colors[16][3] =
{
    {65535, 6425, 6425},
    {65535, 10794, 0},
    {65535, 26214, 6425},
    {65535, 46003, 6425},
    {65535, 65535, 6425},
    {26214, 65535, 6425},
    {6425, 65535, 16448},
    {6425, 65535, 35980},
    {6425, 65535, 55769},
    {6425, 55769, 65535},
    {6425, 35980, 65535},
    {6425, 16448, 65535},
    {16448, 6425, 65535},
    {35980, 6425, 65535},
    {55769, 6425, 65535},
    {65535, 6425, 65535}
};

const uint32_t MAX_PACK_VOLTAGE = 330000; // mV
const uint32_t MIN_PACK_VOLTAGE = 200000;
const uint16_t HANDLE_LED_BARS_PERIOD = 1000; // ms
const uint32_t MAX_POWER = 80000;
const uint16_t MIN_CELL_TEMP = 2000;
const uint16_t MAX_CELL_TEMP = 6000;

//Helper functions to place bits for sending in 16 byte chunks over Spi

/*
    Given array value and bit offset, clears the corresponding bit
*/
static void Clear_Bit(unsigned short* val, int offset) {
    *val = *val & ~(1<<offset); // Clear the bit in val at position offset
}

/*
    Given array value and bit offset, sets the corresponding bit
*/
static void Set_Bit(unsigned short* val, int offset) {
    *val = *val | (1<<offset); //Set the bit in val at position offset
}

/*
    Given a bit, clears/sets a bit in the 16 bit data array sent to the LED driers
    Remembers the position of the bit last placed
*/
static void Pack_Bit(uint8_t bit) {

    int short_position = current_bit_position / 16;
    int offset = current_bit_position % 16;

    if(bit == 0) {

        Clear_Bit(&(driver_data[short_position]), 15 - offset);

    } else {

        Set_Bit(&(driver_data[short_position]), 15 - offset);
    }

    current_bit_position++;
}

/*
    Given a byte, calls Pack_bit to pack byte's contents, MSB first
*/
static void Pack_Byte(uint8_t byte) {
    int i;

    for(i = 7; i >= 0; i--) {

        Pack_Bit(byte & (1<<i));
    }
}

/*
    Given a 16 bit value, packs contents MSB first
*/
static void Pack_Short(uint16_t sh) {
    Pack_Byte(sh >> 8);
    Pack_Byte(sh & (0xFF));
}


/*
    Function to toggle latch with pulse width of 100 us
*/
static void Toggle_Lat(void) {
    //Toggle LAT
    Pin_Write(PIN_LED_DRIVER_LAT_PORT, PIN_LED_DRIVER_LAT, 0);
    DWT_Delay_us(100);
    Pin_Write(PIN_LED_DRIVER_LAT_PORT, PIN_LED_DRIVER_LAT, 1);
    DWT_Delay_us(100);
    Pin_Write(PIN_LED_DRIVER_LAT_PORT, PIN_LED_DRIVER_LAT, 0);
}

/*
    Functions to pack the corresponding control data bits
*/
static void Pack_FC(TLC5955* led_driver) {
    Pack_Bit(led_driver->FC_Data[LSDVLT]);
    Pack_Bit(led_driver->FC_Data[ESPWM]);
    Pack_Bit(led_driver->FC_Data[RFRESH]);
    Pack_Bit(led_driver->FC_Data[TMGRST]);
    Pack_Bit(led_driver->FC_Data[DSPRPT]);
}

static void Pack_BC(TLC5955* led_driver) {

    for(int color = RED; color < BLUE + 1; color++) {

        for(int k = 6; k >= 0; k--) {

            Pack_Bit(led_driver->BC_Data[color] & (1 << k));
        }
    }
}

static void Pack_MC(TLC5955* led_driver) {

    for(int color = RED; color < BLUE + 1; color++) {

        for(int k = 2; k >= 0; k--) {

            Pack_Bit(led_driver->MC_Data[color] & (1 << k));
        }
    }
}

static void Pack_DC(TLC5955* led_driver, int driver) {
    for(int led = 0; led < NUM_LEDS_PER_CHIP; led++) {

        for(int color = 0; color < NUM_COLOR_CHANNELS; color++) {

            for(int j = 6; j >= 0; j--) {

                Pack_Bit(led_driver->DC_Data[driver][led][color] & (1<<j)); //100% max current output)
            }
        }
    }

}

/*
    Initializes PWM frequency values to 100% duty Cycle
    Intializes default control settngs
*/
void Init_LED_Driver(TLC5955* led_driver) {

    for(int driver = 0; driver < NUM_DRIVERS; driver++) {

        for(int led = 0; led < NUM_LEDS_PER_CHIP; led++) {

            for(int color = 0; color < NUM_COLOR_CHANNELS; color++) {

                led_driver->GS_Data[driver][led][color] = 0x07FF; //Maximum Duty Cycle
                led_driver->DC_Data[driver][led][color] = DC_VAL; //100% max current output
            }
        }
    }

    //Maximum Current Data
    led_driver->MC_Data[RED] = MAX_CURR_RED; //19.1 mA
    led_driver->MC_Data[GREEN] = MAX_CURR_GREEN;
    led_driver->MC_Data[BLUE] = MAX_CURR_BLUE;

    //Brightness Control Data
    led_driver->BC_Data[RED] = BRIGHT_CORR_RED; //100% of Maximum Current Output
    led_driver->BC_Data[GREEN] = BRIGHT_CORR_GREEN;
    led_driver->BC_Data[BLUE] = BRIGHT_CORR_BLUE;

    //Function Control Data
    led_driver->FC_Data[DSPRPT] = true; //want pwm display period to repeat
    led_driver->FC_Data[TMGRST] = false;
    led_driver->FC_Data[RFRESH] = false;
    led_driver->FC_Data[ESPWM] = false;
    led_driver->FC_Data[LSDVLT] = true;

    Pin_Write(PIN_LED_DRIVER_LAT_PORT, PIN_LED_DRIVER_LAT, 0);

    Send_Control_Data(led_driver);
    Send_GS_Data(led_driver);

}

/*
    Sends control data - Function Control, Maximum Current, Brightness, Dot correction
*/
void Send_Control_Data(TLC5955* led_driver) {
    Pin_Write(PIN_LED_DRIVER_LAT_PORT, PIN_LED_DRIVER_LAT, 0);

    int i,j;

    current_bit_position = 0; //reset current bit position counter for packing bits

    for(i = 0; i < (16 - NUM_DRIVERS); i++) { //pad front with dummy bits to make multiple of 16
        Pack_Bit(1);
    }

    for(i = 0; i < NUM_DRIVERS; i++) {

        Pack_Bit(1); // Leading bit to write to let Driver know we want to write to Control Data
        Pack_Byte(0x96); //Byte to let driver know we want to write

        //now need to pad for the extra bits
            //769 (total bits) - 1 (leading bit) - 8 (write byte) - 5 (FC) - 9 (MC) - 21 (BC) - 7*48 (DC)
            // =  389 bits
        for(j = 0; j < 389; j++) {
            Pack_Bit(0);
        }

        //5 FC bits first
        Pack_FC(led_driver);

        //Then Pack the 21 global brightness Bits
        Pack_BC(led_driver);

        //Then pack the 9 MC bits
        Pack_MC(led_driver);

        //Then pack the 7 * 48 DC Data bits
        Pack_DC(led_driver, i);
    }

    //Bitbang the data
    //Sent twice to make sure the Maximum Current Data is written
    transmit_driver_data(led_driver, false);
    Toggle_Lat();
    DWT_Delay_us(1000);
    transmit_driver_data(led_driver, false);
    Toggle_Lat();
    }

void Set_DC_Data(TLC5955* led_driver, int driver, int led, int color, uint16_t dc_val) {
    led_driver->DC_Data[driver][led][color] = dc_val;
    Send_Control_Data(led_driver);
}

void Set_MC_BC(TLC5955* led_driver, uint8_t MC_RED, uint8_t MC_BLUE, uint8_t MC_GREEN, uint8_t BC_RED, uint8_t BC_GREEN, uint8_t BC_BLUE) {
    //Maximum Current Data
    led_driver->MC_Data[RED] = MC_RED; //19.1 mA
    led_driver->MC_Data[GREEN] = MC_GREEN;
    led_driver->MC_Data[BLUE] = MC_BLUE;

    //Brightness Control Data
    led_driver->BC_Data[RED] = BC_RED; //100% of Maximum Current Output
    led_driver->BC_Data[GREEN] = BC_GREEN;
    led_driver->BC_Data[BLUE] = BC_BLUE;

}

//Sets the value of a specific led channel
void Set_GS_Data(TLC5955* led_driver, int driver, int led, int color, uint16_t pwm_val) {

    int led_num = (driver == 0) ? left_led_ordering[led] : right_led_ordering[led];
    led_driver->GS_Data[driver][led_num][color] = pwm_val;
}

uint16_t Get_GS_Data(TLC5955* led_driver, int driver, int led, int color) {

    int led_num = (driver == 0) ? left_led_ordering[led] : right_led_ordering[led];
    return led_driver->GS_Data[driver][led_num][color];
}

/*
    Sets all the leds to an intensity
*/
void Set_All_GS_Data(TLC5955* led_driver, uint16_t pwm_val) {

    for(int driver = 0; driver < NUM_DRIVERS; driver++) {

        Set_All_Led_Driver(led_driver, driver, pwm_val);
    }
}

void Set_All_Color(TLC5955* led_driver, uint16_t pwm_val, uint8_t color) {
    for(int driver = 0; driver < NUM_DRIVERS; driver++) {

        for(int led = 0; led < NUM_LEDS_PER_CHIP; led++) {

            led_driver->GS_Data[driver][led][color] = pwm_val; //Maximum Duty Cycle

        }
    }
}

void Set_All_Led_Driver(TLC5955* led_driver, int driver, uint16_t pwm_val) {
    for(int led = 0; led < NUM_LEDS_PER_CHIP; led++) {
        for(int color = 0; color < NUM_COLOR_CHANNELS; color++) {
            led_driver->GS_Data[driver][led][color] = pwm_val; //Maximum Duty Cycle
        }

    }
}

/*
    turns on a certain number of leds based on a given percentage
    Leds turn from Red to green as the percentage increases
*/
void Set_LED_Bar_Increase_RG_Individual(TLC5955* led_driver, float battery_percentage, LED_BAR bar) {

    float f_leds_on = battery_percentage * (float)NUM_BATTERY_LEVEL_LEDS;
    int leds_on = (int)f_leds_on;
    float fractional_led = f_leds_on - leds_on;

    Set_All_Led_Driver(led_driver, bar, 0);

    for(int led = 0; led < leds_on; led++) {

        Set_GS_Data(led_driver, bar, led, RED, 0x0);
        Set_GS_Data(led_driver, bar, led, GREEN, 0xFFFF);
        Set_GS_Data(led_driver, bar, led, BLUE, 0x0);
    }
    int fractional_led_num;
    if(leds_on < 16) {

        if(leds_on == 0) {
            fractional_led_num = 0;
        } else {
            fractional_led_num = leds_on;
        }

        if(fractional_led > 0.01) {
            Set_GS_Data(led_driver, bar, fractional_led_num, RED, (uint16_t)((1-fractional_led) * 0xFFFF));
            Set_GS_Data(led_driver, bar, fractional_led_num, GREEN, (uint16_t)(fractional_led * 0xFFFF));
            Set_GS_Data(led_driver, bar, fractional_led_num, BLUE, 0x0);
        } else {
            Set_GS_Data(led_driver, bar, fractional_led_num, RED, 0);
            Set_GS_Data(led_driver, bar, fractional_led_num, GREEN,0);
            Set_GS_Data(led_driver, bar, fractional_led_num, BLUE, 0x0);
        }

    }

    Send_GS_Data(led_driver);
}

void Set_LED_Bar_Increase_GYR_Bar(TLC5955* led_driver, float power_percentage, LED_BAR bar) {
    float f_leds_on = power_percentage * (float)NUM_BATTERY_LEVEL_LEDS;
    int leds_on = (int)f_leds_on;
    if(leds_on > 16) {
        leds_on = 16;
    }
    float fractional_led = f_leds_on - leds_on;
    Set_All_Led_Driver(led_driver, bar, 0);
    // green for the first ...
    for(uint8_t led = 0; led < leds_on; led++) {
    	uint16_t red_val;
    	uint16_t green_val;
    	uint16_t blue_val = 0x0;
        if(led < 9) {
            red_val = 0x0;
            green_val = 0xFFFF;
        }
        else if(led > 9 && led < 14) {
            red_val =  0xFFFF;
            green_val = 0x1FFF;
        }
        else {
            red_val = 0xFFFF;
            green_val = 0x0;
        }
        Set_GS_Data(led_driver, bar, led, RED, red_val);
        Set_GS_Data(led_driver, bar, led, GREEN, green_val);
        Set_GS_Data(led_driver, bar, led, BLUE, blue_val);
    }
    int fractional_led_num;

    if(leds_on < 16 && fractional_led > 0.01) {

        fractional_led_num = leds_on;

        if(fractional_led_num < 9) {
            Set_GS_Data(led_driver, bar, fractional_led_num, RED, 0x0);
            Set_GS_Data(led_driver, bar, fractional_led_num, GREEN, (uint16_t) (fractional_led * 0xFFFF));
        }
        else if(fractional_led_num > 9 && fractional_led_num < 14) {
            Set_GS_Data(led_driver, bar, fractional_led_num, RED, (uint16_t) (fractional_led * 0xFFFF));
            Set_GS_Data(led_driver, 1, fractional_led_num, GREEN, (uint16_t) (fractional_led * 0x1FFF));
        }
        else {
            Set_GS_Data(led_driver, bar, fractional_led_num, RED, (uint16_t) (fractional_led * 0xFFFF));
            Set_GS_Data(led_driver, bar, fractional_led_num, GREEN, 0x0);
        }
        Set_GS_Data(led_driver, bar, fractional_led_num, BLUE, 0x0);
    }
}



/*
    Packs the pwm freq data and bitbangs it
*/
void Send_GS_Data(TLC5955* led_driver) {
    Pin_Write(PIN_LED_DRIVER_LAT_PORT, PIN_LED_DRIVER_LAT, 0);

    current_bit_position = 0;

    int idx = 95;
    //MSB - LSB  is Blue Green Red order

    for(int i = 0; i < (16 - NUM_DRIVERS); i++) { //pad front with dummy bits to make multiple of 16
        Pack_Bit(1);
    }

    for(int driver = NUM_DRIVERS - 1; driver >= 0; driver--) {

        Pack_Bit(0);

        for(int led = NUM_LEDS_PER_CHIP - 1; led >= 0; led--) {

            for(int color = 2; color >= 0; color--) {

                Pack_Short(led_driver->GS_Data[driver][led][color]);
                idx--;
            }

        }


    }

        transmit_driver_data(led_driver, true);
        Toggle_Lat();

}

/*
    Functions to bring clock pin high and low
*/
void SCK_Low(void) {
    Pin_Write(PIN_LED_DRIVER_SCK_PORT, PIN_LED_DRIVER_SCK, 0);
}

void SCK_Hi(void) {
    Pin_Write(PIN_LED_DRIVER_SCK_PORT, PIN_LED_DRIVER_SCK, 1);
}

/*
    Send driver data with either blocking or non-blocking interrupt-based spi
*/
void transmit_driver_data(TLC5955* led_driver, bool non_blocking) {

    UNUSED(led_driver);

    HAL_StatusTypeDef s;

    if(non_blocking) {

        s = spi_transmit_non_blocking(SPI1, (uint8_t*)driver_data, sizeof(driver_data)/sizeof(driver_data[0]));

    } else {
        s = spi_transmit(SPI1, (uint8_t*)driver_data, NUM_CHANNELS * NUM_DRIVERS + 1, 1000);
    }

    if(s != HAL_OK) {
// #ifdef DEBUG
//         if(s == HAL_BUSY) {
//             printf("Spi transmit hal busy\r\n");
//         } else if(s == HAL_TIMEOUT) {
//             printf("Spi transmit hal timeout\r\n");
//         } else if(s == HAL_ERROR) {
//             printf("Spi transmit hal error\r\n");
//         }
// #endif
    }
}

// colorful functions:


void pretty(TLC5955* led_driver) {
    for(int i = 0; i<16; i++) {
        Set_GS_Data(led_driver, 0, i, RED, pretty_colors[(i + pretty_color_offset)%16][0]);
        Set_GS_Data(led_driver, 1, i, RED, pretty_colors[(i + pretty_color_offset)%16][0]);
        Set_GS_Data(led_driver, 0, i, GREEN, pretty_colors[(i + pretty_color_offset)%16][1]);
        Set_GS_Data(led_driver, 1, i, GREEN, pretty_colors[(i + pretty_color_offset)%16][1]);
        Set_GS_Data(led_driver, 0, i, BLUE, pretty_colors[(i + pretty_color_offset)%16][2]);
        Set_GS_Data(led_driver, 1, i, BLUE, pretty_colors[(i + pretty_color_offset)%16][2]);
    }
    Send_GS_Data(led_driver);
    pretty_color_offset++;
    if(pretty_color_offset == 16) {
        pretty_color_offset = 0;
    }
}

void cycle_RGB(TLC5955* led_driver) {
    if(color == RED) {
        color = BLUE;
        zero_color_1 = GREEN;
        zero_color_2 = RED;
    } else if(color == BLUE) {
        color = GREEN;
        zero_color_1 = BLUE;
        zero_color_2 = RED;
    } else {
        color = RED;
        zero_color_1 = GREEN;
        zero_color_2 = BLUE;
    }

    Set_All_Color(led_driver, 0xFFFF, color);
    Set_All_Color(led_driver, 0x0, zero_color_2);
    Set_All_Color(led_driver, 0x0, zero_color_1);

    Send_GS_Data(led_driver);
}

void fill_cell_voltage_bar(TLC5955* led_driver, LED_BAR bar) {
    int32_t power = CANlib_critical_CurrentSensor_Voltage1_Input.result - MIN_PACK_VOLTAGE;
    float battery_percentage_voltage = (float)power / (float)(MAX_PACK_VOLTAGE - MIN_PACK_VOLTAGE);
    battery_percentage_voltage = (battery_percentage_voltage > 1.0) ? 1.0 : battery_percentage_voltage;
    battery_percentage_voltage = (battery_percentage_voltage < 0.0) ? 0.0 : battery_percentage_voltage;
    Set_LED_Bar_Increase_RG_Individual(led_driver, battery_percentage_voltage, bar);
}

void fill_power_bar(TLC5955* led_driver, LED_BAR bar) {
    float power_percentage = (float) CANlib_critical_CurrentSensor_Power_Input.result / (float) MAX_POWER;
    power_percentage = (power_percentage > 1.0) ? 1.0 : power_percentage;
    power_percentage = (power_percentage < 0.0) ? 0.0 : power_percentage;
    Set_LED_Bar_Increase_GYR_Bar(led_driver, power_percentage, bar);
}

void fill_cell_temp_bar(TLC5955* led_driver, LED_BAR bar) {
    int16_t cell_temp = CANlib_critical_CellTemperatureRange_Input.max - MIN_CELL_TEMP;
    float temp_percentage = (float) cell_temp / (float) (MAX_CELL_TEMP - MIN_CELL_TEMP);
    //
    temp_percentage = (temp_percentage > 1.0) ? 1.0 : temp_percentage;
    temp_percentage = (temp_percentage < 0.0) ? 0.0 : temp_percentage;
    Set_LED_Bar_Increase_GYR_Bar(led_driver, temp_percentage, bar);
}

void fill_factor_bar(TLC5955* led_driver, LED_BAR bar) {
    float fac = (float) controls_settings2.torque_factor;
    float fac_percentage = fac / 100.0;
    fac_percentage = (fac_percentage > 1.0) ? 1.0 : fac_percentage;
    fac_percentage = (fac_percentage < 0.0) ? 0.0 : fac_percentage;
    Set_LED_Bar_Increase_GYR_Bar(led_driver, fac_percentage, bar);
}

uint32_t last_handle_led_bars = 0;
void handle_led_bars(TLC5955* led_driver) {
    if(HAL_GetTick() - last_handle_led_bars > HANDLE_LED_BARS_PERIOD) {
        fill_factor_bar(led_driver, LEFT_BAR);
        fill_cell_temp_bar(led_driver, RIGHT_BAR);
        last_handle_led_bars = HAL_GetTick();
    }
}
