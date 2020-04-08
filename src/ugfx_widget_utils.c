#include "ugfx_widget_utils.h"
#include <stdio.h>

GWidgetInit widget_template;
char numBuffer[20];

/*
    Create a widget that can be used for all components to specify x, y, width, height
*/
void create_widget(uint8_t width, uint8_t height, uint8_t x, uint8_t y) {
    gwinWidgetClearInit(&widget_template);
    widget_template.g.show = FALSE;

    widget_template.g.width = width; widget_template.g.height = height;
    widget_template.g.x = x; widget_template.g.y = y;
    widget_template.text = "*";
}

void create_label(GHandle* gh, uint8_t width, uint8_t height, uint8_t x, uint8_t y) {
    create_widget(width, height, x, y);
    *gh = gwinLabelCreate(0, &widget_template); 
}

/*
    Create a label.
    attr specifies the label text that precedes the label's value
    attr : value
*/
void create_label_with_attr(GHandle* gh, const char* attr, uint8_t width, uint8_t height, uint8_t x, uint8_t y, uint8_t tab_width) {
    create_label(gh, width, height, x, y);
    gwinLabelSetAttribute(*gh, tab_width, attr);
}

// currently this is slower than Dot_Matrix_NHD_US25664.c's draw bitmap
void create_image(GHandle* gh, const char* file_name, uint8_t width, uint8_t height, uint8_t x, uint8_t y) {
    create_widget(width, height, x,y);
    *gh = gwinImageCreate(0, &widget_template.g);
    gwinImageOpenFile(*gh, file_name);
}

void create_radio_button(GHandle* gh, char* label, Radio_Group group, uint8_t width, uint8_t height, uint8_t x, uint8_t y) {
    create_widget(width, height, x, y);
    widget_template.text = label;
    *gh = gwinRadioCreate(0, &widget_template, group);

    // press with gwinRadioPress(gh);
}

// commented out for warnings
void create_slider(GHandle* gh, char* label, uint8_t lower_bound, uint8_t upper_bound, uint8_t width, uint8_t height, uint8_t x, uint8_t y) {
    UNUSED(gh);
    UNUSED(lower_bound);
    UNUSED(upper_bound);
    create_widget(width, height, x,y);
    widget_template.text = label;
    // *gh = gwinSliderCreate(0, &widget_template);
    // gwinSliderSetRange(*gh, lower_bound, upper_bound);
}

void set_label_text_num(GHandle label, int32_t num) {

  sprintf(numBuffer,"%ld", num);
  gwinSetText(label, numBuffer, TRUE);
}

void set_error(GHandle gh, uint8_t error_state) {
    switch (error_state) {
        case 0:
            gwinSetText(gh,  "", TRUE);
            break;
        case 1:
            gwinSetText(gh,  "*", TRUE);
            break;
        case 2:
            gwinSetText(gh,  "-", TRUE);
            break;
        default:
            break;
    }
}
