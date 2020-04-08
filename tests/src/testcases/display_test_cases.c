#include "display_test_cases.h"
#include "Dot_Matrix_NHD_US25664.c"

void test_oled_send_byte() {
	Oled_T oled;

	uint8_t oled_sdi_buffer[2];
	memset(oled_sdi_buffer, 0, sizeof(oled_sdi_buffer[0]) * 2);
	init_buffer_byte(oled_sdi_buffer, PIN_OLED_SDI);
	oled_send_byte(&oled, 0x34);
	deinit_fill_buffer();
	TEST_ASSERT_EQUAL_MESSAGE(0x34, oled_sdi_buffer[0], "oled_send_byte sent wrong data");
}

void test_oled_send_cmd() {
	Oled_T oled;

	uint8_t oled_sdi_buffer[2];
	memset(oled_sdi_buffer, 0, sizeof(oled_sdi_buffer[0]) * 2);
	init_buffer_byte(oled_sdi_buffer, PIN_OLED_SDI);
	oled_send_cmd(&oled, ENHANCE_A);
	deinit_fill_buffer();

	TEST_ASSERT_EQUAL_MESSAGE(0, oled_sdi_buffer[0] & 0x80, "First bit (command bit) not set to 0");
	TEST_ASSERT_EQUAL_MESSAGE(ENHANCE_A, ((oled_sdi_buffer[0]) << 1) | (oled_sdi_buffer[1] >> 7), "oled send cmd sent wrong command");
}

void test_oled_send_data() {
	Oled_T oled;

	uint8_t oled_sdi_buffer[2];
	memset(oled_sdi_buffer, 0, sizeof(oled_sdi_buffer[0]) * 2);
	init_buffer_byte(oled_sdi_buffer, PIN_OLED_SDI);
	oled_send_data(&oled, 0xF5);
	deinit_fill_buffer();

	TEST_ASSERT_EQUAL_MESSAGE(0x80, oled_sdi_buffer[0] & 0x80, "First bit (data bit) not set to 1");
	oled_sdi_buffer[0] <<= 1;
	oled_sdi_buffer[0] = oled_sdi_buffer[0] | oled_sdi_buffer[1] >> 7;
	TEST_ASSERT_EQUAL_MESSAGE(0xF5, oled_sdi_buffer[0], "oled send cmd sent wrong command");
}

void test_oled_set_column_addr() {
	Oled_T oled;

	uint8_t oled_sdi_buffer[4];
	memset(oled_sdi_buffer, 0, sizeof(oled_sdi_buffer[0]) * 4);	
	init_buffer_byte(oled_sdi_buffer, PIN_OLED_SDI);
	uint8_t start = 0x09; 
	uint8_t end = 0x04;
	set_column_addr(&oled, start,end);
	deinit_fill_buffer();

	uint8_t expected_bytes[] = {SET_COL_ADDR, start, end};
	uint8_t expected_command_bits[] = {0, 128, 128};

	test_sequence_bitbanged_bytes(oled_sdi_buffer, expected_bytes, expected_command_bits, 3);
}

void test_oled_set_row_addr() {
	Oled_T oled;

	uint8_t oled_sdi_buffer[4];
	memset(oled_sdi_buffer, 0, sizeof(oled_sdi_buffer[0]) * 4);	
	init_buffer_byte(oled_sdi_buffer, PIN_OLED_SDI);

	uint8_t start = 0x09; 
	uint8_t end = 0x04;
	set_row_addr(&oled, start,end);

	deinit_fill_buffer();

	uint8_t expected_bytes[] = {SET_ROW_ADDR, start, end};
	uint8_t expected_command_bits[] = {0, 128, 128};

	test_sequence_bitbanged_bytes(oled_sdi_buffer, expected_bytes, expected_command_bits, 3);
}

void test_start_write_ram() {
	Oled_T oled;

	uint8_t oled_sdi_buffer[2];
	memset(oled_sdi_buffer, 0, sizeof(oled_sdi_buffer[0]) * 2);
	init_buffer_byte(oled_sdi_buffer, PIN_OLED_SDI);
	start_write_ram(&oled, 0, 0);
	deinit_fill_buffer();

	uint8_t expected_bytes[] = {SET_COL_ADDR, 0, MAXCOLS, SET_ROW_ADDR, 0, MAXROWS, WRITE_RAM_CMD};
	uint8_t expected_command_bits[] = {0, 128, 0,0,128,0,0};

	test_sequence_bitbanged_bytes(oled_sdi_buffer, expected_bytes, expected_command_bits, 1);
}

