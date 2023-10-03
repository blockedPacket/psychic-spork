#ifndef HTTP_H
#define HTTP_H


#include <HTTPClient.h>
#include <ArduinoJson.h>
#define URL "http://192.168.100.101:13000"
#define FETCH_INTERVAL 10000

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
};

#endif