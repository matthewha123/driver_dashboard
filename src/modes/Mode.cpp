#include "Mode.h"
#include <stdio.h>

Mode::Mode(Page** pages, uint8_t num_pages, Mode_Name mode_name) {
    this->pages = new Page*[num_pages];
    for(uint8_t page_idx = 0; page_idx < num_pages; page_idx++) {
        this->pages[page_idx] = pages[page_idx];
    }
    this->num_pages = num_pages;
    printf("Num pages: %d\r\n", num_pages);
    this->mode_name = mode_name;
    this->current_page_idx = 0;
    this->current_page = this->pages[this->current_page_idx];
    this->new_page = this->current_page;
}

// if theres any special behaviour that needs to be implemented
// that doesn't fit nicely into the page's code
// can implement that logic by switching on the page's page_name
void Mode::update() {
    this->current_page->update(false);
}

void Mode::page() {
    // if paging is required
    if(this->new_page != this->current_page) {
        this->current_page = this->new_page;
        gdispClear(Black);
        this->hide_all_pages();
        this->current_page->set_visibility(TRUE);
        this->current_page->update(true);
    }

}

void Mode::hide_all_pages() {
    for(uint8_t page_idx = 0; page_idx < num_pages; page_idx++) {
        Page* page = this->pages[page_idx];
        page->set_visibility(FALSE);
    }
}

void Mode::process_buttons(B_State far_right_button, B_State far_left_button,
                        B_State wheel_1_button, B_State wheel_2_button, B_State wheel_3_button,
                        B_State wheel_4_button, B_State wheel_5_button, B_State wheel_6_button) {

    switch(this->current_page->page_name) {
#ifdef TUNING_POW_LIM
        case POW_LIM_CONTROLS:
#elif defined TUNING_RG_TL_VL
        case SECOND_CONTROLS:
#endif
#ifdef TUNING_LC
        case LAUNCH_CONTROL:
#endif
        case ROMAN_GENERAL_MAIN:
            if(this->current_page->get_has_paged_past()) {
                if(far_left_button == SHORT_PRESS || far_left_button == LONG_PRESS) {
                    this->increment_page();
                    this->current_page->reset_has_paged_past();
                }
            } else {
                this->current_page->modify_page(far_right_button, far_left_button, 
                                        wheel_1_button, wheel_2_button, wheel_3_button,
                                        wheel_4_button, wheel_5_button, wheel_6_button);
            }
            break;
        default:
            if(far_left_button == SHORT_PRESS || far_left_button == LONG_PRESS) {
                this->increment_page();
            }
            this->current_page->modify_page(far_right_button, far_left_button, 
                                        wheel_1_button, wheel_2_button, wheel_3_button,
                                        wheel_4_button, wheel_5_button, wheel_6_button);
            break;
    }
}

void Mode::increment_page() {
    this->current_page_idx = (this->current_page_idx + 1) % this->num_pages;
    this->new_page = this->pages[this->current_page_idx];
}

void Mode::set_visibility(gBool visible) {
    this->current_page->set_visibility(visible);
    if(visible == TRUE) {
        this->current_page->update(true);
    }
}

void Mode::set_current_page(Page* page) {
    this->current_page = page;
    this->new_page = this->current_page;
    for(uint8_t page_idx = 0; page_idx < this->num_pages; page_idx++) {
        if(this->pages[page_idx] == page) {
            this->current_page_idx = page_idx;
            break;
        }
    }
    this->set_visibility(TRUE);
}
