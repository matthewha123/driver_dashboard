#include "button.h"
#include "button_test_cases.h"
#include <unistd.h>
#include "button.c"

void test_make_button() {
	Button b;
	make_button(&b, PIN_BUTTON_PORT, PIN_BUTTON);
	TEST_ASSERT_EQUAL_MESSAGE(PIN_BUTTON_PORT, b.port, "Button port is wrong");
	TEST_ASSERT_EQUAL_MESSAGE(PIN_BUTTON, b.pin, "Button pin is wrong");
	TEST_ASSERT_EQUAL_MESSAGE(true, b.last_val, "Button's last_val inititialized incorrectly");
	TEST_ASSERT_EQUAL_MESSAGE(false, b.edge, "Button's edge inititialized incorrectly");
	TEST_ASSERT_EQUAL_MESSAGE(0, b.press_count,"Button's press_count inititialized incorrectly");
}

void test_short_press() {
	Button b;
	make_button(&b, PIN_BUTTON_PORT, PIN_BUTTON);

	Pin_Write(PIN_BUTTON_PORT, PIN_BUTTON, false);
	TEST_ASSERT_EQUAL_MESSAGE(HELD, poll(&b), "Button not registering pressed");
	usleep(200000);
	Pin_Write(PIN_BUTTON_PORT, PIN_BUTTON, true);
	TEST_ASSERT_EQUAL_MESSAGE(SHORT_PRESS, poll(&b), "Button not registering short_press");
	TEST_ASSERT_EQUAL_MESSAGE(UNPRESSED, poll(&b), "Button not registering unpressed");
	TEST_ASSERT_EQUAL_MESSAGE(1, b.press_count,"Button's press_count inititialized incorrectly");

}

void test_long_press() {
	Button b;
	make_button(&b, PIN_BUTTON_PORT, PIN_BUTTON);

	Pin_Write(PIN_BUTTON_PORT, PIN_BUTTON, false);
	TEST_ASSERT_EQUAL_MESSAGE(HELD, poll(&b), "Button not registering pressed");
	usleep(800000);
	Pin_Write(PIN_BUTTON_PORT, PIN_BUTTON, true);
	TEST_ASSERT_EQUAL_MESSAGE(LONG_PRESS, poll(&b), "Button not registering short_press");
	TEST_ASSERT_EQUAL_MESSAGE(UNPRESSED, poll(&b), "Button not registering unpressed");
	TEST_ASSERT_EQUAL_MESSAGE(1, b.press_count,"Button's press_count inititialized incorrectly"); 
}

void test_button() {
	printf("---BUTTON TESTS---\r\n");
	RUN_TEST(test_make_button);
	RUN_TEST(test_short_press);
	RUN_TEST(test_long_press);
}