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
#include "support.h"

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
    int co2 = 0;
    int temp = 0;

    uint32_t chart_update_interval_ms = (1 * 60 * 60 / CHART_SERIES_DATAPOINTS) * 1000;

    uint32_t datapoint_added_millis = 0;

	while (1)
	{
        int co2_new = app_mhz19_get_co2();
        int temp_new = app_mhz19_get_temp();

        temp = temp_new < -30 ? temp : temp_new;
        co2 = co2_new >= 5000 ? co2 : co2_new;

        app_ui_chart_temp_update_value_label(temp);
        app_ui_chart_co2_update_value_label(co2);
        app_ui_chart_pm25_update_value_label(1000);

        if (millis() - datapoint_added_millis >= chart_update_interval_ms)
        {
            app_ui_chart_temp_add_datapoint(temp);
            app_ui_chart_co2_add_datapoint(co2);
            app_ui_chart_pm25_add_datapoint(1000);

            datapoint_added_millis = millis();
        }

		vTaskDelay(pdMS_TO_TICKS(2000));
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
