#ifndef NETWORK_H
#define NETWORK_H

#include <WiFi.h>
#define WIFI_SSID "DEFCON"
#define WIFI_PASSWORD "Explode@98"

void x_task_wifi(void *pvParameters);

constexpr unsigned long WIFI_CHECK_DELAY = 600000;
constexpr unsigned long TASK_DELAY = 10000;
constexpr unsigned long WIFI_TIMEOUT = 20000;
constexpr unsigned long WIFI_RECOVERY = 30000;

#endif