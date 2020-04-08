#include "Label_Static_Widget.h"

Label_Static_Widget::Label_Static_Widget(GHandle ugfx_widget, const char* static_text) {
    this->static_text = static_text;
    this->ugfx_widget = ugfx_widget;
}
void Label_Static_Widget::set_visibility(gBool visible) {
    gwinSetVisible(this->ugfx_widget, visible);
}
void Label_Static_Widget::update(bool force_update) {
    if (force_update) {
        gwinSetText(this->ugfx_widget, this->static_text, TRUE);
    }
}
