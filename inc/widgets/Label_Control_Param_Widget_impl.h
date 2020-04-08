#include "display_pages.h"

template <class T>
Label_Control_Param_Widget<T>::Label_Control_Param_Widget(GHandle ugfx_widget, const char* control_param_name, T* values, uint16_t num_values, T* current_val, uint8_t label_tab) {
    this->current_val = current_val;
    
    this->set_control_param_name(control_param_name);
    this->values = values;
    this->num_values = num_values;
    this->ugfx_widget = ugfx_widget;
    this->label_tab = label_tab;
    this->curr_val_idx = 0;
    *current_val = this->values[this->curr_val_idx];
    this->update(true); // force an update to redraw wth initialized values
}

template <class T>
void Label_Control_Param_Widget<T>::update(bool force_update) {
    if(force_update || this->old_val != *this->current_val) {
        sprintf(this->num_buffer,(sizeof(*this->current_val) > 2) ? "%ld" : "%d", *this->current_val);
        gwinSetText(this->ugfx_widget, this->num_buffer, TRUE);
        this->old_val = *this->current_val;
    }
}

template <class T>
void Label_Control_Param_Widget<T>::modify_param(B_State inc_button) {

    B_State curr_inc_button_state = inc_button;

    if(curr_inc_button_state == SHORT_PRESS || curr_inc_button_state == LONG_PRESS) {
        this->curr_val_idx = (this->curr_val_idx + 1) % this->num_values;
    }
    *(this->current_val) = this->values[this->curr_val_idx];
}

template <class T>
void Label_Control_Param_Widget<T>::set_selected(gBool is_selected) {
    if(is_selected) {
        gwinLabelSetAttribute(this->ugfx_widget, this->label_tab, this->control_param_name);
    } else {
        gwinLabelSetAttribute(this->ugfx_widget, this->label_tab, this->control_param_name + 1);
    }
}

template <class T>
void Label_Control_Param_Widget<T>::set_visibility(gBool visible) {
    gwinSetVisible(this->ugfx_widget, visible);
}

template <class T>
void Label_Control_Param_Widget<T>::set_control_param_name(const char* control_param_name) {
    uint8_t idx = 0;
    this->control_param_name[0] = '*'; // reserve this spot for asterisk indication of label selection
    while(true) {
        char c = control_param_name[idx];
        this->control_param_name[idx + 1] = c;
        if(c == '\0' || (idx + 1) == 24) {
            break;
        }
        idx++;
    }
    return;
}
