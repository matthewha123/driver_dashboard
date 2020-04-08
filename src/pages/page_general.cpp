#include "page_general.h"

Page_General::Page_General(Widget** _widgets, int num_widgets, Page_Name page_name) {
    this->widgets = new Widget*[num_widgets];
    for(int i = 0; i < num_widgets; i++) {
        this->widgets[i] = _widgets[i];
    }
    this->num_widgets = num_widgets;
    this->page_name = page_name;
}

void Page_General::update(bool force_update) {
    for(int i = 0; i < this->num_widgets; i++) {
        this->widgets[i]->update(force_update);
    }
}

void Page_General::set_visibility(gBool visible) {
    for(int i = 0; i < this->num_widgets; i++) {
        this->widgets[i]->set_visibility(visible);
    }
}
