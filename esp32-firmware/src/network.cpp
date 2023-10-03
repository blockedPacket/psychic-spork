#include <network.hpp>

void x_task_wifi(void *pvParameters)
{
    for (;;)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            IPAddress ipAddress(WiFi.localIP());
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

String ipv4Convert(IPAddress &ip)
{
    String ADDR;
    ADDR.reserve(16);
    ADDR = ip[0];
    ADDR += '.';
    ADDR += ip[1];
    ADDR += '.';
    ADDR += ip[2];
    ADDR += '.';
    ADDR += ip[3];
    return ADDR;
}