#ifndef __UGFX_WIDGET_UTILS_H_
#define __UGFX_WIDGET_UTILS_H_

#include "common.h"
#include "gfx.h"
#include "display_timer.h"

typedef enum {
	GROUP_1,
	GROUP_2,
} Radio_Group;

#ifdef __cplusplus
extern "C" {
#endif

/*
	Create a widget that can be used for all components to specify x, y, width, height
*/
void create_widget(uint8_t width, uint8_t height, uint8_t x, uint8_t y);

void create_label(GHandle* gh, uint8_t width, uint8_t height, uint8_t x, uint8_t y);
/*
	Create a label.
	attr specifies the label text that precedes the label's value
	attr : value
*/
void create_label_with_attr(GHandle* gh, const char* attr, uint8_t width, uint8_t height, uint8_t x, uint8_t y, uint8_t tab_width);

// currently this is slower than Dot_Matrix_NHD_US25664.c's draw bitmap
void create_image(GHandle* gh, const char* file_name, uint8_t width, uint8_t height, uint8_t x, uint8_t y);

void create_radio_button(GHandle* gh, char* label, Radio_Group group, uint8_t width, uint8_t height, uint8_t x, uint8_t y);

void create_slider(GHandle* gh, char* label, uint8_t lower_bound, uint8_t upper_bound, uint8_t width, uint8_t height, uint8_t x, uint8_t y);

void set_label_text_num(GHandle label, int32_t num);

void set_error(GHandle gh, uint8_t error_state);

#ifdef __cplusplus
}
#endif

#endif // __UGFX_WIDGET_UTILS_H_

