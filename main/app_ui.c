#include <stdio.h>
#include <stdlib.h>
#include "esp_log.h"

#include "lvgl/lvgl.h"
#include "app_lvgl.h"
#include "app_ui.h"

lv_obj_t* chart_temp;
lv_chart_series_t* chart_series_temp;
lv_obj_t* chart_value_label_temp;
lv_obj_t* chart_unit_label_temp;
lv_obj_t* chart_min_max_label_temp;

lv_obj_t* chart_co2;
lv_chart_series_t* chart_series_co2;
lv_obj_t* chart_value_label_co2;
lv_obj_t* chart_unit_label_co2;
lv_obj_t* chart_min_max_label_co2;

lv_obj_t* chart_pm25;
lv_chart_series_t* chart_series_pm25;
lv_obj_t* chart_value_label_pm25;
lv_obj_t* chart_unit_label_pm25;
lv_obj_t* chart_min_max_label_pm25;

static lv_style_t chart_style;
static lv_style_t chart_name_label_style;
static lv_style_t chart_value_label_style;
static lv_style_t chart_unit_label_style;
static lv_style_t chart_min_max_label_style;

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

void app_ui_init_chart_unit_label_style()
{
    lv_style_copy(&chart_unit_label_style, &lv_style_pretty);
    chart_unit_label_style.text.font = &lv_font_roboto_12;
}

void app_ui_init_chart_min_max_label_style()
{
    lv_style_copy(&chart_min_max_label_style, &lv_style_pretty);
    chart_min_max_label_style.text.font = &lv_font_roboto_12;
}

void app_ui_chart_value_label_update(lv_obj_t* label, int value)
{
    char buf[10];
    sprintf(buf, "%d", value);

    lv_label_set_text(label, buf);
    lv_obj_realign(label);
}

void app_ui_chart_min_max_label_update(lv_obj_t* label, int min, int max)
{
    char buf[25];
    sprintf(buf, "Min %d Max %d", min, max);

    lv_label_set_text(label, buf);
    lv_obj_realign(label);
}

lv_obj_t* app_ui_chart_create(lv_obj_t* container, int min, int max)
{
    lv_obj_t* chart = lv_chart_create(container, NULL);
    lv_obj_set_style(chart, &chart_style);
    lv_obj_set_size(chart, CHART_WIDTH, 80);
    lv_obj_align(chart, NULL, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_chart_set_type(chart, LV_CHART_TYPE_AREA);
    lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_SHIFT);
    lv_chart_set_point_count(chart, CHART_SERIES_DATAPOINTS);
    lv_chart_set_series_opa(chart, LV_OPA_70);
    lv_chart_set_series_width(chart, 4);
    lv_chart_set_range(chart, min, max);

    return chart;
}

lv_chart_series_t* app_ui_chart_series_create(lv_obj_t* chart, lv_color_t color)
{
    return lv_chart_add_series(chart, color);
}

lv_obj_t* app_ui_chart_name_label_create(lv_obj_t* container, char* text)
{
    lv_obj_t* label = lv_label_create(container, NULL);
    lv_label_set_align(label, LV_LABEL_ALIGN_LEFT);
    lv_label_set_text(label, text);
    lv_obj_set_width(label, 90);
    lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_LEFT, CHART_WIDTH + CHART_RIGHT_MARGIN, 10);
    lv_obj_set_style(label, &chart_name_label_style);

    return label;
}

lv_obj_t* app_ui_chart_value_label_create(lv_obj_t* container)
{
    lv_obj_t* label = lv_label_create(container, NULL);
    lv_label_set_align(label, LV_LABEL_ALIGN_LEFT);
    lv_label_set_text(label, "0");
    lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_LEFT, CHART_WIDTH + CHART_RIGHT_MARGIN, 30);
    lv_obj_set_style(label, &chart_value_label_style);

    return label;
}

lv_obj_t* app_ui_chart_unit_label_create(lv_obj_t* container, lv_obj_t* value_label, char* unit)
{
    lv_obj_t* label = lv_label_create(container, NULL);
    lv_label_set_align(label, LV_LABEL_ALIGN_LEFT);
    lv_label_set_text(label, unit);
    lv_obj_align(label, value_label, LV_ALIGN_OUT_RIGHT_BOTTOM, 3, -5);
    lv_obj_set_style(label, &chart_min_max_label_style);

    return label;
}

lv_obj_t* app_ui_chart_min_max_label_create(lv_obj_t* container)
{
    lv_obj_t* label = lv_label_create(container, NULL);
    lv_label_set_align(label, LV_LABEL_ALIGN_LEFT);
    lv_obj_set_width(label, 90);
    lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_LEFT, CHART_WIDTH + CHART_RIGHT_MARGIN, 65);
    lv_obj_set_style(label, &chart_min_max_label_style);
    app_ui_chart_min_max_label_update(label, 0, 0);

    return label;
}

