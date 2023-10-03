#include <sprites.hpp>
#include <lcd.hpp>

LiquidCrystal_I2C lcd(0x27, coluns, rows);

void drawSprite(bool animate)
{

    lcd.createChar(0, INVADER_STATE1);
    lcd.createChar(1, INVADER_STATE2);

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
    for (size_t i = 0; i < coluns; i++)
    {
        msg = " " + msg;
    }

    msg = msg + " ";

    for (size_t p = 0; p < msg.length(); p++)
    {
        lcd.setCursor(0, row);
        lcd.print(msg.substring(p, p + coluns));
        vTaskDelay(delay);
    }
}

void drawicons()
{
    lcd.setCursor(0, 0);
    lcd.write(4);

    for (size_t i = 0; i < 4; i++)
    {
        lcd.setCursor(0, i);
        lcd.write(4);
        lcd.setCursor(19, i);
        lcd.write(3);

    }

    for(size_t i = 1; i < 19; i ++){
        lcd.setCursor(i,3);
        lcd.write(5);
    }
}


void x_task_lcd(void *pvParams)
{

    lcd.init();
    lcd.backlight();
    struct API_DATA data;
    lcd.createChar(3, LEFT_ICON);
    lcd.createChar(4, RIGHT_ICON);
    lcd.createChar(5, BOTTOM_BAR);

    for (;;)
    {

        lcd.setCursor(2, 0);
        scrollMessage(1, "WELLCOME !!!", 300);
        vTaskDelay(2000);
        lcd.clear();

        drawSprite(false);
        vTaskDelay(200);
        drawSprite(true);
        vTaskDelay(300);
        drawSprite(false);

        vTaskDelay(2000);
        lcd.clear();

        vTaskDelay(1000);

        if (WiFi.status() == WL_CONNECTED)
        {

            if (xQueueReceive(API_DATA_QUEUE, &data, portMAX_DELAY) == pdPASS)
            {

                drawicons();

                lcd.setCursor(1, 0);
                lcd.print("CPU-USAGE:  " + data.cpu_percent);
                lcd.setCursor(1, 1);
                lcd.print("CPU-CORES:  " + data.cpu_total_cores);
                // lcd.setCursor(1, 2);
                // lcd.print("CPU-THR:    " + data.cpu_total_threads);
                vTaskDelay(20000);
                lcd.clear();

                drawicons();

                lcd.setCursor(1, 0);
                lcd.print("RAM-USAGE: " + data.memory_percent);
                lcd.setCursor(1, 1);
                lcd.print("RAM-TOTAL: " + data.memory_total);
                lcd.setCursor(1, 2);
                lcd.print("RAM-USED:  " + data.memory_used);
                vTaskDelay(20000);
                lcd.clear();


            }
        }
        else
        {
            scrollMessage(1, "NOT DATA", 250);
        }

        // String current_ip;

        // if (xQueueReceive(ESP32_NETWORK_QUEUE, &esp32_wifi_data, portMAX_DELAY) == pdPASS)
        // {

        //     current_ip = esp32_wifi_data.ipv4;
        // }
        // else
        // {
        //     current_ip = "";
        // }

        // if (current_ip.length() > 0)
        // {
        //     unsigned long start = millis();
        //     while (millis() - start < WIFI_CHECK_DELAY)
        //     {
        //         scrollMessage(1, "IP ADDRESS: " + current_ip, 250);
        //         lcd.clear();
        //         vTaskDelay(200);
        //     }
        // }

        Serial.println("[DEBUG] STACK SIZE: " + String(uxTaskGetStackHighWaterMark(nullptr)));
        taskYIELD();
    }
}