#ifndef TouchDisplay_Ft3168Sh8601_h
#define TouchDisplay_Ft3168Sh8601_h

#include <Arduino.h>
#include <stdint.h>
#include "esp_lcd_panel_vendor.h"

#include "Arduino.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "esp_err.h"
#include "esp_timer.h"
#include "esp_lcd_panel_interface.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_commands.h"
#include "lvgl.h"
#include "demos/lv_demos.h"
#include "esp_check.h"
#include "driver/gpio.h"
#include "Display_Sh8601.h"
#include "Touch_Ft3168.h"

#include "Define.h"
#include "Orientation.h"

class TouchDisplay_Ft3168Sh8601 {
private:

    static SemaphoreHandle_t lvgl_mux;
    static esp_lcd_panel_io_handle_t amoled_panel_io_handle;

    static lv_obj_t* labelI;
    static lv_obj_t* sliderI;
    static lv_obj_t* cboxSw0;
    static lv_obj_t* cboxSw1;
    static lv_obj_t* cboxSw2;

    static void slider_event_cb(lv_event_t* e);

public:

    static void touchDisplayBegin(void);
    static bool touchDisplayAcquireLock(int timeout_ms);
    static void touchDisplayReleaseLock(void);
    static void touchDisplayRunUiTask(void* arg);
    static void touchDisplayIncreaseTick(void* arg);
    static bool touchDisplayNotifyFlushReady(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t* edata, void* user_ctx);
    static void touchDisplayFlushCallback(lv_disp_drv_t* drv, const lv_area_t* area, lv_color_t* color_map);
    static void touchDisplayRoundCallback(struct _lv_disp_drv_t* disp_drv, lv_area_t* area);
    static void touchDisplayTouchCallback(lv_indev_drv_t* drv, lv_indev_data_t* data);
    static esp_err_t touchDisplaySetBrightness(uint8_t brig);

};

#endif