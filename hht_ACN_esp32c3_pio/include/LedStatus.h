#pragma once

#include <Arduino.h>

const int wifi_led = 12;
const int hht_led = 13;

void int_LedStatus()
{
    pinMode(wifi_led, OUTPUT);
    pinMode(hht_led, OUTPUT);
}

void LedStatus_Blink(char led_pin, unsigned int blink_time)
{
    digitalWrite(led_pin, LOW);
    delay(blink_time);
    digitalWrite(led_pin, HIGH);
    delay(blink_time);
}

void LedStatus_Light(char led_pin)
{
    digitalWrite(led_pin, HIGH);
}

void LedStatus_Quench(char led_pin)
{
    digitalWrite(led_pin, LOW);
}