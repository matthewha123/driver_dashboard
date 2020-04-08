#include "tlc_test_cases.h"
#include "TLC5955.c"

int _left_led_ordering[16] = {7,3,6,2,1,5, 0 ,4,11,15,10,14,13,9,12,8}; //mapping indices of GS_Data to the ordering of led's left to right
int _right_led_ordering[16] = {4, 0 ,5,1,2,6,3,7,8,12,9,13,14,10,15,11};

void test_tlc_init() {
	TLC5955 led_driver;
	Init_LED_Driver(&led_driver);

	TEST_ASSERT_EQUAL_MESSAGE(MAX_CURR_RED, led_driver.MC_Data[RED], "Red max current intialized wrong");
	TEST_ASSERT_EQUAL_MESSAGE(MAX_CURR_GREEN, led_driver.MC_Data[GREEN], "Green max current intialized wrong");
	TEST_ASSERT_EQUAL_MESSAGE(MAX_CURR_BLUE, led_driver.MC_Data[BLUE], "Blue max current intialized wrong");

	TEST_ASSERT_EQUAL_MESSAGE(BRIGHT_CORR_RED, led_driver.BC_Data[RED], "Red brightness intialized wrong");
	TEST_ASSERT_EQUAL_MESSAGE(BRIGHT_CORR_GREEN, led_driver.BC_Data[GREEN], "Green brightness intialized wrong");
	TEST_ASSERT_EQUAL_MESSAGE(BRIGHT_CORR_BLUE, led_driver.BC_Data[BLUE], "Blue brightness intialized wrong");

	TEST_ASSERT_EQUAL_MESSAGE(true, led_driver.FC_Data[DSPRPT], "DSPRPT intialized wrong");
	TEST_ASSERT_EQUAL_MESSAGE(false, led_driver.FC_Data[TMGRST], "TMGRST intialized wrong");
	TEST_ASSERT_EQUAL_MESSAGE(false, led_driver.FC_Data[RFRESH], "RFRESH intialized wrong");
	TEST_ASSERT_EQUAL_MESSAGE(false, led_driver.FC_Data[ESPWM], "ESPWM intialized wrong");
	TEST_ASSERT_EQUAL_MESSAGE(true, led_driver.FC_Data[LSDVLT], "LSDVLT intialized wrong");

	for(int driver = 0; driver < NUM_DRIVERS; driver++) {

		for(int led = 0; led < NUM_LEDS_PER_CHIP; led++) {

			for(int color = 0; color < NUM_COLOR_CHANNELS; color++) {
				
				TEST_ASSERT_EQUAL_MESSAGE(0xFFFF, led_driver.GS_Data[driver][led][color], "Grayscale data not initalized to max duty cycle");
				TEST_ASSERT_EQUAL_MESSAGE(0x7F, led_driver.DC_Data[driver][led][color], "Dot correction data not initalized correctly");

			}
		}
	}
}

void test_latch() {
	uint8_t latch_buffer[1];
	memset(latch_buffer, 0, sizeof(latch_buffer[0]));
	init_buffer_byte(latch_buffer, PIN_LED_DRIVER_LAT);
	Toggle_Lat();
	TEST_ASSERT_EQUAL_MESSAGE(64, latch_buffer[0], "latch not toggling correctly");
	deinit_fill_buffer();
}

void test_send_control_data() {
	TLC5955 led_driver;
	Init_LED_Driver(&led_driver);

	uint16_t led_sin_buffer[(NUM_CHANNELS * NUM_DRIVERS + 1) * 2];
	memset(led_sin_buffer, 0, sizeof(led_sin_buffer[0]) * (NUM_CHANNELS * NUM_DRIVERS + 1) * 2);

	init_buffer_short(led_sin_buffer, PIN_LED_DRIVER_SIN);

	Send_Control_Data(&led_driver);
	deinit_fill_buffer();

	for(int i =0; i < (NUM_CHANNELS * NUM_DRIVERS + 1) * 2; i++) {
		// CHECK VALUE OF CONTROL DATA HERE
		// printf("%d \r\n", led_sin_buffer[i]);
	}

	uint8_t send_control_data_latch_buffer[1];
	memset(send_control_data_latch_buffer, 0, sizeof(send_control_data_latch_buffer[0]));
	init_buffer_byte(send_control_data_latch_buffer, PIN_LED_DRIVER_LAT);
	Send_Control_Data(&led_driver);
	TEST_ASSERT_EQUAL_MESSAGE(36, send_control_data_latch_buffer[0], "latch not toggling correctly in send_control_data()");
	deinit_fill_buffer();
}

