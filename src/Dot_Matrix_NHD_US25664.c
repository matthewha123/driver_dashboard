#include "Dot_Matrix_NHD_US25664.h"
#include <stdio.h>

Oled_T ugfx_oled;
char numBuffer[20];

/*
    Bitbangs byte to display
*/
static void oled_send_byte(Oled_T* oled, uint8_t b) {

    UNUSED(oled);

    for(int i = 7; i >= 0; i--) {

        Pin_Write(OLED_GPIO_PORT, PIN_OLED_SCLK, 0);
        Pin_Write(OLED_GPIO_PORT, PIN_OLED_SDI, b & (0x01 << i));
        Pin_Write(OLED_GPIO_PORT, PIN_OLED_SCLK, 1);

    }
}

/*
    Sends command bit(0), then the command byte
*/
void oled_send_cmd(Oled_T* oled, uint8_t cmd) {

    // OLED_Data_Pins_Init();

    Pin_Write(OLED_GPIO_PORT, PIN_OLED_SCLK, 0);
    Pin_Write(OLED_GPIO_PORT, PIN_OLED_SDI, 0);
    Pin_Write(OLED_GPIO_PORT, PIN_OLED_SCLK, 1);

    oled_send_byte(oled, cmd);

    // spi_init(SPI3);
}

/*
    Sends data bit(0), then the data byte
*/
void oled_send_data(Oled_T* oled, uint8_t data) {

    // OLED_Data_Pins_Init();

    Pin_Write(OLED_GPIO_PORT, PIN_OLED_SCLK, 0);
    Pin_Write(OLED_GPIO_PORT, PIN_OLED_SDI, 1);
    Pin_Write(OLED_GPIO_PORT, PIN_OLED_SCLK, 1);

    oled_send_byte(oled, data);

    // spi_init(SPI3);
}

/*
    Sets the column address, where the display will start writing new display data
*/
static void set_column_addr(Oled_T* oled, unsigned char start, unsigned char end) {
    oled_send_cmd(oled, SET_COL_ADDR);
    oled_send_data(oled, start);
    oled_send_data(oled, end);
}

/*
    Sets the row address, where the display will start writing new display data
*/
static void set_row_addr(Oled_T* oled, unsigned char start, unsigned char end) {
    oled_send_cmd(oled, SET_ROW_ADDR);
    oled_send_data(oled, start);
    oled_send_data(oled, end);
}


/*
    Tells display to accept incoming data as display ram writes
*/
static void start_write_ram(Oled_T* oled, uint8_t col_num, uint8_t row_num) {
    set_column_addr(oled, col_num, MAXCOLS);
    set_row_addr(oled, row_num, MAXROWS);
    oled_send_cmd(oled, WRITE_RAM_CMD);
}

/*
    Writes one column's worth (2 bytes) of display data to display ram
*/
static void oled_write_ram(Oled_T* oled, uint8_t col_num, uint8_t row_num, uint8_t* data, uint8_t last_x) {

    oled_send_data(oled, data[0]);
    oled_send_data(oled, data[1]);

    uint16_t gddr_idx = get_gddr_idx(col_num, row_num);

    oled->gddr[gddr_idx].data[0] = data[0];
    oled->gddr[gddr_idx].data[1] = data[1];
    oled->gddr[gddr_idx].last_x = last_x;

}

static void oled_fill_old_col_data(Oled_T* oled, uint8_t* col, uint8_t start, uint8_t end, int col_num, int row_num) {
    uint8_t* gddr_col_data = oled->gddr[get_gddr_idx(col_num, row_num)].data;
    uint8_t col_byte_idx = start / 2;
    uint8_t nibble_mask_shift = (start % 2 == 0) ? 0 : 4;
    for(uint8_t i = start; i < end; i++) {
        if(i > 1) {
            col_byte_idx = 1;
        }
        col[col_byte_idx] |= gddr_col_data[col_byte_idx] & (0xF0>> nibble_mask_shift);
        nibble_mask_shift = (nibble_mask_shift == 0) ? 4 : 0;
    }
}

