# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

set(lvgl_lv_conf_h_path ${CMAKE_SOURCE_DIR}/3rdparty/lv_port_esp32_ili9341/components/lvgl/lv_conf.h)
set(main_lv_conf_h_path ${CMAKE_SOURCE_DIR}/main/lv_conf.h)

add_custom_target(
	copy_lv_conf_h
	ALL
	COMMAND
        ${CMAKE_COMMAND} -E copy ${main_lv_conf_h_path} ${lvgl_lv_conf_h_path}
	COMMENT "Updating lv_conf.h from main")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

set(EXTRA_COMPONENT_DIRS 3rdparty/lv_port_esp32_ili9341/components)

project(esp32-air-quality-monitor)
