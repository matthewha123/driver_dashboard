#ifndef __WIDGET_H_
#define __WIDGET_H_
#include "gfx.h"
#include "button.h"
#include <stdint.h>

class Widget {
public:
    virtual void update(bool force_update);
    virtual void set_visibility(gBool visible);
    virtual void set_selected(gBool selected) {UNUSED(selected);};
    virtual void modify_param(B_State inc_button) {UNUSED(inc_button); };
    virtual void set_ms(uint32_t ms) {UNUSED(ms);};
protected:
    GHandle ugfx_widget;
};


#endif // __WIDGET_H_

