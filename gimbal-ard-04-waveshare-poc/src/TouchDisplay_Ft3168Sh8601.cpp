#include "TouchDisplay_Ft3168Sh8601.h"

#define LCD_HOST    SPI2_HOST

#define SH8601_ID 0x86
#define CO5300_ID 0xff

static const sh8601_lcd_init_cmd_t lcd_init_cmds[] = {
  {0x11, (uint8_t[]) { 0x00 }, 0, 80},
  {0xC4, (uint8_t[]) { 0x80 }, 1, 0},
  {0x35, (uint8_t[]) { 0x00 }, 1, 0},
  {0x53, (uint8_t[]) { 0x20 }, 1, 1},
  {0x63, (uint8_t[]) { 0xFF }, 1, 1},
  {0x51, (uint8_t[]) { 0x00 }, 1, 1},
  {0x29, (uint8_t[]) { 0x00 }, 0, 10},
  {0x51, (uint8_t[]) { 0xFF }, 1, 0},    // 亮度
};

SemaphoreHandle_t TouchDisplay_Ft3168Sh8601::lvgl_mux = NULL; //mutex semaphores
esp_lcd_panel_io_handle_t TouchDisplay_Ft3168Sh8601::amoled_panel_io_handle = NULL;

lv_obj_t* TouchDisplay_Ft3168Sh8601::labelI;
lv_obj_t* TouchDisplay_Ft3168Sh8601::sliderI;
lv_obj_t* TouchDisplay_Ft3168Sh8601::cboxSw0;
lv_obj_t* TouchDisplay_Ft3168Sh8601::cboxSw1;
lv_obj_t* TouchDisplay_Ft3168Sh8601::cboxSw2;

/**
 * callback for when the slider was changed
 */
void TouchDisplay_Ft3168Sh8601::slider_event_cb(lv_event_t* e) {

    lv_obj_t* sliderT = lv_event_get_target(e);

    /*Refresh the text*/
    lv_label_set_text_fmt(labelI, "%" LV_PRId32, lv_slider_get_value(sliderT));
    lv_obj_align_to(labelI, sliderT, LV_ALIGN_OUT_TOP_MID, 0, -15);    /*Align top of the slider*/

}