/*
    intializes display
*/
void initialize_oled(Oled_T* oled) {
    oled_send_cmd(oled, SET_CMD_LOCK);
    oled_send_data(oled, 0x12); // 0x12 allows the dislplay MCU to receive commands. 0x16 locks MCU

    oled_send_cmd(oled, SET_FRONT_CLK_DIV);
    oled_send_data(oled, 0x91); // first 4 bits is the clock divider, 0xD0 sets divider to 1. second 4 bits specified frequency of the clock.

    oled_send_cmd(oled, SET_MULTIPLEX_RATIO);
    oled_send_data(oled, 0x3F); // Sets the number of common pins, each of which control one row. 0x3F corresponds to 64 rows.

    oled_send_cmd(oled, SET_DISP_OFFSET);
    oled_send_data(oled, 0x0); // Specifies which row the display will start with.
                              // 0x0 = starting at row 0. Non zero value will cause vertical wrapping

    oled_send_cmd(oled, SET_DISP_START_LN);
    oled_send_data(oled, 0x0); // Functionally the same as SET_DISP_OFFSET

    oled_send_cmd(oled, SET_REMAP_DUAL_COM);
    oled_send_data(oled, 0x06);  // first bit Sets Horizontal Address Increment (writing to display ram will wrap to next row)
                                 // second bit Sets Column Address Remap - Pixels are represented by nibbles called segments. There are 4 segments in a column.
                                // This maps Columns 0 - 119 -> SEG(476-479 ~ 0-3 ), backwards.
                                // third bit Enables Nibble Re-map. Each nibble in a column maps to pixels in horizontal reading order.
                                // fourth bit Rows are displayed first to last
                               // fifth bit disables COM Split Odd Even: Com 0 will correspond to Row 0
    oled_send_data(oled, 0x0);  // fifth bit disables dual com mode (row's contents are split betwee two coms.)

    oled_send_cmd(oled, SET_GPIO); // Disables gpio input pins
    oled_send_data(oled, 0x00);

    oled_send_cmd(oled, SET_FUNC_SEL);
    oled_send_data(oled, 0x01); // Uses internal voltage regulattor

    oled_send_cmd(oled, ENHANCE_A);
    oled_send_data(oled, 0xA0); // first bit 0 enables external Voltage low reference signal
    oled_send_data(oled, 0xB5); // Normal display quality

    oled_send_cmd(oled, SET_CONTRAST_CURRENT);
    oled_send_data(oled, 0xFF); // Sets contrast leevl of display, higher value gives higher contrast (max 256)

    oled_send_cmd(oled, MASTER_CURR_CTRL);
    oled_send_data(oled, 0x0F); // first 4 bits - sets output current. Lower values reduce output current.

    oled_send_cmd(oled, SET_DEF_LN_GRAYSCALE_TBL); // Uses default gray scale table for brightness of pixels.

    oled_send_cmd(oled, SET_PHASE_LENGTH); // first 4 bits, sets phase 1 period to 5 clocks. Second 4 bits sets phase 2 period, set to 7 clocks
    oled_send_data(oled, 0xE2);

    oled_send_cmd(oled, ENHANCE_B);
    oled_send_data(oled, 0x82); // Set to "enhance display performance".
    oled_send_data(oled, 0x20); // Required to send, is not configurable

    oled_send_cmd(oled, SET_PRE_CHARGE_VOLTAGE);
    oled_send_data(oled, 0x1F); // sets precharge voltage to 0.6 * Vcc

    oled_send_cmd(oled, SET_SECOND_PRE_CHARGE_PERIOD);
    oled_send_data(oled, 0x08); // prechage period is 8 clocks

    oled_send_cmd(oled, SET_VCOMH);
    oled_send_data(oled, 0x07);  // sets vcomh to 0.86 * Vcc

    oled_send_cmd(oled, SET_DISPLAY_NORMAL);  // Normal, as opposed to off or inverted

    oled_send_cmd(oled, EXIT_PARTIAL_DISP);

    oled_send_cmd(oled, SET_SLEEP_MODE_OFF);

    for(uint16_t i = 0; i < (sizeof(oled->gddr) / sizeof(oled->gddr[0])); i++) {
        oled->gddr[i].data[0] = 0;
        oled->gddr[i].data[1] = 0;
        oled->gddr[i].last_x = 0;
    }
}

void start_display(Oled_T* oled) {

    Pin_Write(OLED_CS_PORT, PIN_OLED_CS, 0);
    Pin_Write(OLED_GPIO_PORT, PIN_OLED_RS, 0);
    Pin_Write(OLED_GPIO_PORT, PIN_OLED_RW, 0);
    Pin_Write(OLED_GPIO_PORT, PIN_OLED_E, 0);
    Pin_Write(OLED_GPIO_PORT, PIN_OLED_RS2, 1);

    HAL_Delay(100);

    initialize_oled(oled);
    clear_display(oled);
    // spi_init(SPI3);
}

