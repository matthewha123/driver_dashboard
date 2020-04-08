#include "gpio.h"


bool gpio_clock_A_enabled = false;
bool gpio_clock_B_enabled = false;
bool gpio_clock_C_enabled = false;
bool gpio_clock_D_enabled = false;
bool gpio_clock_E_enabled = false;
bool gpio_clock_F_enabled = false;
bool gpio_clock_G_enabled = false;
bool gpio_clock_H_enabled = false;



bool fillBuffer = false;
uint8_t* byte_buffer;
uint16_t* short_buffer;
bool filling_byte_buffer = false;
Pin_T fillBuffer_Pin;
uint32_t current_bit_position = 0;


bool State_PIN_OLED_SCLK = false;
bool State_PIN_OLED_SDI = false;
bool State_PIN_OLED_CS = false;
bool State_PIN_OLED_RS = false;
bool State_PIN_OLED_RW = false;
bool State_PIN_OLED_E = false;
bool State_PIN_OLED_RS2 = false;
bool State_PIN_DEBUG_LED = false;
bool State_PIN_LED_DRIVER_GSCLK = false;
bool State_PIN_LED_DRIVER_LAT = false;
bool State_PIN_LED_DRIVER_SCK = false;
bool State_PIN_LED_DRIVER_SIN = false;
bool State_PIN_BUTTON = false;

void GPIO_Init(void) {

	gpio_clock_A_enabled = true;
	gpio_clock_B_enabled = true;
	gpio_clock_C_enabled = true;
	gpio_clock_D_enabled = true;
	gpio_clock_E_enabled = true;
	gpio_clock_F_enabled = true;
	gpio_clock_G_enabled = true;
	gpio_clock_H_enabled = true;

	printf("GPIO Init Called\r\n");
}

//do I want to be able to verify the entire array? 
	//IDEA:
		// Create a buffer for the Pin writes
		// Parameters that are needed
			// Data type size (how many bit bashes will require a shift in position in the buffer)
			// How many bytes to expect, or size of buffer
			// Pin to read from
			// So basically, this function works like our packing bit function

			// THe idea is after our display and led functions are called
				// The bit banging will fill these buffers
				// And after everything I can check the results

				
void Pin_Write(int port, Pin_T pin, bool pin_state){
	switch(pin) {
		case PIN_OLED_SCLK:
			State_PIN_OLED_SCLK = pin_state;
		break;
		case PIN_OLED_SDI:
			State_PIN_OLED_SDI = pin_state;
		break;
		case PIN_OLED_CS:
			State_PIN_OLED_CS = pin_state;
		break;
		case PIN_OLED_RS:
			State_PIN_OLED_RS = pin_state;
		break;
		case PIN_OLED_RW:
			State_PIN_OLED_RW = pin_state;
		break;
		case PIN_OLED_E:
			State_PIN_OLED_E = pin_state;
		break;
		case PIN_OLED_RS2:
			State_PIN_OLED_RS2 = pin_state;
		break;
		case PIN_DEBUG_LED:
			State_PIN_DEBUG_LED = pin_state;
		break;
		case PIN_LED_DRIVER_GSCLK:
			State_PIN_LED_DRIVER_GSCLK = pin_state;
		break;
		case PIN_LED_DRIVER_LAT:
			State_PIN_LED_DRIVER_LAT = pin_state;
		break;
		case PIN_LED_DRIVER_SCK:
			State_PIN_LED_DRIVER_SCK = pin_state;
		break;
		case PIN_LED_DRIVER_SIN:
			State_PIN_LED_DRIVER_SIN = pin_state;
		break;
		case PIN_BUTTON:
			State_PIN_BUTTON = pin_state;
		break;
	}



	if(fillBuffer && pin == fillBuffer_Pin) {
		// if(pin == PIN_OLED_SDI) {
		// 	printf("Attempting to pack bit %d\r\n", pin_state);
		// 	printf("byte_buffer[0]: %d\r\n", byte_buffer[0]);
		// }
		GPIO_Pack_Bit(pin_state);

	}
}

