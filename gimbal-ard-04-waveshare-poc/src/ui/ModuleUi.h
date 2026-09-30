#ifndef ModuleUi_h
#define ModuleUi_h

#include <Arduino.h>
#include "lvgl.h"


#include "Define.h"
#include "../sens/SensorBno085.h"
#include "../sens/SensorRotEnc.h"
#include "../coms/ModuleWifi.h"
#include "../coms/ModuleCcApi.h"
#include "../coms/NowSrv.h"

class ModuleUi {

private:

    static lv_obj_t* rootContainer;
    static lv_obj_t* wifiContainer;

    static lv_obj_t* scanButton;
    static lv_obj_t* shutButton;
    static void handleScanButtonPress(lv_event_t* e);
    static void handleShutButtonPress(lv_event_t* e);

    static lv_obj_t* labelI;
    static lv_obj_t* sliderI;
    // static lv_obj_t* ledI;
    static lv_obj_t* encoderI;
    static lv_meter_indicator_t* indicatorI;
    
    // static void slider_event_cb(lv_event_t* e);

public:
    static void setup();
    static void update();
    static void handleCcApiConnect(bool connected);
};

#endif