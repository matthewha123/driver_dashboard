#ifndef _DOT_MATRIX_NHD_US25664_H_
#define _DOT_MATRIX_NHD_US25664_H_

#include "common.h"
#include "gpio.h"
#include "pins.h"
#include "uart.h"
#include "powerGreen_12pt_tight_h_tight_w.h"
#include "bitmaps.h"
#include "spi.h"

#ifdef __cplusplus
extern "C" {
#endif

// COMMANDS
#define ENABLE_GRAY_SCALE_TABLE 0x0
#define SET_COL_ADDR 0x15
#define WRITE_RAM_CMD 0x5C
#define READ_RAM_CMD  0x5D
#define SET_ROW_ADDR 0x75
#define SET_REMAP_DUAL_COM 0xA0
#define SET_DISPLAY_BLACK 0xA4
#define SET_DISPLAY_WHITE 0xA5
#define SET_DISPLAY_NORMAL 0xA6
#define SET_DISPLAY_INV   0xA7
#define DISPLAY_ON 0xAE
#define DISPLAY_OFF 0xAF
#define SET_PHASE_LENGTH 0xB1
#define SET_FRONT_CLK_DIV 0xB3
#define ENHANCE_A 0xB4
#define SET_GPIO 0xB5
#define SET_SECOND_PRE_CHARGE_PERIOD 0xB6
#define SET_GRAY_SCALE_TABLE 0xB8h
#define SELECT_DEF_LIN_GRAY_SCALE_TABLE 0x89
#define SET_VCOMH 0xBE
#define SET_CONTRAST_CURRENT 0xC1
#define MASTER_CURR_CTRL 0xC7
#define SET_MULTIPLEX_RATIO 0xCA
#define ENHANCE_B 0xD1
#define SET_CMD_LOCK 0xFD
#define SET_DISP_OFFSET 0xA2
#define SET_DISP_START_LN 0xA1
#define SET_FUNC_SEL 0xAB
#define SET_DEF_LN_GRAYSCALE_TBL 0xB9
#define SET_PRE_CHARGE_VOLTAGE  0xBB
#define EXIT_PARTIAL_DISP 0xA9
#define SET_SLEEP_MODE_ON 0xAE
#define SET_SLEEP_MODE_OFF 0xAF

#define MAXROWS 0x3F
#define MAXCOLS 0x5B
#define START_ADDR 0x1C

typedef struct gddram {
    uint8_t data[2];
    uint8_t last_x;
} gddram_T;

typedef struct oled {
    gddram_T gddr[(MAXCOLS - START_ADDR + 1) * (MAXROWS+1)];
    uint8_t last_x;
    uint8_t last_y;
} Oled_T;

extern Oled_T ugfx_oled;
extern bool debug_pixel_print;

void initialize_oled(Oled_T* oled);
void start_display(Oled_T* oled);

void oled_send_cmd(Oled_T* oled, uint8_t cmd);
void oled_send_data(Oled_T* oled, uint8_t data);

// Clearing/Filling display
void clear_display(Oled_T* oled);
void clear_display_keep_gddr(Oled_T* oled);
void fill_display(Oled_T* oled);
void oled_rewrite_gddram(Oled_T* oled);

// Converting row/col num to gddram copy index
uint16_t get_gddr_idx(uint8_t col_num, uint8_t row_num);

// String and Number Printing Functions
uint8_t oled_send_char(Oled_T* oled, uint8_t x, uint8_t y, uint16_t font_idx);
void oled_test_all_chars(Oled_T* oled);
void oled_draw_string(Oled_T* oled, uint8_t x, uint8_t y, char* str, bool wrap);
void oled_draw_int(Oled_T* oled, uint8_t x, uint8_t y, int32_t num, bool wrap, uint8_t base);
void oled_draw_racecar_bmp_greyscale(Oled_T* oled, uint8_t x, uint8_t y);
void oled_fill(Oled_T* oled, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t brightness);


#ifdef __cplusplus
}
#endif
#endif //_DOT_MATRIX_NHD_US25664_H_

