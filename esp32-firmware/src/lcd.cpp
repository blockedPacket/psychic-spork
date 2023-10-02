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

void x_task_lcd(void *pvParams)
{

    lcd.init();
    lcd.backlight();
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

        // if (current_ip.length() > 0)
        // {
        //     scrollMessage(1, "IP ADDRESS: " + current_ip, 250);
        //     lcd.clear();
        // }
        // else
        // {
        //     scrollMessage(0, "NETWORK FAILED !!!", 250);
        //     lcd.clear();
        // }

        lcd.draw_horizontal_graph(2, 0, 80, 20);
        lcd.clear();

        Serial.println("[DEBUG] STACK SIZE: " + String(uxTaskGetStackHighWaterMark(nullptr)));

        vTaskDelay(200);
        taskYIELD();
    }
}