#ifndef __PAGE_GENERAL_H___
#define __PAGE_GENERAL_H___
#include "page.h"

class Page_General : public Page {
public:
    Page_General(Widget** _widgets, int num_widgets, Page_Name page_name);
    void update(bool force_update);
    void set_visibility(gBool visible);
    int num_widgets;
    Widget** widgets;
private:
};

#endif // __PAGE_H_

