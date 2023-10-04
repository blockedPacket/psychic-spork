#ifndef SPRITES_H
#define SPRITES_H

#include <Arduino.h>

byte INVADER_STATE1[8] = {
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111};

byte INVADER_STATE2[8] = {
    B11111,
    B11111,
    B10001,
    B10001,
    B10001,
    B10001,
    B11111,
    B11111};

byte RIGHT_ICON[8] = {
    B10000,
    B11000,
    B11100,
    B11110,
    B11110,
    B11100,
    B11000,
    B10000};

byte LEFT_ICON[8] = {
    B00001,
    B00011,
    B00111,
    B01111,
    B01111,
    B00111,
    B00011,
    B00001};

byte BOTTOM_BAR[8] = {
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B11111,
    B11111};
#endif