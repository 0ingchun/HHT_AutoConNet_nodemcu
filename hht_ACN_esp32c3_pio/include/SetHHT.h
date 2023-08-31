#pragma once

#ifndef _HHT_SET_H_
#define _HHT_SET_H_

// #include <Arduino.h>

#include <WiFi.h>
#include <esp_wifi.h>

#include <DNSServer.h>
#include <WebServer.h>
#include <Preferences.h>
#include "HTTPClient.h"

#include "SetWifi.h"

extern String Pref_HHT_Username, Pref_HHT_Password, Pref_HHT_Domain, Pref_HHT_Interval ,Pref_HHT_FollowerUrl;
extern String hht_username, hht_password, hht_domain, hht_followerUrl ,hht_interval;

// extern bool setHHT_Flag_new;
extern bool setHHT_Flag;
extern bool login_HHT_Flag;
extern String payload;

void hht_handleRoot();  //访问主页回调函数

void hht_initSoftAP(void);  //初始化AP模式

void hht_initDNS(void);

void hht_handleRootPost();  //Post回调函数

void hht_initWebServer(void);

void connectNewHHT(void);

void setHHT();

void DeleteHHT();

void HHT_Connect(String hht_domain, String hht_username, String hht_password, String hht_followerUrl, bool* p_login_HHT_Flag);

void HHT_Connect_Hard(String hht_domain, String hht_username, String hht_password, String hht_followerUrl, bool* p_login_HHT_Flag);

// void setHHT_new();

#endif