bool Pin_Read(int port, Pin_T pin) {
	switch(pin) {
		case PIN_OLED_SCLK:
			return State_PIN_OLED_SCLK;
		break;
		case PIN_OLED_SDI:
			return State_PIN_OLED_SDI;
		break;
		case PIN_OLED_CS:
			return State_PIN_OLED_CS;
		break;
		case PIN_OLED_RS:
			return State_PIN_OLED_RS;
		break;
		case PIN_OLED_RW:
			return State_PIN_OLED_RW;
		break;
		case PIN_OLED_E:
			return State_PIN_OLED_E;
		break;
		case PIN_OLED_RS2:
			return State_PIN_OLED_RS2;
		break;
		case PIN_DEBUG_LED:
			return State_PIN_DEBUG_LED;
		break;
		case PIN_LED_DRIVER_GSCLK:
			return State_PIN_LED_DRIVER_GSCLK;
		break;
		case PIN_LED_DRIVER_LAT:
			return State_PIN_LED_DRIVER_LAT;
		break;
		case PIN_LED_DRIVER_SCK:
			return State_PIN_LED_DRIVER_SCK;
		break;
		case PIN_LED_DRIVER_SIN:
			return State_PIN_LED_DRIVER_SIN;
		break;
		case PIN_BUTTON:
			return State_PIN_BUTTON;
		break;
	}
}

//Test flow
	// To test the bitbanging, call the init corresponding to the size of the data chunks you want
	// Specify which pin you want to snoop
	// Then on every Pin write with that pin, the array you provide will be filled
	// After the corresponding function that you are testing is finished, the buffer should be full of data that you can analyze
	// After you perform your checks, you can call deinit_fill_buffer
		// This merely resets the counter for bit position and makes sure that Pack_Bit isn't called on subsequent pin writes

void init_buffer_short(uint16_t* buffer, Pin_T pin) {
	short_buffer = buffer;
	current_bit_position = 0;
	fillBuffer_Pin = pin;
	fillBuffer = true;
	filling_byte_buffer = false;
}

void init_buffer_byte(uint8_t* buffer, Pin_T pin) {
	byte_buffer = buffer;
	current_bit_position = 0;
	fillBuffer_Pin = pin;
	fillBuffer = true;
	filling_byte_buffer = true;
}

void deinit_fill_buffer() {
	fillBuffer = false;
	current_bit_position = 0;
}

/*
	Given array value and bit offset, clears the corresponding bit
*/
void GPIO_Clear_Bit_Short(unsigned short* val, int offset) {
	*val = *val & ~(1<<offset); // Clear the bit in val at position offset
}

/*
	Given array value and bit offset, sets the corresponding bit
*/
void GPIO_Set_Bit_Short(unsigned short* val, int offset) {
	*val = *val | (1<<offset); //Set the bit in val at position offset
}

/*
	Given array value and bit offset, clears the corresponding bit
*/
void GPIO_Clear_Bit_Byte(uint8_t* val, int offset) {
	*val = *val & ~(1<<offset); // Clear the bit in val at position offset
}

/*
	Given array value and bit offset, sets the corresponding bit
*/
void GPIO_Set_Bit_Byte(uint8_t* val, int offset) {
	*val = *val | (1<<offset); //Set the bit in val at position offset
}

/*
	Given a bit, clears/sets a bit in the 16 bit data array sent to the LED driers
	Remembers the position of the bit last placed
*/
void GPIO_Pack_Bit(uint8_t bit) {

	uint8_t size = (filling_byte_buffer) ? 8 : 16;

	int position = current_bit_position / size;
	int offset = current_bit_position % size;
	if(bit == 0) {
		if(filling_byte_buffer) {
			GPIO_Clear_Bit_Byte(&(byte_buffer[position]), size - 1 - offset);
		} else {
			GPIO_Clear_Bit_Short(&(short_buffer[position]), size - 1 - offset);
		}

	} else {
		if(filling_byte_buffer) {
			GPIO_Set_Bit_Byte(&(byte_buffer[position]), size - 1 - offset);
		} else {
			GPIO_Set_Bit_Short(&(short_buffer[position]), size - 1 - offset);
		}
	}

	current_bit_position++;
}

