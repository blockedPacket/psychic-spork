#ifndef LCD_H
#define LCD_H


#include <LiquidCrystal_I2C.h>
#include <Wire.h>

constexpr unsigned rows = 4;
constexpr unsigned coluns = 20;


void drawSprite(bool animate);
void scrollMessage(unsigned row, String msg, int delay);
void x_task_lcd(void *pvParams);


#endif