void TouchDisplay_Ft3168Sh8601::touchDisplayBegin(void) {

    Touch_Ft3168::touchBegin();

    static lv_disp_draw_buf_t disp_buf; // contains internal graphic buffer(s) called draw buffer(s)
    static lv_disp_drv_t disp_drv;      // contains callback functions

    const spi_bus_config_t buscfg = SH8601_PANEL_BUS_QSPI_CONFIG(
        EXAMPLE_PIN_NUM_LCD_PCLK,
        EXAMPLE_PIN_NUM_LCD_DATA0,
        EXAMPLE_PIN_NUM_LCD_DATA1,
        EXAMPLE_PIN_NUM_LCD_DATA2,
        EXAMPLE_PIN_NUM_LCD_DATA3,
        EXAMPLE_LCD_H_RES * EXAMPLE_LCD_V_RES * LCD_BIT_PER_PIXEL / 8
    );

    ESP_ERROR_CHECK_WITHOUT_ABORT(spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO));
    esp_lcd_panel_io_handle_t io_handle = NULL;

    const esp_lcd_panel_io_spi_config_t io_config = SH8601_PANEL_IO_QSPI_CONFIG(
        EXAMPLE_PIN_NUM_LCD_CS,
        example_notify_lvgl_flush_ready,
        &disp_drv
    );

    sh8601_vendor_config_t vendor_config = {
      .init_cmds = lcd_init_cmds,
      .init_cmds_size = sizeof(lcd_init_cmds) / sizeof(lcd_init_cmds[0]),
      .flags =
      {
        .use_qspi_interface = 1,
      },
    };

    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST, &io_config, &io_handle));

    amoled_panel_io_handle = io_handle;
    esp_lcd_panel_handle_t panel_handle = NULL;
    const esp_lcd_panel_dev_config_t panel_config = {
      .reset_gpio_num = EXAMPLE_PIN_NUM_LCD_RST,
      .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
      .bits_per_pixel = LCD_BIT_PER_PIXEL,
      .vendor_config = &vendor_config,
    };

    ESP_ERROR_CHECK_WITHOUT_ABORT(Display_Sh8601::esp_lcd_new_panel_sh8601(io_handle, &panel_config, &panel_handle));
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_lcd_panel_init(panel_handle));
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_lcd_panel_disp_on_off(panel_handle, true));

    lv_init();
    lv_color_t* buf1 = (lv_color_t*)heap_caps_malloc(EXAMPLE_LCD_H_RES * EXAMPLE_LVGL_BUF_HEIGHT * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf1);
    lv_color_t* buf2 = (lv_color_t*)heap_caps_malloc(EXAMPLE_LCD_H_RES * EXAMPLE_LVGL_BUF_HEIGHT * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf2);
    lv_disp_draw_buf_init(&disp_buf, buf1, buf2, EXAMPLE_LCD_H_RES * EXAMPLE_LVGL_BUF_HEIGHT);
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = EXAMPLE_LCD_H_RES;
    disp_drv.ver_res = EXAMPLE_LCD_V_RES;
    disp_drv.flush_cb = example_lvgl_flush_cb;
    disp_drv.rounder_cb = example_lvgl_rounder_cb;
    disp_drv.draw_buf = &disp_buf;
    disp_drv.user_data = panel_handle;

    // #ifdef EXAMPLE_Rotate_90
    disp_drv.sw_rotate = 1;
    disp_drv.rotated = LV_DISP_ROT_180;
    // #endif
    lv_disp_t* disp = lv_disp_drv_register(&disp_drv);

    static lv_indev_drv_t indev_drv;    // Input device driver (Touch)
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.disp = disp;
    indev_drv.read_cb = example_lvgl_touch_cb;
    lv_indev_drv_register(&indev_drv);

    const esp_timer_create_args_t lvgl_tick_timer_args = {
      .callback = &example_increase_lvgl_tick,
      .name = "lvgl_tick"
    };

    esp_timer_handle_t lvgl_tick_timer = NULL;

    ESP_ERROR_CHECK(esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(lvgl_tick_timer, EXAMPLE_LVGL_TICK_PERIOD_MS * 1000));

    TouchDisplay_Ft3168Sh8601::lvgl_mux = xSemaphoreCreateMutex(); //mutex semaphores
    assert(TouchDisplay_Ft3168Sh8601::lvgl_mux);

    // xTaskCreate(example_lvgl_port_task, "LVGL", EXAMPLE_LVGL_TASK_STACK_SIZE, NULL, EXAMPLE_LVGL_TASK_PRIORITY, NULL);
    xTaskCreatePinnedToCore(example_lvgl_port_task, "run-loop-lvgl", EXAMPLE_LVGL_TASK_STACK_SIZE, NULL, EXAMPLE_LVGL_TASK_PRIORITY, NULL, 1); // run on primary core
    if (example_lvgl_lock(-1)) {

        /*Create a slider in the center of the display*/
        sliderI = lv_slider_create(lv_scr_act());
        lv_slider_set_range(sliderI, -180, 180);
        lv_obj_set_width(sliderI, 200);                          /*Set the width*/
        lv_obj_center(sliderI);                                  /*Align to the center of the parent (screen)*/
        lv_obj_add_event_cb(sliderI, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);     /*Assign an event function*/

        /*Create a label above the slider*/
        labelI = lv_label_create(lv_scr_act());
        lv_label_set_text(labelI, "0");
        lv_obj_align_to(labelI, sliderI, LV_ALIGN_OUT_TOP_MID, 0, -15);    /*Align top of the slider*/

        cboxSw0 = lv_checkbox_create(lv_scr_act());
        lv_checkbox_set_text(cboxSw0, "switch-0");
        lv_obj_align_to(cboxSw0, sliderI, LV_ALIGN_OUT_BOTTOM_MID, 0, 45);    /*Align top of the slider*/

        cboxSw1 = lv_checkbox_create(lv_scr_act());
        lv_checkbox_set_text(cboxSw1, "switch-1");
        lv_obj_align_to(cboxSw1, sliderI, LV_ALIGN_OUT_BOTTOM_MID, 0, 70);    /*Align top of the slider*/

        cboxSw2 = lv_checkbox_create(lv_scr_act());
        lv_checkbox_set_text(cboxSw2, "switch-2");
        lv_obj_align_to(cboxSw2, sliderI, LV_ALIGN_OUT_BOTTOM_MID, 0, 95);    /*Align top of the slider*/
        // lv_obj_add_event_cb(cboxSw0, event_handler, LV_EVENT_ALL, NULL);

        // lv_demo_widgets();      /* A widgets example */
        // lv_demo_music();        /* A modern, smartphone-like music player demo. */
        // lv_demo_stress();       /* A stress test for LVGL. */
        // lv_demo_benchmark();    /* A demo to measure the performance of LVGL or to compare different settings. */

        // Release the mutex
        example_lvgl_unlock();

    }

}

bool TouchDisplay_Ft3168Sh8601::example_lvgl_lock(int timeout_ms) {
    assert(lvgl_mux && "bsp_display_start must be called first");
    const TickType_t timeout_ticks = (timeout_ms == -1) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
    return xSemaphoreTake(lvgl_mux, timeout_ticks) == pdTRUE;
}

void TouchDisplay_Ft3168Sh8601::example_lvgl_unlock(void) {
    assert(lvgl_mux && "bsp_display_start must be called first");
    xSemaphoreGive(lvgl_mux);
}

