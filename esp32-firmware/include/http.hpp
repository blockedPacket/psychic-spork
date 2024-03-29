#ifndef HTTP_H
#define HTTP_H

#include <HTTPClient.h>
#include <ArduinoJson.h>
#define URL "http://192.168.100.101:13000"
#define FETCH_INTERVAL 30000

void x_task_fetch(void *pvParams);
extern QueueHandle_t API_DATA_QUEUE;

struct API_DATA
{
    String cpu_percent;
    String cpu_total_cores;
    String cpu_total_threads;
    String date;
    String memory_percent;
    String memory_total;
    String memory_used;
    String cpu_freq_current;
    String cpu_freq_max;
    String cpu_freq_min;
    String nvidia_cuda_version;
    String nvidia_driver_version;
    String nvidia_fan_speed;
    String nvidia_gpu_rate;
    String nvidia_memory_free;
    String nvidia_memory_rate;
    String nvidia_memory_total;
    String nvidia_memory_used;
    String nvidia_temperature;
    String system_info;
};

#endif
