#include <network.hpp>

void x_task_wifi(void *pvParameters)
{
    for (;;)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            vTaskDelay(WIFI_CHECK_DELAY / portTICK_PERIOD_MS);
            Serial.println("[DEBUG] WIFI STACK SIZE: " + String(uxTaskGetStackHighWaterMark(nullptr)));
            continue;
        }
        vTaskDelay(TASK_DELAY);

        Serial.println("[WIFI SETUP] CONNECTING");
        WiFi.mode(WIFI_STA);
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

        unsigned long start_connection = millis();
        while (WiFi.status() != WL_CONNECTED && millis() - start_connection < WIFI_TIMEOUT)
        {
        }

        if (WiFi.status() != WL_CONNECTED)
        {
            Serial.println("[WIFI SETUP] FAILED!");
            vTaskDelay(WIFI_RECOVERY / portTICK_PERIOD_MS);
            continue;
        }

        taskYIELD();
    }
}
