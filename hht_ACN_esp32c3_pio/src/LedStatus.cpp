#include <Arduino.h>

#include "LedStatus.h"

void int_LedStatus()
{
    pinMode(wifi_led, OUTPUT);
    pinMode(hht_led, OUTPUT);
}

void LedStatus_Blink(char led_pin, unsigned int blink_time)
{
    digitalWrite(led_pin, HIGH);
    delay(blink_time/2);
    digitalWrite(led_pin, LOW);
    delay(blink_time/2);
}

void LedStatus_Light(char led_pin)
{
    digitalWrite(led_pin, HIGH);
}

void LedStatus_Quench(char led_pin)
{
    digitalWrite(led_pin, LOW);
}

void LedStatus_Switch(char led_pin)
{
    digitalWrite(led_pin, !digitalRead(led_pin));
}