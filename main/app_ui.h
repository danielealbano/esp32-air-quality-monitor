#ifndef APP_UI_INIT_H
#define APP_UI_INIT_H

#define CHART_WIDTH 200
#define CHART_SERIES_DATAPOINTS CHART_WIDTH
#define CHART_RIGHT_MARGIN 7
#define CHART_SERIES_TEMP_MIN 0.0
#define CHART_SERIES_TEMP_MAX 50.0
#define CHART_SERIES_CO2_MIN 0.0
#define CHART_SERIES_CO2_MAX 2000.0
#define CHART_SERIES_PM25_MIN 0.0
#define CHART_SERIES_PM25_MAX 2000.0

void app_ui_build();

void app_ui_chart_temp_update_value_label(int temp);
void app_ui_chart_temp_add_datapoint(int temp);
void app_ui_chart_co2_update_value_label(int co2);
void app_ui_chart_co2_add_datapoint(int co2);
void app_ui_chart_pm25_update_value_label(int pm25);
void app_ui_chart_pm25_add_datapoint(int pm25);

#endif