void TouchDisplay_Ft3168Sh8601::example_lvgl_port_task(void* arg) {

    uint32_t task_delay_ms = EXAMPLE_LVGL_TASK_MAX_DELAY_MS;
    for (;;) {

        if (example_lvgl_lock(-1)) {

            task_delay_ms = lv_timer_handler();

            vector________t orientation = Orientation::getOrientation();
            if (TouchDisplay_Ft3168Sh8601::sliderI != nullptr) {
                int gradZ = (int)round(orientation.z / PI * 180);
                lv_slider_set_value(TouchDisplay_Ft3168Sh8601::sliderI, gradZ, LV_ANIM_ON);
                lv_label_set_text_fmt(labelI, "%" LV_PRId32, gradZ);
            }

            if (TouchDisplay_Ft3168Sh8601::cboxSw0 != nullptr) {
                if (digitalRead(GPIO_NUM____SW0) == LOW) {
                    lv_obj_add_state(cboxSw0, LV_STATE_CHECKED);  // check
                } else {
                    lv_obj_clear_state(cboxSw0, LV_STATE_CHECKED); // uncheck
                }
            }

            if (TouchDisplay_Ft3168Sh8601::cboxSw1 != nullptr) {
                if (digitalRead(GPIO_NUM____SW1) == LOW) {
                    lv_obj_add_state(cboxSw1, LV_STATE_CHECKED);  // check
                } else {
                    lv_obj_clear_state(cboxSw1, LV_STATE_CHECKED); // uncheck
                }
            }

            if (TouchDisplay_Ft3168Sh8601::cboxSw2 != nullptr) {
                if (digitalRead(GPIO_NUM____SW2) == LOW) {
                    lv_obj_add_state(cboxSw2, LV_STATE_CHECKED);  // check
                } else {
                    lv_obj_clear_state(cboxSw2, LV_STATE_CHECKED); // uncheck
                }
            }


            example_lvgl_unlock();

        }

        if (task_delay_ms > EXAMPLE_LVGL_TASK_MAX_DELAY_MS) {
            task_delay_ms = EXAMPLE_LVGL_TASK_MAX_DELAY_MS;
        } else if (task_delay_ms < EXAMPLE_LVGL_TASK_MIN_DELAY_MS) {
            task_delay_ms = EXAMPLE_LVGL_TASK_MIN_DELAY_MS;
        }
        vTaskDelay(pdMS_TO_TICKS(task_delay_ms));

    }

}

void TouchDisplay_Ft3168Sh8601::example_increase_lvgl_tick(void* arg) {
    lv_tick_inc(EXAMPLE_LVGL_TICK_PERIOD_MS);
}

bool TouchDisplay_Ft3168Sh8601::example_notify_lvgl_flush_ready(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t* edata, void* user_ctx) {
    lv_disp_drv_t* disp_driver = (lv_disp_drv_t*)user_ctx;
    lv_disp_flush_ready(disp_driver);
    return false;
}

void TouchDisplay_Ft3168Sh8601::example_lvgl_flush_cb(lv_disp_drv_t* drv, const lv_area_t* area, lv_color_t* color_map) {
    esp_lcd_panel_handle_t panel_handle = (esp_lcd_panel_handle_t)drv->user_data;
    const int offsetx1 = area->x1 + 0x14;
    const int offsetx2 = area->x2 + 0x14;
    const int offsety1 = area->y1;
    const int offsety2 = area->y2;
    esp_lcd_panel_draw_bitmap(panel_handle, offsetx1, offsety1, offsetx2 + 1, offsety2 + 1, color_map);
}

void TouchDisplay_Ft3168Sh8601::example_lvgl_rounder_cb(struct _lv_disp_drv_t* disp_drv, lv_area_t* area) {

    uint16_t x1 = area->x1;
    uint16_t x2 = area->x2;

    uint16_t y1 = area->y1;
    uint16_t y2 = area->y2;

    // round the start of coordinate down to the nearest 2M number
    area->x1 = (x1 >> 1) << 1;
    area->y1 = (y1 >> 1) << 1;
    // round the end of coordinate up to the nearest 2N+1 number
    area->x2 = ((x2 >> 1) << 1) + 1;
    area->y2 = ((y2 >> 1) << 1) + 1;
}

void TouchDisplay_Ft3168Sh8601::example_lvgl_touch_cb(lv_indev_drv_t* drv, lv_indev_data_t* data) {
    uint16_t tp_x, tp_y;
    uint8_t win = Touch_Ft3168::getTouch(&tp_x, &tp_y);
    if (win) {
        data->point.x = tp_x;
        data->point.y = tp_y;
        data->state = LV_INDEV_STATE_PRESSED;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

esp_err_t TouchDisplay_Ft3168Sh8601::set_amoled_backlight(uint8_t brig) {
    uint32_t lcd_cmd = 0x51;
    lcd_cmd &= 0xff;
    lcd_cmd <<= 8;
    lcd_cmd |= 0x02 << 24;
    return esp_lcd_panel_io_tx_param(amoled_panel_io_handle, lcd_cmd, &brig, 1);
}
