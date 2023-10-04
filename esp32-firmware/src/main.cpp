#include <Arduino.h>
#include <sound.hpp>
#include <lcd.hpp>
#include <network.hpp>
#include <http.hpp>

void setup()
{

  Serial.begin(115200);
  soundmessage();
  delay(2000);

  API_DATA_QUEUE = xQueueCreate(2, sizeof(struct API_DATA));
  xTaskCreate(x_task_wifi, "X_TASK_WIFI", 6000, NULL, 1, NULL);
  xTaskCreate(x_task_lcd, "X_TASK_LCD", 3000, NULL, 2, NULL);
  xTaskCreate(x_task_fetch, "X_TASK_FETCH", 4000, NULL, 2, NULL);

  vTaskStartScheduler();
}

void loop()
{
  vTaskSuspend(NULL);
}