/*
    Clears all the display ram, with 0's, also replaces Oled_T's gddram copy
*/
void clear_display(Oled_T* oled) {
    uint8_t i, j;

    set_column_addr(oled, START_ADDR, MAXCOLS+1);
    set_row_addr(oled, 0x0, MAXROWS+1);
    oled_send_cmd(oled, WRITE_RAM_CMD);

    start_write_ram(oled, START_ADDR, 0);

    uint8_t data[] = {0,0};
      for(i=0;i<MAXROWS+1;i++)
      {
        start_write_ram(oled, START_ADDR, i);
        for(j=0;j<(MAXCOLS-START_ADDR + 1);j++)
        {

            oled_write_ram(oled, j + START_ADDR, i, data, 0);

        }
      }
}

/*
    Clears display, keeps Oled_T's gddram copy
*/
void clear_display_keep_gddr(Oled_T* oled) {
    uint8_t i, j;

    set_column_addr(oled, START_ADDR, MAXCOLS + 1);
    set_row_addr(oled, 0x0, MAXROWS + 1);
    oled_send_cmd(oled, WRITE_RAM_CMD);

      for(i=0;i<MAXROWS + 1;i++)
      {
        start_write_ram(oled, START_ADDR, i);
        for(j=0;j<(MAXCOLS-START_ADDR) + 1;j++)
        {
            oled_send_data(oled, 0x00);
            oled_send_data(oled, 0x00);

        }
      }
}

/*
    Fills display with max brightness pixels, does not alter Oled_T's gddram
*/
void fill_display(Oled_T* oled) {
    uint8_t i, j;

    set_column_addr(oled, START_ADDR, MAXCOLS + 1);
    set_row_addr(oled, 0x0, MAXROWS + 1);
    oled_send_cmd(oled, WRITE_RAM_CMD);
    uint8_t data[] = {0xFF,0xFF};
      for(i=0;i<MAXROWS + 1;i++)
      {
        start_write_ram(oled, START_ADDR, i);

        for(j=0;j<(MAXCOLS-START_ADDR) + 1;j++)
        {
            oled_write_ram(oled, j + START_ADDR, i, data, 0);

        }
      }
}

/*
    Given a column number and row number, returns the index of the corresponding column
    in Oled_T's gddram copy
    Column number is not given as 0 - 63, but instead START_ADDR - MAXCOLS
*/
uint16_t get_gddr_idx(uint8_t col_num, uint8_t row_num) {
    return row_num * (MAXCOLS - START_ADDR + 1) + (col_num - START_ADDR);
}

