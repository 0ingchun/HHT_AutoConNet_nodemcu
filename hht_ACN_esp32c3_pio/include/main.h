#pragma once

#include <Arduino.h>
#include <Preferences.h>
#include <WiFi.h>
#include <esp_wifi.h>
// #include <ESP8266WiFi.h>
// #include <WiFiClient.h>
#include "HTTPClient.h"
#include "SetWifi.h"
#include "SetHHT.h"

const char* wifi_ssid = "keke";
const char* wifi_password = "qp10al29zm38keke";

#define LED_STARE_PIN 4
#define LED_WORK_PIN 5
//在VSCode Platform IO里面，板子led灯需要注释掉，不然报警告，在ArduinoIDE中需要启用这个宏定义，不然找不到该定义的错误信息产生。
#define LED_BUILTIN  (2)    //板载led灯

// String PrefSSID, PrefPassword, cityCode;
// String Pref_HHT_Username, Pref_HHT_Password, Pref_HHT_Domain, Pref_HHT_FollowerUrl;


#define NTP1 "ntp1.aliyun.com"
#define NTP2 "ntp2.aliyun.com"
#define NTP3 "ntp3.aliyun.com"

unsigned long wifiTimes = 0;
bool ledState = LOW;
const unsigned long interval = 6000UL; //设置延时时间间隔
unsigned long previousMillis = 0;

//const char* ssid = "MERCURY_D268G";
//const char* password = "pba5ayzk";
//强制门户Web配网
const int resetPin = 0;                    //设置重置按键引脚,用于删除WiFi信息
// bool setWiFi_Flag = false;

String NowTime();