#include "display_pages.h"

template <class T>
Label_CAN_Fault_Widget<T>::Label_CAN_Fault_Widget(GHandle ugfx_widget, T* current_can_val) {
    this->current_can_val = current_can_val;
    this->ugfx_widget = ugfx_widget;
}

template <class T>
void Label_CAN_Fault_Widget<T>::update(bool force_update) {

    if(force_update || this->old_can_val != *this->current_can_val) {
        set_error(static_cast<uint8_t>(*this->current_can_val));
        this->old_can_val = *this->current_can_val;
    }
}

template <class T>
void Label_CAN_Fault_Widget<T>::set_visibility(gBool visible) {
    gwinSetVisible(this->ugfx_widget, visible);
}

template <class T>
void Label_CAN_Fault_Widget<T>::set_error(uint8_t error_state) {
    switch (error_state) {
        case 0:
            gwinSetText(this->ugfx_widget,  "", TRUE);
            break;
        case 1:
            gwinSetText(this->ugfx_widget,  "*", TRUE);
            break;
        case 2:
            gwinSetText(this->ugfx_widget,  "-", TRUE);
            break;
        default:
            break;
    }
}
