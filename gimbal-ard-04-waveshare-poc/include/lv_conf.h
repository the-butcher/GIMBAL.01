/**
 * lv_conf.h  –  LVGL v8 configuration
 * Place this file in your project's  include/  folder.
 * The build flag -DLV_CONF_INCLUDE_SIMPLE tells LVGL to pick it up.
 *
 * Only the settings you are most likely to need are uncommented here.
 * Full reference: https://github.com/lvgl/lvgl/blob/master/lv_conf_template.h
 */

#if 1 /*Set it to "1" to enable the content*/

#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>


 /* ── Colour depth ──────────────────────────────────────── */
#define LV_COLOR_DEPTH 16   /* 16-bit RGB565 – matches CO5300 default */

/* ── Memory ────────────────────────────────────────────── */
/*
 * LVGL manages its own heap for widgets.
 * 64 kB is comfortable; increase if you get MEM_FULL warnings.
 * With 8 MB PSRAM available you can safely go up to 256 kB+.
 */
#define LV_MEM_CUSTOM 0
#define LV_MEM_SIZE   (48U * 1024U)   /* 64 kB internal heap */

 /* ── HAL tick ──────────────────────────────────────────── */
#define LV_TICK_CUSTOM 0
#if LV_TICK_CUSTOM
#define LV_TICK_CUSTOM_INCLUDE  "Arduino.h"
#define LV_TICK_CUSTOM_SYS_TIME_EXPR (millis())
#endif

/* ── Display refresh ───────────────────────────────────── */
#define LV_DISP_DEF_REFR_PERIOD  10   /* ms – 100 fps cap */
#define LV_INDEV_DEF_READ_PERIOD 10   /* ms – touch poll  */

/* ── Double-buffering hint ─────────────────────────────── */
/*
 * We allocate two line-buffers in main.cpp.
 * No change needed here; this is informational.
 */

 /* ── Logging ───────────────────────────────────────────── */
#define LV_USE_LOG 1
#if LV_USE_LOG
#define LV_LOG_LEVEL LV_LOG_LEVEL_WARN
#define LV_LOG_PRINTF 1   /* send to Serial via printf */
#endif

/* ── Font ──────────────────────────────────────────────── */
// #define LV_FONT_MONTSERRAT_14 1
// #define LV_FONT_MONTSERRAT_16 1
// #define LV_FONT_MONTSERRAT_20 1
#define LV_FONT_MONTSERRAT_30 1
#define LV_FONT_DEFAULT &lv_font_montserrat_30

/* ── Waveshare Demo ─────────────────── */
// #define LV_USE_DEMO_WIDGETS 1

/* ── Widgets needed by the touch demo ─────────────────── */
#define LV_USE_SLIDER  1
#define LV_USE_LABEL   1
// #define LV_USE_BTN     1
// #define LV_USE_ARC     1
// #define LV_USE_TABVIEW 1
// #define LV_USE_BAR     1

/* ── Theme ─────────────────────────────────────────────── */
// https://forum.lvgl.io/t/image-colors-not-displaying-properly/12672/4
#define LV_COLOR_16_SWAP 1
#define LV_USE_THEME_DEFAULT 1
#if LV_USE_THEME_DEFAULT
#define LV_THEME_DEFAULT_DARK    1   /* dark mode looks great on AMOLED */
#define LV_THEME_DEFAULT_GROW    1
#define LV_THEME_DEFAULT_TRANSITION_TIME 80
#endif

/* ── Misc ──────────────────────────────────────────────── */
#define LV_USE_PERF_MONITOR 0   /* flip to 1 to overlay FPS / CPU% */
#define LV_USE_MEM_MONITOR  0

#endif /* LV_CONF_H */
#endif /* End of "1" */
