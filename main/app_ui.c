#include <stdio.h>
#include <stdlib.h>
#include "esp_log.h"

#include "lvgl/lvgl.h"
#include "app_lvgl.h"

#define CHART_SERIES_CO2_MIN 0.0
#define CHART_SERIES_CO2_MAX 1500.0
#define CHART_SERIES_TEMP_MIN 0.0
#define CHART_SERIES_TEMP_MAX 50.0
#define CHART_SERIES_PM25_MIN 0.0
#define CHART_SERIES_PM25_MAX 2000.0

lv_obj_t* chart_co2;
lv_chart_series_t* chart_series_co2;
lv_obj_t* chart_value_label_co2;

lv_obj_t* chart_temp;
lv_chart_series_t* chart_series_temp;
lv_obj_t* chart_value_label_temp;

lv_obj_t* chart_pm25;
lv_chart_series_t* chart_series_pm25;
lv_obj_t* chart_value_label_pm25;

static lv_style_t chart_style;
static lv_style_t chart_name_label_style;
static lv_style_t chart_value_label_style;

static const char* TAG = "APP/UI";

#define APP_UI_CONTAINER_CREATE(parent, x, y, width, height) \
    ({ \
         \
        lv_obj_t* __obj = lv_obj_create(parent, NULL); \
        lv_obj_set_pos(__obj, x, y); \
        lv_obj_set_size(__obj, width, height); \
        lv_obj_set_style(__obj, &lv_style_plain); \
        __obj; \
    })

void app_ui_init_chart_style()
{
    lv_style_copy(&chart_style, &lv_style_pretty);

    chart_style.body.radius = 0;
}

void app_ui_init_chart_name_label_style()
{
    lv_style_copy(&chart_name_label_style, &lv_style_pretty);
    chart_name_label_style.text.font = &lv_font_roboto_12;
}

void app_ui_init_chart_value_label_style()
{
    lv_style_copy(&chart_value_label_style, &lv_style_pretty);
    chart_value_label_style.text.font = &lv_font_roboto_28;
}

lv_obj_t* app_ui_chart_create(lv_obj_t* container, int min, int max)
{
    lv_obj_t* chart = lv_chart_create(container, NULL);
    lv_obj_set_style(chart, &chart_style);
    lv_obj_set_size(chart, 230, 80);
    lv_obj_align(chart, NULL, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
    lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_SHIFT);
    lv_chart_set_point_count(chart, 60);
    lv_chart_set_series_opa(chart, LV_OPA_70);
    lv_chart_set_series_width(chart, 4);
    lv_chart_set_range(chart, min, max);

    return chart;
}

lv_chart_series_t* app_ui_chart_series_create(lv_obj_t* chart, lv_color_t color)
{
    return lv_chart_add_series(chart, color);
}

lv_obj_t* app_ui_chart_labels_create(lv_obj_t* container, char* text)
{
    lv_obj_t * chart_name_label = lv_label_create(container, NULL);
    lv_label_set_align(chart_name_label, LV_LABEL_ALIGN_LEFT);
    lv_label_set_text(chart_name_label, text);
    lv_obj_set_width(chart_name_label, 90);
    lv_obj_align(chart_name_label, NULL, LV_ALIGN_IN_TOP_LEFT, 240, 10);
    lv_obj_set_style(chart_name_label, &chart_name_label_style);

    lv_obj_t * chart_value_label = lv_label_create(container, NULL);
    lv_label_set_align(chart_value_label, LV_LABEL_ALIGN_LEFT);
    lv_label_set_text(chart_value_label, "0");
    lv_obj_set_width(chart_value_label, 90);
    lv_obj_align(chart_value_label, NULL, LV_ALIGN_IN_TOP_LEFT, 240, 30);
    lv_obj_realign(chart_value_label);
    lv_obj_set_style(chart_value_label, &chart_value_label_style);

    return chart_value_label;
}

void app_ui_build()
{
    app_ui_init_chart_style();
    app_ui_init_chart_name_label_style();
    app_ui_init_chart_value_label_style();

    lv_obj_t* screen = lv_disp_get_scr_act(NULL);

    lv_obj_t* container_temp = APP_UI_CONTAINER_CREATE(screen, 0, 0, 320, 80);
    lv_obj_t* container_co2 = APP_UI_CONTAINER_CREATE(screen, 0, 80, 320, 80);
    lv_obj_t* container_pm25 = APP_UI_CONTAINER_CREATE(screen, 0, 160, 320, 80);

    chart_value_label_temp = app_ui_chart_labels_create(container_temp, "Temp (c°)");
    chart_temp = app_ui_chart_create(container_temp, 0, 50);
    chart_series_temp = app_ui_chart_series_create(chart_temp, LV_COLOR_RED);

    chart_value_label_co2 = app_ui_chart_labels_create(container_co2, "CO2 (ppm)");
    chart_co2 = app_ui_chart_create(container_co2, 0, 1200);
    chart_series_co2 = app_ui_chart_series_create(chart_co2, LV_COLOR_GREEN);

    chart_value_label_pm25 = app_ui_chart_labels_create(container_pm25, "PM2.5 (ppm)");
    chart_pm25 = app_ui_chart_create(container_pm25, 0, 1200);
    chart_series_pm25 = app_ui_chart_series_create(chart_pm25, LV_COLOR_BLUE);
}

void app_ui_chart_series_add_point(lv_obj_t * chart, lv_chart_series_t * series, int value)
{
    app_lvgl_ui_global_lock_grab();

    lv_chart_set_next(chart, series, value);
    
    app_lvgl_ui_global_lock_release();
}

void app_ui_label_value_update(lv_obj_t * label, int value)
{
    app_lvgl_ui_global_lock_grab();

    char buf[10];
    sprintf(buf, "%d", value);
    lv_label_set_text(label, buf);
    
    app_lvgl_ui_global_lock_release();
}

void app_ui_chart_temp_add_point(int temp)
{
    app_ui_chart_series_add_point(chart_temp, chart_series_temp, temp);
    app_ui_label_value_update(chart_value_label_temp, temp);
}

void app_ui_chart_co2_add_point(int co2)
{
    app_ui_chart_series_add_point(chart_co2, chart_series_co2, co2);
    app_ui_label_value_update(chart_value_label_co2, co2);
}

void app_ui_chart_pm25_add_point(int pm25)
{
    app_ui_chart_series_add_point(chart_pm25, chart_series_pm25, pm25);
    app_ui_label_value_update(chart_value_label_pm25, pm25);
}
