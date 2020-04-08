#ifndef __DISPLAY_PAGES_H_
#define __DISPLAY_PAGES_H_

#include "common.h"
#include "button.h"
#include "Dot_Matrix_NHD_US25664.h"
#include "gfx.h"
#include "can.h"
#include "romfs_files.h"
#include "display_timer.h"
#include "ugfx_widget_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

void gui_init(void);
void gui_update(void);
void create_image(GHandle* gh, const char* file_name, uint8_t width, uint8_t height, uint8_t x, uint8_t y);
void interrupt_process_buttons(B_State far_right_btn_val, B_State far_left_btn_val,
                        B_State wheel_1_button, B_State wheel_2_button, B_State wheel_3_button,
                        B_State wheel_4_button, B_State wheel_5_button, B_State wheel_6_button);
    
void process_controls_updates(B_State far_right_button, B_State far_left_button);
bool has_finished_gui_init(void);

#define UPDATE_WIDGET(widget, old_val, can_val, update_function) {\
        if(force_update || old_val != can_val) { \
            update_function(widget, can_val); \
            old_val = can_val; \
        }\
    }\

typedef struct radio{
    char* radio_text;
    Radio_Group group;
} Radio_T;

#ifdef __cplusplus
}
#endif

#endif //__DISPLAY_PAGES_H_

