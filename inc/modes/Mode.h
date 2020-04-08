#ifndef __MODE_H__
#define __MODE_H__
#include "page.h"

typedef enum {
    ROMAN_GENERAL,
    ROMAN_DEBUG,
} Mode_Name;

class Mode {
public:
    Mode(Page**, uint8_t, Mode_Name);
    void update();
    void process_buttons(B_State far_right_button, B_State far_left_button,
                        B_State wheel_1_button, B_State wheel_2_button, B_State wheel_3_button,
                        B_State wheel_4_button, B_State wheel_5_button, B_State wheel_6_button);
    
    void set_visibility(gBool visible);
    void hide_all_pages();
    void set_current_page(Page* page);
    void page();
private:
    Page** pages;
    Page* current_page;
    Page* new_page;
    Mode_Name mode_name;
    void increment_page();
    uint8_t current_page_idx;
    uint8_t num_pages;
};

#endif // __MODE_H__