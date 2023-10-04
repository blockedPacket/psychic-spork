#include <http.hpp>

QueueHandle_t API_DATA_QUEUE;

String httpGETRequest(const char *server)
{
    HTTPClient http;
    String payload = "";

    if (!http.begin(server))
    {
        return payload;
        http.end();
    }
    unsigned responseCode = http.GET();

    if (responseCode == 200)
    {
        payload = http.getString();
        http.end();
        return payload;
    }
    else
    {
        Serial.println("HTTP STATUS CODE: " + responseCode);
        http.end();
        return payload;
    }
}

void x_task_fetch(void *pvParams)
{

    struct API_DATA c_data;
    StaticJsonDocument<1024> doc;

    for (;;)
    {

        if (WiFi.status() == WL_CONNECTED)
        {
            String payload = httpGETRequest(URL);
            if (!payload)
            {
                taskYIELD();
                continue;
            }

            DeserializationError error = deserializeJson(doc, payload);

            if (error)
            {
                Serial.println("[*] ERROR GET DATA");
                xQueueReset(API_DATA_QUEUE);
                taskYIELD();
            }

            const char *cpu_freq_current = doc["cpu_freq_current"];           
            const char *cpu_freq_max = doc["cpu_freq_max"];                   
            const char *cpu_freq_min = doc["cpu_freq_min"];                   
            const char *cpu_percent = doc["cpu_percent"];                     
            const char *cpu_total_cores = doc["cpu_total_cores"];             
            const char *cpu_total_threads = doc["cpu_total_threads"];         
            const char *date = doc["date"];                                   
            const char *memory_percent = doc["memory_percent"];               
            const char *memory_total = doc["memory_total"];                   
            const char *memory_used = doc["memory_used"];                     
            const char *nvidia_cuda_version = doc["nvidia_cuda_version"];     
            const char *nvidia_driver_version = doc["nvidia_driver_version"]; 
            const char *nvidia_fan_speed = doc["nvidia_fan_speed"];           
            const char *nvidia_gpu_rate = doc["nvidia_gpu_rate"];             
            const char *nvidia_memory_free = doc["nvidia_memory_free"];       
            const char *nvidia_memory_rate = doc["nvidia_memory_rate"];       
            const char *nvidia_memory_total = doc["nvidia_memory_total"];     
            const char *nvidia_memory_used = doc["nvidia_memory_used"];       
            const char *nvidia_temperature = doc["nvidia_temperature"];       
            const char *system_info = doc["system_info"];

            c_data.cpu_percent = cpu_percent;
            c_data.cpu_total_cores = cpu_total_cores;
            c_data.cpu_total_threads = cpu_total_threads;
            c_data.date = date;
            c_data.memory_percent = memory_percent;
            c_data.memory_total = memory_total;
            c_data.memory_used = memory_used;
            c_data.cpu_freq_current = cpu_freq_current;
            c_data.cpu_freq_max = cpu_freq_max;
            c_data.cpu_freq_min = cpu_freq_min;
            c_data.system_info = system_info;

            c_data.nvidia_cuda_version = nvidia_cuda_version;
            c_data.nvidia_driver_version = nvidia_driver_version;
            c_data.nvidia_fan_speed = nvidia_fan_speed;
            c_data.nvidia_gpu_rate = nvidia_gpu_rate;
            c_data.nvidia_memory_free = nvidia_memory_free;
            c_data.nvidia_memory_rate = nvidia_memory_rate;
            c_data.nvidia_memory_total = nvidia_memory_total;
            c_data.nvidia_memory_used = nvidia_memory_used;
            c_data.nvidia_temperature = nvidia_temperature;

            xQueueSend(API_DATA_QUEUE, &c_data, portMAX_DELAY);

            Serial.println("[DEBUG] HTTP STACK SIZE: " + String(uxTaskGetStackHighWaterMark(nullptr)));
            vTaskDelay(FETCH_INTERVAL);
            taskYIELD();
        }

        vTaskDelay(2000);
        taskYIELD();
    }
}