/*
    Draws bitmap specified by data array, with width, height, and data length specified in params
    Output is either a max-brightness pixel or unlit pixel
*/
void oled_draw_bitmap_binary(Oled_T* oled, uint8_t x, uint8_t y, const uint8_t* data, const uint16_t* params) {
    uint16_t width = params[0];
    uint16_t height = params[1];

    UNUSED(height);

    uint16_t len = params[2];

    uint8_t num_pix_filled = 0; // number of pixels filled in one row of the character
    uint8_t num_pix_in_col = 0; // tracks the number of pixels in the column we are currently filled
    uint8_t col[] = {0,0}; // Data representing one column's pixel data (4 pixels, 1 nibble per pixel)
    uint8_t col_byte_idx = 0; // The byte of the column we are currently filling with data

    uint8_t nibble_mask_shift = 0x0; // How much to shift a mask to obtain pixel data from font data

    uint8_t col_start = x / 4 + START_ADDR; // the start column address
    uint8_t col_num = col_start; // the column address we are writing data to
    uint8_t x_offset = x - (x/4) * 4; // the offset the x address has from col_start
    uint8_t row_num = y; // the row address we are writing data to
    start_write_ram(oled, col_num, row_num);
    for(uint16_t i = 0; i < len; i ++) { // loop through every byte of the font character

        uint8_t font_data = data[i];
        for(uint8_t j = 0; j < 8; j++) { //loop through every bit of the font_data

            if((num_pix_in_col != 0) && (num_pix_in_col % 2) == 0) { // Once filled 2 pixels, need to fill the next byte of the column

                col_byte_idx++;
                if(col_byte_idx > 1) { // we have filled all 2 bytes (4 pixels) of a column and need to send the col data
                    oled_write_ram(oled, col_num, row_num, col, 4);
                    col_num++;
                    memset(col, 0, sizeof(col[0]) * 2);

                    col_byte_idx = 0;
                    nibble_mask_shift = 0x0;
                    num_pix_in_col = 0;

                }
            }

            if((num_pix_filled != 0) && (num_pix_filled % width) == 0) { // we have filled all the pixels in a row and need to send the col data

                //but also need to obtain gddram's previous data
                if(num_pix_in_col != 0) {
                    oled_fill_old_col_data(oled, col, num_pix_in_col, 4, col_num, row_num);
                    oled_write_ram(oled, col_num, row_num, col, num_pix_in_col);
                }
                memset(col, 0, sizeof(col[0]) * 2);

                col_byte_idx = 0;
                num_pix_filled = 0;
                nibble_mask_shift = 0x0;
                row_num++; // move to next row
                col_num = col_start; // move back to the column start
                num_pix_in_col = 0;
                start_write_ram(oled, col_num, row_num);
                break;
            }

            if(x_offset != 0 && num_pix_in_col == 0 && col_num == col_start) { // if we have a non-zero x-offset, we need to copy over exisiting pixel data in the column
                                                                               // and combine with the new font_data

                oled_fill_old_col_data(oled, col, 0, x_offset, col_num, row_num);

                num_pix_in_col = x_offset;
                col_byte_idx = (x_offset/2);
                nibble_mask_shift = (x_offset % 2 == 0) ? 0 : 4;
            }

            uint8_t bit = (font_data & 0x80); // get the first bit
            uint8_t pix = (bit != 0) ? (0xF0 >> nibble_mask_shift) : (0x0); // if bit = 0; pix data is 0x0000, otherwise pixel data is either 0xF0 or 0x0F

            col[col_byte_idx] |= pix; // fill the approriate bits in the column with new pixel data

            nibble_mask_shift = (nibble_mask_shift == 0x0) ? 0x04 : 0x0;

            font_data <<= 1;

            num_pix_filled++;
            num_pix_in_col++;
        }
    }

}

/*
    Draws a greyscale bitmap from data, with width height and length specified in params
    Each byte of data represents the greyscale brigthness of 2 pixels
*/
void oled_draw_bitmap_greyscale(Oled_T* oled, uint8_t x, uint8_t y, const uint8_t* data, const uint16_t* params) {
    uint16_t width = params[0];

    uint16_t height = params[1];
    UNUSED(height);

    uint16_t len = params[2];

    uint8_t num_pix_in_col = 0;
    uint8_t num_pix_in_row = 0;

    uint8_t col_start = x / 4 + START_ADDR; // the start column address
    uint8_t col_num = col_start; // the column address we are writing data to
    uint8_t x_offset = x - (x/4) * 4; // the offset the x address has from col_start
    uint8_t row_num = y; // the row address we are writing data to

    uint8_t col[] = {0,0}; // Data representing one column's pixel data (4 pixels, 1 nibble per pixel)
    uint8_t col_idx = 0;
    uint8_t nibble_mask_shift = 0;

    start_write_ram(oled, col_num, row_num);

    for(int i = 0; i < len * 2; i ++) {
        if(num_pix_in_col == 4) {
            oled_write_ram(oled, col_num, row_num, col, 4);
            col_num++;
            memset(col, 0, sizeof(col[0]) * 2);
            num_pix_in_col = 0;
            col_idx = 0;
        }
        if(num_pix_in_row == width) { // need to handle case where num_pix_in_row is one away from width
            if(num_pix_in_col != 0) {
                oled_fill_old_col_data(oled, col, num_pix_in_col, 4, col_num, row_num);
                oled_write_ram(oled, col_num, row_num, col, 4);
            }
            memset(col, 0, sizeof(col[0]) * 2);
            row_num++;
            col_num = col_start;
            num_pix_in_row = 0;
            num_pix_in_col = 0;
            col_idx = 0;
            nibble_mask_shift = 0;
            start_write_ram(oled, col_num, row_num);
        }

        if((col_num == col_start) && (num_pix_in_col == 0) && (x_offset != 0)) {
            oled_fill_old_col_data(oled, col, 0, x_offset, col_num, row_num);
            num_pix_in_col = x_offset;
            col_idx = (x_offset/2);
            nibble_mask_shift = (x_offset % 2 == 0) ? 0 : 4;
        }

        uint8_t pix_data = (i % 2 == 0) ? data[i/2] : data[i/2] << 4;
        col[col_idx] |= (pix_data & 0xF0) >> nibble_mask_shift;
        num_pix_in_col++;
        num_pix_in_row++;
        col_idx = (num_pix_in_col > 1) ? 1 : 0;
        nibble_mask_shift = (nibble_mask_shift == 0) ? 4 : 0;
    }

}