void test_send_gs_data() {
	TLC5955 led_driver;
	Init_LED_Driver(&led_driver);

	uint16_t led_sin_buffer[NUM_CHANNELS * NUM_DRIVERS + 1];
	init_buffer_short(led_sin_buffer, PIN_LED_DRIVER_SIN);

	Send_GS_Data(&led_driver);

	deinit_fill_buffer();

	for(int i =0; i < NUM_CHANNELS * NUM_DRIVERS + 1; i++) {
		//CHECK VALUE OF GRAYSCALE DATA HERE
		// printf("%d \r\n", led_sin_buffer[i]);
	}
	uint8_t send_gs_data_latch_buffer[1];
	memset(send_gs_data_latch_buffer, 0, sizeof(send_gs_data_latch_buffer[0]));
	init_buffer_byte(send_gs_data_latch_buffer, PIN_LED_DRIVER_LAT);

	Send_GS_Data(&led_driver);
	TEST_ASSERT_EQUAL_MESSAGE(32, send_gs_data_latch_buffer[0], "latch not toggling correctly in send_gs_data()");
	deinit_fill_buffer();
}

void test_set_gs_data() {
	TLC5955 led_driver;
	Init_LED_Driver(&led_driver);
	int driver = 0;
	int led = 14;
	Set_GS_Data(&led_driver, driver, led, RED, 1024);
	TEST_ASSERT_EQUAL_MESSAGE(1024, Get_GS_Data(&led_driver, driver, led, RED), "set_gs_data not setting the correct value");

}

void test_set_all_gs_data() {
	TLC5955 led_driver;
	Init_LED_Driver(&led_driver);
	Set_All_GS_Data(&led_driver, 578);

	for(int driver = 0; driver < NUM_DRIVERS; driver++) {

		for(int led = 0; led < NUM_LEDS_PER_CHIP; led++) {

			for(int color = 0; color < NUM_COLOR_CHANNELS; color++) {
				
				TEST_ASSERT_EQUAL_MESSAGE(578, led_driver.GS_Data[driver][led][color], "set_all_gs_data not setting the correct value");

			}
		}
	}
}

void test_set_percentage() {
	TLC5955 led_driver;
	Init_LED_Driver(&led_driver);
	Set_Percentage(&led_driver, 50.1);
	int led;
	int driver = 0;
	for(led = 0; led < 8; led ++) {
		TEST_ASSERT_EQUAL_MESSAGE(0, Get_GS_Data(&led_driver, driver, led, RED), "There should not be red color in a lit battery level led");
		TEST_ASSERT_EQUAL_MESSAGE(0xFFFF, Get_GS_Data(&led_driver, driver, led, GREEN), "Should be max duty cycle green");
		TEST_ASSERT_EQUAL_MESSAGE(0, Get_GS_Data(&led_driver, driver, led, BLUE), "There should not be blue color in a lit battery level led");
	}
	int _led = _left_led_ordering[8];
	TEST_ASSERT_EQUAL_MESSAGE((1.0-.016) * 0xFFFF, Get_GS_Data(&led_driver, driver, 8, RED), "There should be mostly Red color in the 9th led");
	TEST_ASSERT_EQUAL_MESSAGE(.016*0xFFFF, Get_GS_Data(&led_driver, driver, 8, GREEN), "There should be a little green color in 9th led");
	TEST_ASSERT_EQUAL_MESSAGE(0, Get_GS_Data(&led_driver, driver, 8, BLUE), "There should not be blue color in a lit battery level led");

	for(led = 9; led < NUM_BATTERY_LEVEL_LEDS; led++ ) {
		int _led = _left_led_ordering[led];
		TEST_ASSERT_EQUAL_MESSAGE(0, Get_GS_Data(&led_driver, driver, led, RED), "These led's should be off");
		TEST_ASSERT_EQUAL_MESSAGE(0, Get_GS_Data(&led_driver, driver, led, GREEN), "These led's should be off");
		TEST_ASSERT_EQUAL_MESSAGE(0, Get_GS_Data(&led_driver, driver, led, BLUE), "These led's should be off");
	}

	Set_Percentage(&led_driver, 100);
	for(led = 0; led < 12; led ++) {
		TEST_ASSERT_EQUAL_MESSAGE(0, led_driver.GS_Data[0][led][RED], "There should not be red color in a lit battery level led");
		TEST_ASSERT_EQUAL_MESSAGE(0xFFFF, led_driver.GS_Data[0][led][GREEN], "Should be max duty cycle green");
		TEST_ASSERT_EQUAL_MESSAGE(0, led_driver.GS_Data[0][led][BLUE], "There should not be blue color in a lit battery level led");
	}
}

