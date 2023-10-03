#include <http.hpp>
#include <WiFi.h>

QueueHandle_t API_DATA_QUEUE;

String httpGETRequest(const char *server)
{
    HTTPClient http;
    http.begin(server);
    unsigned responseCode = http.GET();

    String payload = "";
    if (responseCode == 200)
    {
        payload = http.getString();
    }
    else
    {
        Serial.println("HTTP STATUS CODE: " + responseCode);
    }

    http.end();

    return payload;
}

void x_task_fetch(void *pvParams)
{

    struct API_DATA c_data;
    StaticJsonDocument<384> doc;

    for (;;)
    {

        if (WiFi.status() == WL_CONNECTED)
        {
            String payload = httpGETRequest(URL);
            vTaskDelay(200);

            DeserializationError error = deserializeJson(doc, payload);

            if (error)
            {
                Serial.print("deserializeJson() failed: ");
                Serial.println(error.c_str());
                taskYIELD();
                continue;
            }

            const char *cpu_percent = doc["cpu_percent"];             // "7.3%"
            const char *cpu_total_cores = doc["cpu_total_cores"];     // "4"
            const char *cpu_total_threads = doc["cpu_total_threads"]; // "4"
            const char *date = doc["date"];                           // "Mon, 02 Oct 2023 00:00:00 GMT"
            const char *memory_percent = doc["memory_percent"];       // "39.2%"
            const char *memory_total = doc["memory_total"];           // "16GB"
            const char *memory_used = doc["memory_used"];             // "6GB"

            c_data.cpu_percent = cpu_percent;             // "7.3%"
            c_data.cpu_total_cores = cpu_total_cores;     // "4"
            c_data.cpu_total_threads = cpu_total_threads; // "4"
            c_data.date = date;                           // "Mon, 02 Oct 2023 00:00:00 GMT"
            c_data.memory_percent = memory_percent;       // "39.2%"
            c_data.memory_total = memory_total;           // "16GB"
            c_data.memory_used = memory_used;             // "6GB"

            xQueueSend(API_DATA_QUEUE, &c_data, portMAX_DELAY);

            Serial.println(payload);
            Serial.println("[DEBUG] HTTP STACK SIZE: " + String(uxTaskGetStackHighWaterMark(nullptr)));
            vTaskDelay(FETCH_INTERVAL);
            taskYIELD();
            continue;
        }

        vTaskDelay(2000);
        taskYIELD();
    }
}
