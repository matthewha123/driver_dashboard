#ifndef __LABEL_STATIC_WIDGET_H__
#define __LABEL_STATIC_WIDGET_H__

#include "widget.h"

class Label_Static_Widget : public Widget {
public:
    Label_Static_Widget(GHandle ugfx_widget, const char* static_text);
    void update(bool force_update);
    void set_visibility(gBool visible);
protected:
    const char* static_text;
};

#endif // __LABEL_STATIC_WIDGET_H__

