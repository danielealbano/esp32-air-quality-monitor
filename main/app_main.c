#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"

#include "app_mhz19_init.h"
#include "app_lvgl_init.h"
#include "app_mhz19.h"
#include "app_ui.h"

static const char* TAG = "APP/MAIN";

void app_print_info()
{
    char buf[99];

    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);

    memset(buf, 0, sizeof(buf));

    sprintf(buf + strlen(buf), "This is ESP32 chip with %d CPU cores, WiFi%s%s, ",
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");
    sprintf(buf + strlen(buf), "silicon revision %d", chip_info.revision);

    ESP_LOGI(TAG, "%s", buf);
}

void app_main_update_ui()
{
	while (1)
	{
        int co2 = app_mhz19_get_co2();
        int temp = app_mhz19_get_temp();

        app_ui_chart_co2_add_point(co2);
        app_ui_chart_temp_add_point(temp);

		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

void app_main(void)
{
	ESP_LOGI(TAG, "App start");

	app_print_info();
	app_mhz19_init();

	app_lvgl_init();
    
    app_ui_build();

    app_main_update_ui();
}
