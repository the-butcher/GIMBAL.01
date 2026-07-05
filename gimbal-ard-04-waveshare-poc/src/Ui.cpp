#include "Ui.h"

lv_obj_t* Ui::labelI;
lv_obj_t* Ui::sliderI;
lv_obj_t* Ui::cboxSw0;
lv_obj_t* Ui::cboxSw1;
lv_obj_t* Ui::cboxSw2;

/**
 * callback for when the slider was changed
 */
void Ui::slider_event_cb(lv_event_t* e) {

    lv_obj_t* sliderT = lv_event_get_target(e);

    /*Refresh the text*/
    lv_label_set_text_fmt(labelI, "%" LV_PRId32, lv_slider_get_value(sliderT));
    lv_obj_align_to(labelI, sliderT, LV_ALIGN_OUT_TOP_MID, 0, -15);    /*Align top of the slider*/

}


void Ui::setup() {

    /*Create a slider in the center of the display*/
    Ui::sliderI = lv_slider_create(lv_scr_act());
    lv_slider_set_range(Ui::sliderI, -180, 180);
    lv_obj_set_width(Ui::sliderI, 200);                          /*Set the width*/
    lv_obj_center(Ui::sliderI);                                  /*Align to the center of the parent (screen)*/
    lv_obj_add_event_cb(Ui::sliderI, Ui::slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);     /*Assign an event function*/

    /*Create a label above the slider*/
    Ui::labelI = lv_label_create(lv_scr_act());
    lv_label_set_text(labelI, "0");
    lv_obj_align_to(Ui::labelI, Ui::sliderI, LV_ALIGN_OUT_TOP_MID, 0, -15);    /*Align top of the slider*/

    Ui::cboxSw0 = lv_checkbox_create(lv_scr_act());
    lv_checkbox_set_text(Ui::cboxSw0, "switch-0");
    lv_obj_align_to(Ui::cboxSw0, Ui::sliderI, LV_ALIGN_OUT_BOTTOM_MID, 0, 45);    /*Align top of the slider*/

    Ui::cboxSw1 = lv_checkbox_create(lv_scr_act());
    lv_checkbox_set_text(Ui::cboxSw1, "switch-1");
    lv_obj_align_to(Ui::cboxSw1, Ui::sliderI, LV_ALIGN_OUT_BOTTOM_MID, 0, 70);    /*Align top of the slider*/

    Ui::cboxSw2 = lv_checkbox_create(lv_scr_act());
    lv_checkbox_set_text(Ui::cboxSw2, "switch-2");
    lv_obj_align_to(Ui::cboxSw2, Ui::sliderI, LV_ALIGN_OUT_BOTTOM_MID, 0, 95);    /*Align top of the slider*/
    // lv_obj_add_event_cb(cboxSw0, event_handler, LV_EVENT_ALL, NULL);

}

void Ui::update() {

    vector________t orientation = Orientation::getOrientation();
    if (Ui::sliderI != nullptr) {
        int gradZ = (int)round(orientation.z / PI * 180);
        lv_slider_set_value(Ui::sliderI, gradZ, LV_ANIM_ON);
        lv_label_set_text_fmt(Ui::labelI, "%" LV_PRId32, gradZ);
    }

    if (Ui::cboxSw0 != nullptr) {
        if (digitalRead(GPIO_NUM____SW0) == LOW) {
            lv_obj_add_state(Ui::cboxSw0, LV_STATE_CHECKED);  // check
        } else {
            lv_obj_clear_state(Ui::cboxSw0, LV_STATE_CHECKED); // uncheck
        }
    }

    if (Ui::cboxSw1 != nullptr) {
        if (digitalRead(GPIO_NUM____SW1) == LOW) {
            lv_obj_add_state(Ui::cboxSw1, LV_STATE_CHECKED);  // check
        } else {
            lv_obj_clear_state(Ui::cboxSw1, LV_STATE_CHECKED); // uncheck
        }
    }

    if (Ui::cboxSw2 != nullptr) {
        if (digitalRead(GPIO_NUM____SW2) == LOW) {
            lv_obj_add_state(Ui::cboxSw2, LV_STATE_CHECKED);  // check
        } else {
            lv_obj_clear_state(Ui::cboxSw2, LV_STATE_CHECKED); // uncheck
        }
    }

}