/*
    Draws MY18 racecar
*/
void oled_draw_racecar_bmp(Oled_T* oled, uint8_t x, uint8_t y) {
    oled_draw_bitmap_binary(oled, x, y, racecar_bmp, racecar_bmp_params);
}

/*
    Draws MY18 racecar
*/
void oled_draw_racecar_bmp_greyscale(Oled_T* oled, uint8_t x, uint8_t y) {
    oled_draw_bitmap_greyscale(oled, x, y, racecar_bmp_greyscale, racecar_bmp_greyscale_params);
}

/*
    Sends a chracter specified at font_idx, the index of the font data in the font descriptors array
*/
uint8_t oled_send_char(Oled_T* oled, uint8_t x, uint8_t y, uint16_t font_idx) {

    uint16_t char_start_addr = powerGreen_12ptDescriptors[font_idx][2]; //where the data for a character starts at
    uint16_t char_end_addr = powerGreen_12ptDescriptors[font_idx + 1][2]; //where the data for a character ends
    uint8_t width = powerGreen_12ptDescriptors[font_idx][0]; //width of a character in pixels
    uint8_t height = powerGreen_12ptDescriptors[font_idx][1]; //height of a character in pixels

    if(font_idx == 0) {
        return x + width; // font_idx = 0 corrsponds to a space character, in which case, we send nothing and return a shifted x coordinate
    }

    uint8_t data[char_end_addr-char_start_addr];
    for(int i = 0; i < char_end_addr-char_start_addr; i ++) {
        data[i] = powerGreen_12ptBitmaps[i + char_start_addr];
    }
    uint16_t params[] = {width, height, char_end_addr - char_start_addr};
    oled_draw_bitmap_binary(oled, x, y, data, params);

    return x + width + 1; //return the next x

}

/*
    Sends every character
*/
void oled_test_all_chars(Oled_T* oled) {

    uint8_t x = 0;
    uint8_t y = 5;

    for(int font_idx = 0; font_idx < 93; font_idx++) {

        if(x >= 240) {
            x = 0;
            y = y + 15;
        }

        x = oled_send_char(oled, x, y, font_idx);

    }

}

/*
    Rewrites the entire Oled_T's gddram copy to the display
*/
void oled_rewrite_gddram(Oled_T* oled) {

    set_column_addr(oled, START_ADDR, MAXCOLS);
    set_row_addr(oled, 0x0, MAXROWS);
    oled_send_cmd(oled, WRITE_RAM_CMD);

    for(int i = 0; i < ((MAXCOLS - START_ADDR + 1)* (MAXROWS + 1)); i++) {

        uint8_t col_num = i % (MAXCOLS-START_ADDR + 1) + START_ADDR;
        uint8_t row_num = (i - col_num) / (MAXCOLS-START_ADDR + 1);

        set_column_addr(oled, col_num, MAXCOLS);
        set_row_addr(oled, row_num, MAXROWS);

        oled_send_cmd(oled, WRITE_RAM_CMD);


        oled_send_data(oled, oled->gddr[i].data[0]);
        oled_send_data(oled, oled->gddr[i].data[1]);
        col_num++;
    }

}

/*
    Writes a string to display
*/
void oled_draw_string(Oled_T* oled, uint8_t x, uint8_t y, char* str, bool wrap) {
    unsigned char c = str[0];
    int i = 0;

    while(c != '\0') {
        // uart_transmit_ln("sending ... ");
        c = str[i];

        uint8_t font_idx = c - ' ';
        uint8_t width = (c == '\0') ? 3 : powerGreen_12ptDescriptors[font_idx][0];
        if(wrap && (x + width > 253)) {
            x = 0;
            y = y + 15;
        }

        if(y >= MAXROWS) {
            break;
        }

        x = oled_send_char(oled, x, y, font_idx);

        i++;
    }

    oled->last_x = x;
    oled->last_y = y;
}

