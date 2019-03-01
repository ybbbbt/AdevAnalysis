#pragma once

extern "C" {

typedef void (*gyroscope_handler)(double t, double x, double y, double z);
typedef void (*accelerometer_handler)(double t, double x, double y, double z);
typedef void (*error_handler)(const char* error_message);

void sensors_gyroscope_handler_set(gyroscope_handler h);
void sensors_accelerometer_handler_set(accelerometer_handler h);
void sensors_error_handler_set(error_handler h);

void sensors_init();
void sensors_deinit();
void sensors_parse_data(const void* bytes, long size);

}