void test_set_DC() {
	TLC5955 led_driver;
	Init_LED_Driver(&led_driver);
	int led = 13;
	int color = BLUE;
	int driver = 0;
	uint16_t dc_val = 33;
	Set_DC_Data(&led_driver, driver, led, color, dc_val);
	TEST_ASSERT_EQUAL_MESSAGE(dc_val, led_driver.DC_Data[driver][led][color], "Dot correction data not set correctly");
}

void test_set_MC_BC() {
	TLC5955 led_driver;
	Init_LED_Driver(&led_driver);
	uint8_t MC_RED = 2;
	uint8_t MC_BLUE = 1;
	uint8_t MC_GREEN = 3;
	uint8_t BC_RED = 13;
	uint8_t BC_GREEN = 25;
	uint8_t BC_BLUE = 33;
	Set_MC_BC(&led_driver, MC_RED, MC_BLUE, MC_GREEN, BC_RED, BC_GREEN, BC_BLUE);
	TEST_ASSERT_EQUAL_MESSAGE(MC_RED, led_driver.MC_Data[RED], "Maximum current Red not set correctly");
	TEST_ASSERT_EQUAL_MESSAGE(MC_GREEN, led_driver.MC_Data[GREEN], "Maximum current green not set correctly");
	TEST_ASSERT_EQUAL_MESSAGE(MC_BLUE, led_driver.MC_Data[BLUE], "Maximum current blue not set correctly");

	TEST_ASSERT_EQUAL_MESSAGE(BC_RED, led_driver.BC_Data[RED], "brightness Red not set correctly");
	TEST_ASSERT_EQUAL_MESSAGE(BC_GREEN, led_driver.BC_Data[GREEN], "brightness green not set correctly");
	TEST_ASSERT_EQUAL_MESSAGE(BC_BLUE, led_driver.BC_Data[BLUE], "brightness blue not set correctly");
}

void test_sck() {
	SCK_Low();
	TEST_ASSERT_EQUAL_MESSAGE(false, Pin_Read(PIN_LED_DRIVER_SCK_PORT, PIN_LED_DRIVER_SCK), "SCK should be low");
	SCK_Hi();
	TEST_ASSERT_EQUAL_MESSAGE(true, Pin_Read(PIN_LED_DRIVER_SCK_PORT, PIN_LED_DRIVER_SCK), "SCK should be high");
}

void test_sin_write() {
	TLC5955 led_driver;
	Init_LED_Driver(&led_driver);
	uint8_t led_sin_buffer[] = {0};
	init_buffer_byte(led_sin_buffer, PIN_LED_DRIVER_SIN);
	SIN_Write(0, false, false);
	SIN_Write(1, false, false);
	SIN_Write(0, false, false);
	SIN_Write(1, false, false);
	deinit_fill_buffer();
	TEST_ASSERT_EQUAL_MESSAGE(80, led_sin_buffer[0], "SIN write not bitbanging correctly");
}


void test_tlc() {
	printf("---TLC TESTS---\r\n");
	RUN_TEST(test_tlc_init);
	RUN_TEST(test_latch);
	RUN_TEST(test_send_control_data);
	RUN_TEST(test_send_gs_data);
	RUN_TEST(test_set_gs_data);
	RUN_TEST(test_set_all_gs_data);
	RUN_TEST(test_set_percentage);
	RUN_TEST(test_set_DC);
	RUN_TEST(test_set_MC_BC);
	RUN_TEST(test_sck);
	RUN_TEST(test_sin_write);
}