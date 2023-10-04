#include <sprites.hpp>
#include <lcd.hpp>

LiquidCrystal_I2C lcd(0x27, coluns, rows);

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
        scrollMessage(1, "WELCOME !!!", 300);
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

            IPAddress ipAddress(WiFi.localIP());

            lcd.setCursor(2, 0);
            scrollMessage(1, "ESP IPV4: " + ipv4Convert(ipAddress), 300);
            vTaskDelay(2000);
            lcd.clear();

            if (xQueueReceive(API_DATA_QUEUE, &data, portMAX_DELAY) == pdPASS && data.memory_percent != NULL)
            {
                drawicons();
                lcd.setCursor(1, 1);
                lcd.print("CPU-RATE:  " + data.cpu_percent);
                lcd.setCursor(1, 2);
                lcd.print(data.cpu_freq_current);
                vTaskDelay(20000);
                lcd.clear();

                scrollMessage(2, data.system_info, 300);
                lcd.clear();

                drawicons();
                lcd.setCursor(1, 0);
                lcd.print("RAM-RATE:  " + data.memory_percent);
                lcd.setCursor(1, 1);
                lcd.print("RAM-TOTAL: " + data.memory_total);
                lcd.setCursor(1, 2);
                lcd.print("RAM-USED:  " + data.memory_used);
                vTaskDelay(20000);
                lcd.clear();

                drawicons();
                lcd.setCursor(1, 1);
                lcd.print("GPU-FAN:  " + data.nvidia_fan_speed);
                lcd.setCursor(1, 2);
                lcd.print("GPU-TEMP: " + data.nvidia_temperature);
                vTaskDelay(20000);
                lcd.clear();

                drawicons();
                lcd.setCursor(1, 0);
                lcd.print("GPU-MEM-TOTAL: " + data.nvidia_memory_total);
                lcd.setCursor(1, 1);
                lcd.print("GPU-MEM-USED:  " + data.nvidia_memory_used);
                lcd.setCursor(1, 2);
                lcd.print("GPU-MEM-FREE:  " + data.nvidia_memory_free);
                lcd.setCursor(1, 3);
                lcd.print("GPU-MEM-RATE:  " + data.nvidia_memory_rate);
                vTaskDelay(20000);
                lcd.clear();
                scrollMessage(3, "NVIDIA CUDA VERSION: " + data.nvidia_cuda_version, 300);
                scrollMessage(3, "NVIDIA DRIVER VERSION: " + data.nvidia_driver_version, 300);
                scrollMessage(3, "MAXIMUM PROCESSOR FREQUENCY: " + data.cpu_freq_max, 300);
                scrollMessage(3, "MINIMUM PROCESSOR FREQUENCY: " + data.cpu_freq_min, 300);
                lcd.clear();
            } else {
                 scrollMessage(1, "ERROR CONNECTING TO API", 250);
            }
        }
        else
        {
            scrollMessage(1, "NO INTERNET", 250);
        }

        Serial.println("[DEBUG] STACK SIZE: " + String(uxTaskGetStackHighWaterMark(nullptr)));
        taskYIELD();
    }
}