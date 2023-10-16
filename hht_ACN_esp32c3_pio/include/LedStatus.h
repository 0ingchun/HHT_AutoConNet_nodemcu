#pragma once

const int wifi_led = 12;
const int hht_led = 13;

void int_LedStatus();

void LedStatus_Blink(char led_pin, unsigned int blink_time);

void LedStatus_Light(char led_pin);

void LedStatus_Quench(char led_pin);

void LedStatus_Switch(char led_pin);