#include "display_pages.h"

constexpr const char* state_enum_to_string[] = {
    "STICKY_ROOT",
    "ROOT",
    "LV",
    "DRIVER_RESET_LOW",
    "PRECOOLING",
    "PRECHARGE",
    "RTD",
    "ZERO_TORQUE",
    "DRIVING",
    "AUTOM"
};

const char *name_from_state[] = {

};

Label_CAN_VCU_State_Widget::Label_CAN_VCU_State_Widget(GHandle ugfx_widget, CANlib_critical_VCUState_state_T* current_can_val) {
    this->current_can_val = current_can_val;
    this->ugfx_widget = ugfx_widget;
}

void Label_CAN_VCU_State_Widget::update(bool force_update) {
    if(force_update || this->old_can_val != *this->current_can_val) {
        set_state(*this->current_can_val);
        this->old_can_val = *this->current_can_val;
    }
}
void Label_CAN_VCU_State_Widget::set_visibility(gBool visible) {
    gwinSetVisible(this->ugfx_widget, visible);
}
void Label_CAN_VCU_State_Widget::set_state(CANlib_critical_VCUState_state_T state) {

    if(state == CANlib_critical_VCUState_state_NUM_FIELDS) {
        return;
    }

    gwinSetText(this->ugfx_widget, state_enum_to_string[state], TRUE);
}

