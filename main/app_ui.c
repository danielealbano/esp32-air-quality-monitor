#include <stdlib.h>

#include "lvgl/lvgl.h"
#include "app_lvgl.h"

static lv_obj_t* meter_co2;
static lv_obj_t* label_co2_value;
static lv_obj_t* meter_pm25;
static lv_obj_t* label_pm25_value;

static lv_style_t style_lmeter;

void app_ui_init_meter_style()
{
    lv_style_copy(&style_lmeter, &lv_style_pretty_color);
    style_lmeter.line.width = 4;
    style_lmeter.line.color = LV_COLOR_SILVER;
    //style_lmeter.body.main_color = lv_color_hex(0x91bfed);
    style_lmeter.body.grad_color = lv_color_hex(0x04386c);
    style_lmeter.body.padding.left = 14;
}

lv_obj_t* app_ui_init_meter(lv_obj_t* scr)
{
    lv_obj_t* lmeter;

    lmeter = lv_lmeter_create(scr, NULL);
    lv_lmeter_set_range(lmeter, 0, 5000);
    lv_lmeter_set_value(lmeter, 0);
    lv_lmeter_set_scale(lmeter, 240, 31);
    lv_lmeter_set_style(lmeter, LV_LMETER_STYLE_MAIN, &style_lmeter);
    lv_obj_set_size(lmeter, 130, 130);
    lv_obj_align(lmeter, NULL, LV_ALIGN_CENTER, 0, 0);

    return lmeter;
}

lv_obj_t* app_ui_init_meter_labels(lv_obj_t* scr, const char* text)
{
    lv_obj_t* label_text = lv_label_create(scr, NULL);
    lv_label_set_text(label_text, text);
    lv_obj_align(label_text, NULL, LV_ALIGN_CENTER, 0, -5);

    lv_obj_t* label_value =  lv_label_create(scr, NULL);
    lv_label_set_text(label_value, "0");
    lv_obj_align(label_value, NULL, LV_ALIGN_CENTER, 0, 20);

    return label_value;
}

void app_ui_init_co2(lv_obj_t* scr)
{
    meter_co2 = app_ui_init_meter(scr);
    label_co2_value = app_ui_init_meter_labels(scr, "CO2 (ppm)");
}

void app_ui_init_pm25(lv_obj_t* scr)
{
    meter_pm25 = app_ui_init_meter(scr);
    label_pm25_value = app_ui_init_meter_labels(scr, "PM2.5 (ppm)");
}

void app_ui_build()
{
    app_ui_init_meter_style();

    lv_obj_t* scr = lv_disp_get_scr_act(NULL);

    lv_obj_t* scr_left = lv_obj_create(scr, NULL);
    lv_obj_set_pos(scr_left, 0, 0);
    lv_obj_set_size(scr_left, 160, 240);
    lv_obj_set_style(scr_left, &lv_style_plain);

    lv_obj_t* scr_left_top = lv_obj_create(scr_left, NULL);
    lv_obj_set_pos(scr_left_top, 0, 0);
    lv_obj_set_size(scr_left_top, 160, 140);
    lv_obj_set_style(scr_left_top, &lv_style_plain);

    lv_obj_t* scr_left_bottom = lv_obj_create(scr_left, NULL);
    lv_obj_set_pos(scr_left_bottom, 0, 120);
    lv_obj_set_size(scr_left_bottom, 160, 140);
    lv_obj_set_style(scr_left_bottom, &lv_style_plain);

    app_ui_init_co2(scr_left_top);
    app_ui_init_pm25(scr_left_bottom);

    lv_obj_t* scr_right = lv_obj_create(scr, NULL);
    lv_obj_set_pos(scr_right, 160, 0);
    lv_obj_set_size(scr_right, 160, 240);
    lv_obj_set_style(scr_right, &lv_style_plain);
}

void app_ui_update_meter_and_label(int value, lv_obj_t* meter, lv_obj_t* label)
{
    char value_text[10];

    itoa(value, value_text, 10);

    app_lvgl_ui_global_lock_grab();

    lv_lmeter_set_value(meter_co2, value);
    lv_label_set_text(label_co2_value, value_text);
    
    app_lvgl_ui_global_lock_release();
}

void app_ui_set_co2_value(int co2)
{
    app_ui_update_meter_and_label(co2, meter_co2, label_co2_value);
}

void app_ui_set_pm25_value(int pm25)
{
    app_ui_update_meter_and_label(pm25, meter_pm25, label_pm25_value);
}
