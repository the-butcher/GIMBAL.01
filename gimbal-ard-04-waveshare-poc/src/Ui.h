#ifndef Ui_h
#define Ui_h

#include <Arduino.h>
#include "lvgl.h"


#include "Define.h"
#include "Orientation.h"

class Ui {

private:

    static lv_obj_t* labelI;
    static lv_obj_t* sliderI;
    static lv_obj_t* cboxSw0;
    static lv_obj_t* cboxSw1;
    static lv_obj_t* cboxSw2;

    static void slider_event_cb(lv_event_t* e);

public:
    static void setup();
    static void update();
};

#endif