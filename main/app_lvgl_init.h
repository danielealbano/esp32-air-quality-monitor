#ifndef APP_LVGL_INIT_H
#define APP_LVGL_INIT_H

#ifdef __cplusplus
extern "C" {
#endif

void app_lvgl_init_lv();
void app_lvgl_init_display_spi();
void app_lvgl_init_display_hw();
void app_lvgl_init_display_register_lvgl();
void app_lvgl_init_display();
void app_lvgl_init_register_tick_hook();
void app_lvgl_init_register_task_handler_task();
void app_lvgl_init();

#ifdef __cplusplus
}
#endif

#endif
