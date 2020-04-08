#include "roman_general_page.h"
#include <stdio.h>

Roman_General_Page::Roman_General_Page(Widget** _widgets, int num_widgets, Page_Name page_name) {
    this->widgets = new Widget*[num_widgets];
    for(int i = 0; i < num_widgets; i++) {
        this->widgets[i] = _widgets[i];
    }
    this->num_widgets = num_widgets;
    this->page_name = page_name;
}

void Roman_General_Page::update(bool force_update) {
    for(int i = 0; i < this->num_widgets; i++) {
        this->widgets[i]->update(force_update);
    }

    // draw dividers
    gdispDrawLine(84,0, 84, 63, White);
    gdispDrawLine(169, 0, 169, 63, White);
}

void Roman_General_Page::set_visibility(gBool visible) {
    for(int i = 0; i < this->num_widgets; i++) {
        this->widgets[i]->set_visibility(visible);
    }
}
