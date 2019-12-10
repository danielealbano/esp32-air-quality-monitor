#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_freertos_hooks.h"

#include "lvgl/lvgl.h"
#include "disp_spi.h"
#include "ili9341.h"

#include "app_lvgl_init.h"
#include "app_lvgl.h"

static const char* TAG = "APP/LVGL/INIT";

static lv_color_t buf1[DISP_BUF_SIZE];
static lv_color_t buf2[DISP_BUF_SIZE];
static lv_disp_buf_t disp_buf;
lv_disp_drv_t disp_drv;

void app_lvgl_init_register_tick_hook()
{
	esp_register_freertos_tick_hook(app_lvgl_tick_task);
}

void app_lvgl_init_register_task_handler_task()
{
	xTaskCreate(app_lvgl_task_handler_task, "app_lvgl_task_handler_task", 4096, NULL, 50, NULL);
}

void app_lvgl_init()
{
	ESP_LOGI(TAG, "Initializing LVGL");

	app_lvgl_ui_global_lock_create();

	lv_init();

	disp_spi_init();
	ili9341_init();

    lv_disp_buf_init(&disp_buf, buf1, buf2, DISP_BUF_SIZE);

	lv_disp_drv_init(&disp_drv);
	disp_drv.flush_cb = ili9341_flush;
	disp_drv.buffer = &disp_buf;
	lv_disp_drv_register(&disp_drv);

	app_lvgl_init_register_tick_hook();
	app_lvgl_init_register_task_handler_task();

	ESP_LOGI(TAG, "LVGL initialized");
}
