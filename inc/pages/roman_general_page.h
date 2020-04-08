#ifndef __ROMAN_GENERAL_PAGE_H___
#define __ROMAN_GENERAL_PAGE_H___
#include "page.h"

class Roman_General_Page : public Page {
public:
    Roman_General_Page(Widget** _widgets, int num_widgets, Page_Name page_name);
    void update(bool force_update);
    void set_visibility(gBool visible);
    int num_widgets;
    Widget** widgets;
private:
};

#endif // __ROMAN_GENERAL_PAGE_H___