/*
    Writes an integer to display
*/
void oled_draw_int(Oled_T* oled, uint8_t x, uint8_t y, int32_t num, bool wrap, uint8_t base) {
    UNUSED(base);
  sprintf(numBuffer,"%ld", num);
  oled_draw_string(oled, x, y, numBuffer, wrap);
}

/*
    Fills an area from x1 to x2, y1 to y2, of a specified brightness
*/
void oled_fill(Oled_T* oled, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t brightness) {
    //0 1 2 3 4 5 6 7 8 9 10 11
    uint8_t x1_offset = x1 - (x1/4) * 4; // the offset the x address has from start column
    uint8_t x2_offset = ((x2/4 + 1) * 4 - 1) - x2; // x2's with no offset are when you end at 3, 7, etc. Offsets are those that are less than these numbers
    x2_offset = (x2_offset == 0) ? 0 : (4 - x2_offset);
    uint8_t col_start = x1/4 + START_ADDR;

    uint8_t col_data[] = {0,0};

    uint8_t pix_in_col = 0;
    uint8_t col = col_start;
    uint8_t row = y1;
    uint8_t col_byte_idx = 0;
    uint8_t nibble_mask_shift = 0;

    uint16_t y;
    uint16_t x;

    start_write_ram(oled, col, row);

    for(y = y1; y <= y2; y++) {
        for(x = x1; x <= x2; x++) {
            if(pix_in_col == 4) {
                oled_write_ram(oled, col, row, col_data, 4);
                pix_in_col = 0;
                col_data[0] = 0;
                col_data[1] = 0;
                col++;
            }
            col_byte_idx = pix_in_col / 2;
            if((x == x1 && x1_offset != 0)) {
                oled_fill_old_col_data(oled, col_data, 0, x1_offset, col, row);
                pix_in_col = x1_offset;
                col_byte_idx = (x1_offset/2);
                nibble_mask_shift = (x1_offset % 2 == 0) ? 0 : 4;
            }

            col_data[col_byte_idx] |= (brightness & 0xF0) >> nibble_mask_shift;
            nibble_mask_shift = (nibble_mask_shift == 0) ? 4 : 0;
            pix_in_col++;
        }

        if (pix_in_col != 0) {
            oled_fill_old_col_data(oled, col_data, pix_in_col, 4, col, row);
            oled_write_ram(oled, col, row, col_data, 4);
        }
        pix_in_col = 0;
        col = col_start;
        col_data[0] = 0;
        col_data[1] = 0;
        nibble_mask_shift = 0;
        row++;
        start_write_ram(oled, col, row);
    }
}

/*
    Draws a horizontal line
*/
void oled_draw_hline(Oled_T* oled, uint8_t x, uint8_t y, uint8_t len, uint8_t brightness) {
    oled_fill(oled, x, y, x + len - 1, y, brightness);
}

/*
    Draws a vertical line
*/
void oled_draw_vline(Oled_T* oled, uint8_t x, uint8_t y, uint8_t len, uint8_t brightness) {
    oled_fill(oled, x, y, x, y + len - 1, brightness);
}

/*
    Draws box outline
*/
void oled_draw_box_outline(Oled_T* oled, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t brightness) {
    oled_draw_hline(oled, x, y, w, brightness);
    oled_draw_vline(oled, x, y, h, brightness);
    oled_draw_vline(oled, x + w - 1, y, h, brightness);
    oled_draw_hline(oled, x, y + h - 1, w, brightness);
}

/*
    Displays rectangles of every possible brightness
*/
void oled_test_brightness(Oled_T* oled) {
    uint8_t data[2];
    uint8_t col_num = START_ADDR;
    uint8_t row_num = 0;

    start_write_ram(oled, col_num, row_num);

    for(uint8_t i = 0; i < 16; i++) {
        uint8_t col_val = (i<<4) | (i);

        data[0] = col_val;
        data[1] = col_val;

        for(int k = 0; k < 2; k++) {
            row_num = 0;
            for(int j = 0; j < 45; j++) {
                oled_write_ram(oled, col_num, row_num, data, 4);
                row_num++;
                start_write_ram(oled, col_num, row_num);
            }
            col_num++;
        }
    }
}
