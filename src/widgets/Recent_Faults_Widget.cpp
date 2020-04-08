#include "Recent_Faults_Widget.h"

constexpr const char* fault_enum_to_string[] = {
    "BMS",
    "IMD",
    "CNTCR",
    "BSPD",
    "THROT",
    "SENSOR",
    "BRAKE",
    "TSMS",
    "SHUT_D",
    "HVLV",
    "HB",
    "LEAK_DETECT",
    "*"
};

Recent_Faults_Widget::Recent_Faults_Widget(GHandle* fault_labels, uint8_t num_fault_labels, CANlib_critical_VCUDashInfo_T* faults) {
    this->fault_labels = new Fault_Label[num_fault_labels];
    for(int i = 0; i < num_fault_labels; i++) {
        this->fault_labels[i].ugfx_widget = fault_labels[i];
        this->fault_labels[i].name = NO_FAULT;
    }
    this->num_fault_labels = num_fault_labels;
    this->faults = faults;
    this->current_fault_idx = 0;
    this->update(true);
}

void Recent_Faults_Widget::clear_fault(uint8_t fault_idx) {
    Fault_Label* fault_label = &(this->fault_labels[fault_idx]);
    fault_label->name = NO_FAULT;
    gwinSetText(fault_label->ugfx_widget, "*", TRUE);

}

void Recent_Faults_Widget::set_visibility(gBool visible) {
    for(int i = 0; i < num_fault_labels; i++) {
        gwinSetVisible(this->fault_labels[i].ugfx_widget, visible);
    }
}

void Recent_Faults_Widget::update(bool force_update) {

    this->current_fault_idx = 0;
    // High Priority
            // BMS, IMD, CNTCT, BSPD
    this->update_fault_label_inc(BMS, this->faults->bms, force_update);
    this->update_fault_label_inc(IMD, this->faults->imd, force_update);
    this->update_fault_label_inc(CONTACTOR_MISMATCH, this->faults->contactor_mismatch, force_update);
    this->update_fault_label_inc(BSPD, this->faults->bspd, force_update);

    // MED Priority
            // THROTTLE, SENSOR, BRAKE
    this->update_fault_label_inc(THROTTLE_IMPLAUSIBILITY, this->faults->throttle_implausibility, force_update);
    this->update_fault_label_inc(SENSOR_IMPLAUSIBILITY, this->faults->sensor_implausibility, force_update);
    this->update_fault_label_inc(BRAKE_THROTTLE_CONFLICT, this->faults->brake_throttle_conflict, force_update);

    // LOW Priority
            //TSMS, SHUTDOWN, HVLV, HB
    this->update_fault_label_inc(TSMS, this->faults->tsms, force_update);
    this->update_fault_label_inc(SHUTDOWN_LOOP_LATCH, this->faults->shutdown_loop_latch, force_update);
    this->update_fault_label_inc(HVLV_INTERLOCK, this->faults->hvlv_interlock, force_update);
    this->update_fault_label_inc(HEARTBEAT, this->faults->heartbeat, force_update);
    this->update_fault_label_inc(LEAK_DETECT, this->faults->leak_detect, force_update);
    // clear old fault labels
    this->clear_old_fault_labels(force_update);
}


void Recent_Faults_Widget::update_fault_label(Fault_Name name, uint8_t idx, bool force_update) {

    if(idx < this->num_fault_labels) {
        Fault_Label* fault_label = &(this->fault_labels[idx]);
        if(fault_label->name != name || force_update) {
            // update
            fault_label->name = name;
            gwinSetText(fault_label->ugfx_widget, fault_enum_to_string[name], TRUE);
        }
    }
}

// Update the fault label and increment the current_fault_idx
void Recent_Faults_Widget::update_fault_label_inc(Fault_Name name, uint8_t error_value, bool force_update) {
    if(current_fault_idx < this->num_fault_labels && error_value == 1) { // 1 is Error_Detected
        this->update_fault_label(name, this->current_fault_idx, force_update);
        this->current_fault_idx++;
    }
}

void Recent_Faults_Widget::clear_old_fault_labels(bool force_update) {
    for(int i = this->current_fault_idx; i < this->num_fault_labels; i++) {
        this->update_fault_label_inc(NO_FAULT, 1, force_update);
    }
}

