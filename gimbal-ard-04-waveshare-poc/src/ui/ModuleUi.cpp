#include "ModuleUi.h"

lv_obj_t* ModuleUi::labelI;
lv_obj_t* ModuleUi::sliderI;
lv_obj_t* ModuleUi::ledI;
lv_obj_t* ModuleUi::encoderI;
lv_meter_indicator_t* ModuleUi::indicatorI;

lv_obj_t* ModuleUi::rootContainer;
lv_obj_t* ModuleUi::wifiContainer;
lv_obj_t* ModuleUi::scanButton;
lv_obj_t* ModuleUi::shutButton;

// /**
//  * callback for when the slider was changed
//  */
// void Ui::slider_event_cb(lv_event_t* e) {

//     lv_obj_t* sliderT = lv_event_get_target(e);

//     /*Refresh the text*/
//     lv_label_set_text_fmt(labelI, "%" LV_PRId32, lv_slider_get_value(sliderT));
//     lv_obj_align_to(labelI, sliderT, LV_ALIGN_OUT_TOP_MID, 0, -15);    /*Align top of the slider*/

// }

void ModuleUi::handleScanButtonPress(lv_event_t* e) {

    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        Serial.println("scan button clicked");
        ModuleWifi::scanNetworks(NETWORK_CCAPI);
        if (ModuleWifi::isConnected()) {
            Serial.println("issue ccapi root request");
            bool ccApiSuccess = ModuleCcApi::issueGet(CCAPI__________ROOT); // finalize connection to camera
            ModuleUi::handleCcApiConnect(ccApiSuccess);
        };
    }

}

void ModuleUi::handleShutButtonPress(lv_event_t* e) {

    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        Serial.println("shut button clicked");
        if (ModuleWifi::isConnected()) {
            // Serial.println("issue ccapi root request");
            // bool ccApiSuccess = ModuleCcApi::issueGet(CCAPI__________ROOT); // finalize connection to camera
            // ModuleUi::handleCcApiConnect(ccApiSuccess);
            bool ccApiSuccess = ModuleCcApi::issuePost(CCAPI_SHUTTERBUTTON, "{\"af\":true}"); // finalize connection to camera
        };
    }

}

void ModuleUi::handleCcApiConnect(bool connected) {

    if (connected) {
        // hide "scan wifi button"
        lv_obj_add_flag(ModuleUi::scanButton, LV_OBJ_FLAG_HIDDEN);
        // show ccapi call buttons
        lv_obj_clear_flag(ModuleUi::shutButton, LV_OBJ_FLAG_HIDDEN);
    } else {
        // show "scan wifi button"
        lv_obj_clear_flag(ModuleUi::scanButton, LV_OBJ_FLAG_HIDDEN);
        // hide ccapi call buttons
        lv_obj_add_flag(ModuleUi::shutButton, LV_OBJ_FLAG_HIDDEN);
    }

}



