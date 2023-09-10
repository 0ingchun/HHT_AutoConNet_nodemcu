#pragma once

#ifndef _WIFI_WEB_SET_H_
#define _WIFI_WEB_SET_H_

// #include <Arduino.h>

#include <esp_efuse.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <Preferences.h>

extern String PrefSSID, PrefPassword, cityCode; 

void handleRoot();  //访问主页回调函数

void initSoftAP(void);  //初始化AP模式

void initDNS(void);

void handleRootPost();  //Post回调函数

void initWebServer(void);

void connectNewWifi(void);

void setWiFi();

void DeleteWiFi();

#endif