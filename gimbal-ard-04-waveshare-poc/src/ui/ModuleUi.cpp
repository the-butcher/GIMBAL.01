#include "ModuleUi.h"

// lv_obj_t* Ui::labelI;
// lv_obj_t* Ui::sliderI;

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

    /*Create a container with ROW flex direction*/
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

    // /*Create a slider in the center of the display*/
    // Ui::sliderI = lv_slider_create(lv_scr_act());
    // lv_slider_set_range(Ui::sliderI, -180, 180);
    // lv_obj_set_width(Ui::sliderI, 200);                          /*Set the width*/
    // lv_obj_center(Ui::sliderI);                                  /*Align to the center of the parent (screen)*/
    // lv_obj_add_event_cb(Ui::sliderI, Ui::slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);     /*Assign an event function*/

    // /*Create a label above the slider*/
    // Ui::labelI = lv_label_create(lv_scr_act());
    // lv_label_set_text(labelI, "0");
    // lv_obj_align_to(Ui::labelI, Ui::sliderI, LV_ALIGN_OUT_TOP_MID, 0, -15);    /*Align top of the slider*/


}

void ModuleUi::update() {

    // vector________t orientation = Orientation::getOrientation();
    // if (Ui::sliderI != nullptr) {
    //     int gradZ = (int)round(orientation.z / PI * 180);
    //     lv_slider_set_value(Ui::sliderI, gradZ, LV_ANIM_ON);
    //     lv_label_set_text_fmt(Ui::labelI, "%" LV_PRId32, gradZ);
    // }


}