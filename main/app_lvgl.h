#ifndef APP_LVGL_H
#define APP_LVGL_H

#ifdef __cplusplus
extern "C" {
#endif

void app_lvgl_ui_global_lock_create();
void app_lvgl_ui_global_lock_grab();
void app_lvgl_ui_global_lock_release();
void app_lvgl_task_handler_task(void *unused);
void app_lvgl_tick_task();

#ifdef __cplusplus
}
#endif

#endif
