#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
// #include "esp_spi_flash.h"

#include "app_mhz19_init.h"
#include "app_lvgl_init.h"
#include "app_mhz19.h"
#include "app_ui.h"

static const char* TAG = "APP/MAIN";

void app_print_info()
{
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);

    printf("This is ESP32 chip with %d CPU cores, WiFi%s%s, ",
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");
    printf("silicon revision %d, ", chip_info.revision);

//    printf("%dMB %s flash", spi_flash_get_chip_size() / (1024 * 1024),
//            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

	printf("\n");
    fflush(stdout);
}

void app_main_update_ui()
{
	while (1)
	{
		vTaskDelay(pdMS_TO_TICKS(100));

        int co2 = app_mhz19_get_co2();
        //int temp = app_mhz19_get_temp();

        app_ui_set_co2_value(co2);
	}
}

extern "C" void app_main(void)
{
	ESP_LOGI(TAG, "App start");

	app_print_info();
	app_mhz19_init();

	//app_lvgl_init();
    
    //app_ui_build();

    //app_main_update_ui();
}
