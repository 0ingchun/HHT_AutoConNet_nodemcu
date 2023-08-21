#pragma once

#ifndef _GPIOPOS_H_
#define _GPIOPOS_H_

#include <Arduino.h>

#ifndef _GPIO_PIN_S_
#define _GPIO_PIN_S_

//#define LED_BUILTIN_PIN  (2)    //板载led灯
#define LED_STARE_PIN 4
#define LED_WORK_PIN 5
//在VSCode Platform IO里面，板子led灯需要注释掉，不然报警告，在ArduinoIDE中需要启用这个宏定义，不然找不到该定义的错误信息产生。
#define LED_BUILTIN  (2)    //板载led灯

#endif


void init_gpio_Pos_Digital(uint8_t* GPIO_PINS, bool POS_STATE);

void gpioBlinkH(uint8_t GPIO_PIN, uint32_t POS_TIMES);
void gpioBlinkL(uint8_t GPIO_PIN, uint32_t POS_TIMES);
void gpioShow(uint8_t GPIO_PIN, bool POS_STATE, uint32_t POS_TIMES);
void gpioFlip(uint8_t GPIO_PIN, uint32_t POS_TIMES);


bool gpio_Press_Keep(uint8_t GPIO_PIN, bool POS_STATE, uint32_t POS_TIMES);

#endif