#ifndef __PAGE_H_
#define __PAGE_H_
#include "widget.h"

typedef enum {
  FAULTS,
  BATTERY,
  MOTOR,
#ifdef TUNING_POW_LIM
  POW_LIM_CONTROLS,
#endif // TUNING_POW_LIM
#ifdef TUNING_RG_TL_VL
  SECOND_CONTROLS,
#endif // TUNING_RG_TL_VL
#ifdef TUNING_LC
  LAUNCH_CONTROL,
#endif // TUNING_LC
  ROMAN_GENERAL_MAIN,
  NUM_ROMAN_GENERAL_PAGES,
} Page_Name;

class Page {
public:
  virtual void update(bool force_update);
  virtual void set_visibility(gBool visible);
  virtual void modify_page(B_State far_right_button,
                           B_State far_left_button,
                           B_State wheel_1_button,
                           B_State wheel_2_button,
                           B_State wheel_3_button,
                           B_State wheel_4_button,
                           B_State wheel_5_button,
                           B_State wheel_6_button) {
    UNUSED(far_right_button);
    UNUSED(far_left_button);
    UNUSED(wheel_1_button);
    UNUSED(wheel_2_button);
    UNUSED(wheel_3_button);
    UNUSED(wheel_4_button);
    UNUSED(wheel_5_button);
    UNUSED(wheel_6_button);
  };
  virtual bool get_has_paged_past() { return true; };
  virtual void reset_has_paged_past(){};
  int num_widgets;
  Widget **widgets;
  bool has_paged_past;
  Page_Name page_name;
};

#endif // __WIDGET_H_
