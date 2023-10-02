#include <Arduino.h>
#include <sound.hpp>
#include <lcd.hpp>
#include <WiFi.h>

#define WIFI_SSID "DEFCON"
#define WIFI_PASSWORD "Explode@98"

/* Task handles*/
TaskHandle_t X_LCD;
TaskHandle_t X_WIFI;




String current_ip;










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

void x_task_wifi(void *pvParameters)
{

  (void)pvParameters;
  constexpr unsigned long CHECK_DELAY = 600000;
  constexpr unsigned long TASK_DELAY = 10000;
  constexpr unsigned long WIFI_TIMEOUT = 20000;
  constexpr unsigned long WIFI_RECOVERY = 30000;
  for (;;)
  {

    if (WiFi.status() == WL_CONNECTED)
    {
      IPAddress ipAddress(WiFi.localIP());
      current_ip = ipv4Convert(ipAddress);
      vTaskDelay(CHECK_DELAY / portTICK_PERIOD_MS);
      Serial.println("[DEBUG] STACK SIZE: " + String(uxTaskGetStackHighWaterMark(nullptr)));
      taskYIELD();
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
      current_ip = "";
      vTaskDelay(WIFI_RECOVERY / portTICK_PERIOD_MS);
      continue;
    }
    taskYIELD();
  }
}

void setup()
{

  Serial.begin(115200);
  delay(200);

  soundmessage();

  xTaskCreate(x_task_lcd, "X_TASK_LCD", 3000, NULL, 1, &X_LCD);
  xTaskCreate(x_task_wifi, "X_TASK_WIFI", 5000, NULL, 2, &X_WIFI);
  vTaskStartScheduler();
}

void loop()
{
  vTaskSuspend(NULL);
}