void ModuleUi::setup() {

    // Create a container with ROW flex direction
    ModuleUi::rootContainer = lv_obj_create(lv_scr_act());
    lv_obj_set_size(ModuleUi::rootContainer, TD_DIM________X, TD_DIM________Y);
    lv_obj_align(ModuleUi::rootContainer, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_flex_flow(ModuleUi::rootContainer, LV_FLEX_FLOW_COLUMN);

    lv_obj_set_style_bg_color(ModuleUi::rootContainer, lv_palette_main(LV_PALETTE_GREY), LV_PART_MAIN);

    ModuleUi::wifiContainer = lv_obj_create(ModuleUi::rootContainer);
    // lv_obj_set_size(Ui::wifiContainer, TD_DIM________X, TD_DIM________Y);
    // lv_obj_align(Ui::rootContainer, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_size(ModuleUi::wifiContainer, LV_PCT(100), 100);
    lv_obj_set_flex_flow(ModuleUi::wifiContainer, LV_FLEX_FLOW_COLUMN); // let wifi also be a flex container
    lv_obj_set_style_pad_all(ModuleUi::wifiContainer, 0, LV_PART_MAIN);

    lv_obj_set_style_bg_color(ModuleUi::wifiContainer, lv_palette_main(LV_PALETTE_GREY), LV_PART_MAIN);
    lv_obj_set_style_border_width(ModuleUi::wifiContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ModuleUi::scanButton = lv_btn_create(ModuleUi::wifiContainer);
    lv_obj_add_event_cb(ModuleUi::scanButton, ModuleUi::handleScanButtonPress, LV_EVENT_ALL, NULL);
    lv_obj_set_size(ModuleUi::scanButton, LV_PCT(100), 45);

    lv_obj_t* scanButtonLabel = lv_label_create(ModuleUi::scanButton);
    lv_label_set_text(scanButtonLabel, "scan wifi");
    lv_obj_center(scanButtonLabel);

    ModuleUi::shutButton = lv_btn_create(ModuleUi::wifiContainer);
    lv_obj_add_event_cb(ModuleUi::shutButton, ModuleUi::handleShutButtonPress, LV_EVENT_ALL, NULL);
    lv_obj_set_size(ModuleUi::shutButton, LV_PCT(100), 45);

    lv_obj_t* shutButtonLabel = lv_label_create(ModuleUi::shutButton);
    lv_label_set_text(shutButtonLabel, "shutterbutton");
    lv_obj_center(shutButtonLabel);

    // initial, pre connect, state
    ModuleUi::handleCcApiConnect(false);

    /*Create a slider in the center of the display*/
    ModuleUi::sliderI = lv_slider_create(ModuleUi::rootContainer);
    lv_slider_set_range(ModuleUi::sliderI, -180, 180);
    lv_obj_set_width(ModuleUi::sliderI, LV_PCT(100));
    lv_obj_center(ModuleUi::sliderI);                                  /*Align to the center of the parent (screen)*/
    // lv_obj_add_event_cb(ModuleUi::sliderI, ModuleUi::slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);     /*Assign an event function*/

    /*Create a label above the slider*/
    ModuleUi::labelI = lv_label_create(ModuleUi::rootContainer);
    lv_label_set_text(labelI, "0");
    lv_obj_align_to(ModuleUi::labelI, ModuleUi::sliderI, LV_ALIGN_OUT_TOP_MID, 0, -15);
    lv_obj_set_width(ModuleUi::labelI, LV_PCT(100));
    lv_obj_center(ModuleUi::labelI);

    ModuleUi::encoderI = lv_meter_create(ModuleUi::rootContainer);
    lv_obj_center(ModuleUi::encoderI);
    // lv_obj_set_width(ModuleUi::encoderI, LV_PCT(100));
    lv_meter_scale_t * scale_min = lv_meter_add_scale(ModuleUi::encoderI);
    lv_meter_set_scale_ticks(ModuleUi::encoderI, scale_min, 32, 1, 10, lv_palette_main(LV_PALETTE_GREY));
    lv_meter_set_scale_range(ModuleUi::encoderI, scale_min, 0, 256, 360, 0);

    ModuleUi::indicatorI = lv_meter_add_needle_line(ModuleUi::encoderI, scale_min, 4, lv_palette_main(LV_PALETTE_GREY), -10);
    
    ModuleUi::ledI = lv_led_create(ModuleUi::rootContainer);
    lv_obj_center(ModuleUi::ledI);
    lv_led_off(ModuleUi::ledI);

}

void ModuleUi::update() {

    vector________t orientation = SensorBno085::getOrientation();
    if (ModuleUi::sliderI != nullptr) {
        double gradZ = orientation.z / PI * 180.0;
        lv_slider_set_value(ModuleUi::sliderI, round(gradZ), LV_ANIM_ON);
        lv_label_set_text(ModuleUi::labelI, String(gradZ, 2).c_str());
    }

    if (ModuleUi::ledI != nullptr) {
        if (NowSrv::pndSendDataFlag) {
            lv_led_on(ModuleUi::ledI);
        } else {
            lv_led_off(ModuleUi::ledI);
        }
    }

    if (ModuleUi::encoderI != nullptr && ModuleUi::indicatorI != nullptr) {
        lv_meter_set_indicator_value(ModuleUi::encoderI, ModuleUi::indicatorI, (SensorRotEnc::getPosition() + 256 * 100) % 256);
    }

}