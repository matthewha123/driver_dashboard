#include "controls_page.h"
#include "stdio.h"

Controls_Page::Controls_Page(Widget **_widgets, int num_widgets, Page_Name page_name) {
  this->widgets = new Widget *[num_widgets];
  for (int i = 0; i < num_widgets; i++) {
    this->widgets[i] = _widgets[i];
  }
  this->num_widgets          = num_widgets;
  this->selected_param       = 0;
  this->max_widgets_per_page = 9;
  this->select_parameter(this->selected_param);
  this->old_selected_param = this->selected_param;
  this->has_paged_past     = false;
  this->page_name          = page_name;
  this->start_widget_idx   = (this->selected_param / (this->max_widgets_per_page - 1)) * (this->max_widgets_per_page - 1);
  this->end_widget_idx     = ((this->start_widget_idx + this->max_widgets_per_page - 1) > this->num_widgets) ? this->num_widgets : this->start_widget_idx + this->max_widgets_per_page - 1;
}

Controls_Page::Controls_Page(Widget **_widgets, int num_widgets, Page_Name page_name, uint8_t start_widget_idx, uint8_t end_widget_idx, uint8_t max_widgets_per_page) {
  this->widgets = new Widget *[num_widgets];
  for (int i = 0; i < num_widgets; i++) {
    this->widgets[i] = _widgets[i];
  }
  this->num_widgets          = num_widgets;
  this->selected_param       = start_widget_idx;
  this->max_widgets_per_page = max_widgets_per_page;
  this->select_parameter(this->selected_param);
  this->old_selected_param = this->selected_param;
  this->has_paged_past     = false;
  this->page_name          = page_name;
  this->start_widget_idx   = start_widget_idx;
  this->end_widget_idx     = end_widget_idx;
}

void Controls_Page::update(bool force_update) {
  if (this->old_selected_param != this->selected_param) {
    this->deselect_parameter(this->old_selected_param);
    this->old_selected_param = this->selected_param;
  }
  if ((this->selected_param % (this->max_widgets_per_page - 1)) == 0) {
    this->set_visibility(TRUE);
  }
  this->select_parameter(this->selected_param);
  // this->update_selected_parameter(b1, b2, force_update);
  for (int i = 0; i < this->num_widgets; i++) {
    this->widgets[i]->update(force_update);
  }
}

void Controls_Page::set_visibility(gBool visible) {
  for (int i = 0; i < this->num_widgets; i++) {
    this->widgets[i]->set_visibility(FALSE);
  }

  uint8_t first_widget = (this->selected_param / (this->max_widgets_per_page - 1)) * (this->max_widgets_per_page - 1);
  uint8_t last_widget  = ((this->start_widget_idx + this->max_widgets_per_page - 1) > this->num_widgets) ? this->num_widgets : this->start_widget_idx + this->max_widgets_per_page - 1;

  for (int i = first_widget; i < last_widget; i++) {
    this->widgets[i]->set_visibility(visible);
  }
}

void Controls_Page::select_parameter(uint8_t param_idx) {
  this->widgets[param_idx]->set_selected(TRUE);
}

void Controls_Page::deselect_parameter(uint8_t param_idx) {
  this->widgets[param_idx]->set_selected(FALSE);
}

void Controls_Page::update_selected_parameter(bool force_update) {
  this->widgets[this->selected_param]->update(force_update);
}

void Controls_Page::modify_page(B_State far_right_button,
                                B_State far_left_button,
                                B_State wheel_1_button,
                                B_State wheel_2_button,
                                B_State wheel_3_button,
                                B_State wheel_4_button,
                                B_State wheel_5_button,
                                B_State wheel_6_button) {
  UNUSED(wheel_1_button);
  UNUSED(wheel_2_button);
  UNUSED(wheel_3_button);
  UNUSED(wheel_4_button);
  UNUSED(wheel_5_button);
  UNUSED(wheel_6_button);

  B_State select_button = far_left_button;
  B_State inc_button    = far_right_button;
  if (select_button == SHORT_PRESS || select_button == LONG_PRESS) {
    // UNCOMMENT THIS WHEN HAVE MORE BUTTONS
    // this->selected_param = (this->selected_param + 1) % this->num_widgets;
    this->selected_param++;
    if (this->selected_param >= this->end_widget_idx) {
      this->selected_param = this->start_widget_idx;
      this->has_paged_past = true;
    }
  }
  this->modify_selected_parameter(inc_button);
}

void Controls_Page::modify_selected_parameter(B_State inc_button) {
  this->widgets[this->selected_param]->modify_param(inc_button);
}

bool Controls_Page::get_has_paged_past() {

  return this->has_paged_past;
}

void Controls_Page::reset_has_paged_past() {
  this->has_paged_past = false;
}
