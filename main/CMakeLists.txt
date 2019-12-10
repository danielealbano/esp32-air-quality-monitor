idf_component_register(
	SRCS 
		support.c
		app_lvgl_init.c
		app_lvgl.c
		app_mhz19_init.cpp
		app_mhz19.cpp
		app_ui.c
		app_main.cpp
	INCLUDE_DIRS "."
	REQUIRES lvgl_ili9341 lvgl lv_examples mhz19)