void app_ui_build()
{
    app_ui_init_chart_style();
    app_ui_init_chart_name_label_style();
    app_ui_init_chart_value_label_style();
    app_ui_init_chart_unit_label_style();
    app_ui_init_chart_min_max_label_style();

    lv_obj_t* screen = lv_disp_get_scr_act(NULL);

    lv_obj_t* container_temp = APP_UI_CONTAINER_CREATE(screen, 0, 0, 320, 80);
    lv_obj_t* container_co2 = APP_UI_CONTAINER_CREATE(screen, 0, 80, 320, 80);
    lv_obj_t* container_pm25 = APP_UI_CONTAINER_CREATE(screen, 0, 160, 320, 80);

    chart_temp = app_ui_chart_create(container_temp, CHART_SERIES_TEMP_MIN, CHART_SERIES_TEMP_MAX);
    app_ui_chart_name_label_create(container_temp, "Temperature");
    chart_value_label_temp = app_ui_chart_value_label_create(container_temp);
    chart_unit_label_temp = app_ui_chart_unit_label_create(container_temp, chart_value_label_temp, "c°");
    chart_min_max_label_temp = app_ui_chart_min_max_label_create(container_temp);
    chart_series_temp = app_ui_chart_series_create(chart_temp, LV_COLOR_RED);

    chart_co2 = app_ui_chart_create(container_co2, CHART_SERIES_CO2_MIN, CHART_SERIES_CO2_MAX);
    app_ui_chart_name_label_create(container_co2, "CO2");
    chart_value_label_co2 = app_ui_chart_value_label_create(container_co2);
    chart_unit_label_co2 = app_ui_chart_unit_label_create(container_co2, chart_value_label_co2, "ppm");
    chart_min_max_label_co2 = app_ui_chart_min_max_label_create(container_co2);
    chart_series_co2 = app_ui_chart_series_create(chart_co2, LV_COLOR_GREEN);

    chart_pm25 = app_ui_chart_create(container_pm25, CHART_SERIES_PM25_MIN, CHART_SERIES_PM25_MAX);
    app_ui_chart_name_label_create(container_pm25, "PM2.5");
    chart_value_label_pm25 = app_ui_chart_value_label_create(container_pm25);
    chart_unit_label_pm25 = app_ui_chart_unit_label_create(container_pm25, chart_value_label_pm25, "ppm");
    chart_min_max_label_pm25 = app_ui_chart_min_max_label_create(container_pm25);
    chart_series_pm25 = app_ui_chart_series_create(chart_pm25, LV_COLOR_BLUE);
}

void app_ui_chart_series_add_datapoint(lv_obj_t* chart, lv_chart_series_t * series, int value)
{
    app_lvgl_ui_global_lock_grab();

    lv_chart_set_next(chart, series, value);
    
    app_lvgl_ui_global_lock_release();
}

void app_ui_label_value_update(lv_obj_t* value_label, lv_obj_t* min_max_label, lv_obj_t* unit_label, int value, int* min, int* max)
{
    *max = *max == 0 ? value : *max;
    *max = value > *max ? value : *max;

    *min = *min == 0 ? value : *min;
    *min = value < *min ? value : *min;

    app_lvgl_ui_global_lock_grab();

    app_ui_chart_value_label_update(value_label, value);
    app_ui_chart_min_max_label_update(min_max_label, *min, *max);
    lv_obj_align(unit_label, value_label, LV_ALIGN_OUT_RIGHT_BOTTOM, 3, -5);
    
    app_lvgl_ui_global_lock_release();
}

void app_ui_chart_temp_update_value_label(int temp)
{
    static int min = 0, max = 0;

    app_ui_label_value_update(
        chart_value_label_temp,
        chart_min_max_label_temp,
        chart_unit_label_temp,
        temp,
        &min,
        &max);
}

void app_ui_chart_temp_add_datapoint(int temp)
{
    app_ui_chart_series_add_datapoint(chart_temp, chart_series_temp, temp);
}

void app_ui_chart_co2_update_value_label(int co2)
{
    static int min = 0, max = 0;

    app_ui_label_value_update(
        chart_value_label_co2,
        chart_min_max_label_co2,
        chart_unit_label_co2,
        co2,
        &min,
        &max);
}

void app_ui_chart_co2_add_datapoint(int co2)
{
    app_ui_chart_series_add_datapoint(chart_co2, chart_series_co2, co2);
}

void app_ui_chart_pm25_update_value_label(int pm25)
{
    static int min = 0, max = 0;

    app_ui_label_value_update(
        chart_value_label_pm25,
        chart_min_max_label_pm25,
        chart_unit_label_pm25,
        pm25,
        &min,
        &max);
}

void app_ui_chart_pm25_add_datapoint(int pm25)
{
    app_ui_chart_series_add_datapoint(chart_pm25, chart_series_pm25, pm25);
}
