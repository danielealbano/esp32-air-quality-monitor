#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_freertos_hooks.h"

#include "lvgl/lvgl.h"

#include "app_lvgl.h"

// static const char* TAG = "APP/LVGL";

SemaphoreHandle_t lvgl_ui_global_lock;

void app_lvgl_ui_global_lock_create()
{
	lvgl_ui_global_lock = xSemaphoreCreateMutex();
}

void app_lvgl_ui_global_lock_grab()
{
	while(xSemaphoreTake(lvgl_ui_global_lock, portMAX_DELAY) != pdTRUE)
	{
		// do nothing
	}
}

void app_lvgl_ui_global_lock_release()
{
	xSemaphoreGive(lvgl_ui_global_lock);
}

void app_lvgl_task_handler_task(void *unused)
{
	while (1)
	{
		vTaskDelay(1);

		app_lvgl_ui_global_lock_grab();
		lv_task_handler();
		app_lvgl_ui_global_lock_release();
	}
}

void app_lvgl_tick_task()
{
	lv_tick_inc(portTICK_RATE_MS);
}
