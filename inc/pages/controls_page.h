#ifndef __CONTROLS_PAGE_H___
#define __CONROLS_PAGE_H___
#include "page.h"
#include "timer.h"

class Controls_Page: public Page {
public:
    Controls_Page(Widget** _widgets, int num_widgets, Page_Name page_name);
    Controls_Page(Widget** _widgets, int num_widgets, Page_Name page_name, uint8_t start_widget_idx, uint8_t end_widget_idx, uint8_t max_widgets_per_page);
    void update(bool force_update);
    void set_visibility(gBool visible);
    void modify_page(B_State far_right_button, B_State far_left_button,
                        B_State wheel_1_button, B_State wheel_2_button, B_State wheel_3_button,
                        B_State wheel_4_button, B_State wheel_5_button, B_State wheel_6_button);
    int num_widgets;
    Widget** widgets;
    bool has_paged_past;
    bool get_has_paged_past();
    void reset_has_paged_past();
private:
    void select_parameter(uint8_t param_idx);
    void deselect_parameter(uint8_t param_idx);
    void update_selected_parameter(bool force_update);
    void modify_selected_parameter(B_State inc_button);
    uint8_t old_selected_param;
    uint8_t max_widgets_per_page;
    uint8_t selected_param;
    uint8_t start_widget_idx;
    uint8_t end_widget_idx;
};

#endif // __PAGE_H_

