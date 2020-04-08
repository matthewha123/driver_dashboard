#include "display_pages.h"
#include <stdio.h>

template <class T>
Label_CAN_Num_Widget<T>::Label_CAN_Num_Widget(GHandle ugfx_widget, T* current_can_val) {
    this->current_can_val = current_can_val;
    this->ugfx_widget = ugfx_widget;
}

template <class T>
void Label_CAN_Num_Widget<T>::update(bool force_update) {

    if(force_update || this->old_can_val != *this->current_can_val) {
        sprintf(this->num_buffer,(sizeof(*this->current_can_val) > 2) ? "%ld" : "%d", *this->current_can_val);
        gwinSetText(this->ugfx_widget, this->num_buffer, TRUE);
        this->old_can_val = *this->current_can_val;
    }
}
template <class T>
void Label_CAN_Num_Widget<T>::set_visibility(gBool visible) {
    gwinSetVisible(this->ugfx_widget, visible);
}