/*
	Currently this only works when sending less than 8 commands
*/
void test_sequence_bitbanged_bytes(uint8_t* buffer, uint8_t * expected_bytes, uint8_t* expected_command_bits, uint8_t num_comamnds) {
	uint8_t shift = 0;
	for(int i = 0; i < num_comamnds; i ++) {
		TEST_ASSERT_EQUAL_MESSAGE(expected_command_bits[i] >> shift , buffer[i] & (0x80 >> shift), "First bit (command bit), not set correctly");
		buffer[i] <<= 1 + shift;
		buffer[i] = buffer[i] | (buffer[i + 1] >> (7 - shift));
		TEST_ASSERT_EQUAL_MESSAGE(expected_bytes[i], buffer[i], "Sent wrong byte");
		shift++;
	}
}

void test_oled_send_char() {
	Oled_T oled;
	initialize_oled(&oled);

	uint8_t y = 10;
	uint8_t x = 4;

	for(uint8_t offset = 0; offset < 4; offset++) {
		for(uint8_t i = 1; i<94; i++) {
				oled_send_char(&oled, x + offset,y, i);
				verify_character(&oled, x + offset, y, i + ' ');
				initialize_oled(&oled);
		}
	}
}

void verify_character(Oled_T* oled, uint8_t x, uint8_t y, char c) {
	uint8_t col_start = x / 4;
	uint8_t row_start = y;
	uint8_t x_offset = x - (x/4) * 4;

	uint8_t font_idx = c - ' ';
	uint8_t width = powerGreen_12ptDescriptors[font_idx][0];
	uint8_t height = powerGreen_12ptDescriptors[font_idx][1];
	uint16_t char_start_addr = powerGreen_12ptDescriptors[font_idx][2];
	uint16_t char_end_addr = powerGreen_12ptDescriptors[font_idx + 1][2];

	uint8_t font_buffer[char_end_addr - char_start_addr + 1];

	memset(font_buffer, 0, char_end_addr - char_start_addr + 1);

	uint8_t num_col_in_row = (x_offset == 3) ? (width / 4) + 2 : (width / 4) + 1;

	if(width == 7 || width == 3) {
		num_col_in_row++;
	}

	uint8_t font_buffer_idx = 0;
	uint8_t pix_filled = 0;
	uint8_t nibble_shift = 0;

	bool next_row = false;



	for(int row = row_start; row < row_start + height; row++) {
		next_row = false;
		if(pix_filled >= width) {
			pix_filled = 0;
		}
		for(int col = col_start; col < col_start + num_col_in_row; col++) {

			uint8_t* data = oled->gddr[get_gddr_idx(col + START_ADDR, row)].data;
			for(int k = 0; k < 4; k++) {

				if(k >= x_offset || (pix_filled != 0)) {
					if(pix_filled % 8 == 0 && pix_filled != 0) {
						font_buffer_idx++;
					}

					if(pix_filled == width) {
						pix_filled = 0;
						next_row = true;
						nibble_shift = 0;
						break;
					}
					uint8_t nibble = data[k/2] & (0xF0 >> nibble_shift);
					uint8_t bit = (nibble == 0) ? (0x0) : (0x01 << (7 - (pix_filled % 8)));

					font_buffer[font_buffer_idx] |= bit;
					pix_filled++;
				}
				nibble_shift = (nibble_shift == 0) ? 4 : 0;

			}
			if(next_row) {
				break;
			}

		}
		font_buffer_idx++;
	}

	char* str[] = {c};

	bool wrong = false;
	for(int i = 0; i < char_end_addr - char_start_addr; i++) {

		if(powerGreen_12ptBitmaps[i+char_start_addr] != font_buffer[i]) {
			wrong = true;
		}
		TEST_ASSERT_EQUAL_MESSAGE(powerGreen_12ptBitmaps[i+char_start_addr], font_buffer[i], "Character was sent incorrectly");
	}

	if(wrong) {
		printf(str);
		printf(" was wrong with offset: %d\r\n", x - 4);
	}
}

void test_display() {
	printf("---DISPLAY TESTS---\r\n");
	RUN_TEST(test_oled_send_byte);
	RUN_TEST(test_oled_send_cmd);
	RUN_TEST(test_oled_send_data);
	RUN_TEST(test_oled_set_column_addr);
	RUN_TEST(test_oled_set_row_addr);
	RUN_TEST(test_start_write_ram);
	RUN_TEST(test_oled_send_char);
}