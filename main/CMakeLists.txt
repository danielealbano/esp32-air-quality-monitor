idf_component_register(
	SRCS 
		support.c
		app_lvgl_init.c
		app_lvgl.c
		app_mhz19_init.c
		app_mhz19.c
		app_ui.c
		app_main.c
	INCLUDE_DIRS "."
	REQUIRES lvgl_ili9341 lvgl lv_examples mhz19)
