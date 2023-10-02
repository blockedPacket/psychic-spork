#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>

#define WIFI_SSID "DEFCON"
#define WIFI_PASSWORD "Explode@98"

constexpr unsigned LCD_R = 4;
constexpr unsigned LCD_C = 20;

String current_ip;

LiquidCrystal_I2C lcd(0x27, LCD_C, LCD_R);

byte sprites[8] = {
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111};

byte A1[8] = {
    B11111,
    B11111,
    B10001,
    B10001,
    B10001,
    B10001,
    B11111,
    B11111};

/* Task handles*/
TaskHandle_t X_LCD;
TaskHandle_t X_WIFI;

void drawSprite(bool animate)
{

  lcd.createChar(0, sprites);
  lcd.createChar(1, A1);

  lcd.command(64);

  lcd.home();

  lcd.setCursor(9, 0);
  lcd.write(0);
  lcd.setCursor(11, 0);
  lcd.write(0);

  for (int i = 8; i < 13; i++)
  {
    if (i == 9 || i == 11)
    {
      lcd.setCursor(i, 1);
      if (animate)
      {
        lcd.write(0);
        continue;
      }
      lcd.write(1);
      continue;
    }
    lcd.setCursor(i, 1);
    lcd.write(0);
  }

  for (int i = 7; i < 14; i++)
  {
    lcd.setCursor(i, 2);
    lcd.write(0);
  }

  lcd.setCursor(8, 3);
  lcd.write(0);
  lcd.setCursor(12, 3);
  lcd.write(0);
}




void scrollMessage(unsigned row, String msg, int delay)
{
  for (size_t i = 0; i < LCD_C; i++)
  {
    msg = " " + msg;
  }

  msg = msg + " ";

  for (size_t p = 0; p < msg.length(); p++)
  {
    lcd.setCursor(0, row);
    lcd.print(msg.substring(p, p + LCD_C));
    vTaskDelay(delay);
  }
}

void x_task_lcd(void *pvParams)
{
  for (;;)
  {

    lcd.setCursor(0, 0);
    lcd.print("DASDASDASDASDASDASDASD");
    vTaskDelay(2000);
    lcd.clear();

    drawSprite(false);
    vTaskDelay(200);
    drawSprite(true);
    vTaskDelay(300);
    drawSprite(false);

    vTaskDelay(2000);
    lcd.clear();

    if (current_ip.length() > 0)
    {
      scrollMessage(1, "IP ADDRESS: " + current_ip, 250);
      lcd.clear();

    } else {
      scrollMessage(0, "NETWORK FAILED !!!", 250);
      lcd.clear();
    }

    lcd.draw_horizontal_graph(2,0,80,20);
    lcd.clear();

    Serial.println("[DEBUG] STACK SIZE: " + String(uxTaskGetStackHighWaterMark(nullptr)));

    vTaskDelay(200);
    taskYIELD();
  }
}

String ipv4Convert(IPAddress& ip) {
  String ADDR;
  ADDR.reserve(16);
  ADDR = ip[0];  ADDR += '.';
  ADDR += ip[1]; ADDR += '.';
  ADDR += ip[2]; ADDR += '.';
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

  Wire.begin();
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  delay(200);

  xTaskCreate(x_task_lcd, "X_TASK_LCD", 3000, NULL, 1, &X_LCD);
  xTaskCreate(x_task_wifi, "X_TASK_WIFI", 5000, NULL, 2, &X_WIFI);
  vTaskStartScheduler();

}

void loop()
{
  vTaskSuspend(NULL);
}
