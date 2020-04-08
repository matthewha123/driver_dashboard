#include "tests.h"
#include "gpio.h"
#include "uart.h"

void test_pin_rw(void) {
	Pin_Write(1,PIN_DEBUG_LED, true);
	TEST_ASSERT_EQUAL_MESSAGE(Pin_Read(1,PIN_DEBUG_LED), true, "Pin is not true");
	Pin_Write(1,PIN_DEBUG_LED, false);
	TEST_ASSERT_EQUAL_MESSAGE(Pin_Read(1,PIN_DEBUG_LED), false, "Pin is not false");
}

void test_pin_packing(void) {
	uint16_t led_buffer[200];
	init_buffer_short(led_buffer, PIN_LED_DRIVER_SIN);

	for(int i = 0; i < 200; i ++) {
		Pin_Write(1,PIN_LED_DRIVER_SIN, true);
		Pin_Write(1,PIN_LED_DRIVER_SIN, false);
		Pin_Write(0,PIN_LED_DRIVER_SIN, false);
		Pin_Write(0,PIN_LED_DRIVER_SIN, false);
		Pin_Write(1,PIN_LED_DRIVER_SIN, true);
		Pin_Write(0,PIN_LED_DRIVER_SIN, true);
		Pin_Write(1,PIN_LED_DRIVER_SIN, false);
		Pin_Write(1,PIN_LED_DRIVER_SIN, true);

		Pin_Write(1,PIN_LED_DRIVER_SIN, true);
		Pin_Write(1,PIN_LED_DRIVER_SIN, false);
		Pin_Write(0,PIN_LED_DRIVER_SIN, false);
		Pin_Write(0,PIN_LED_DRIVER_SIN, false);
		Pin_Write(1,PIN_LED_DRIVER_SIN, true);
		Pin_Write(0,PIN_LED_DRIVER_SIN, true);
		Pin_Write(1,PIN_LED_DRIVER_SIN, false);
		Pin_Write(1,PIN_LED_DRIVER_SIN, true);
	}

	TEST_ASSERT_EQUAL_MESSAGE(led_buffer[0], 36237, "WRONG SEQUENCE OF BITS SENT");
	deinit_fill_buffer();
}

int main() {
	UNITY_BEGIN();
	test_pin_packing();
	test_button();
	test_tlc();
	test_display();
	return UNITY_END();
}