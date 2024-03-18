#pragma once

#ifndef _HHT_FREE_H_
#define _HHT_FREE_H_

// #include <Arduino.h>

// #include <WiFi.h>
// #include <esp_wifi.h>
// #include <esp_efuse.h>

// #include <DNSServer.h>
// #include <WebServer.h>
// #include <Preferences.h>
// #include "HTTPClient.h"

// #include "SetWifi.h"

extern String Pref_freeHHT_Username, Pref_freeHHT_Password, Pref_freeHHT_Domain, Pref_freeHHT_Interval ,Pref_freeHHT_FollowerUrl;
extern String freehht_username, freehht_password, freehht_domain, freehht_followerUrl ,freehht_interval;

// extern bool setHHT_Flag_new;
extern bool setHHT_Flag;
extern bool login_HHT_Flag;
extern String payload;

typedef struct numNode_t {
    String num_str;
    struct numNode_t *prev;
    struct numNode_t *next;
} numNode_s;

String generate_freeurl(void);

void freehht_handleRoot(void);  //访问主页回调函数

void freehht_initSoftAP(void);  //初始化AP模式

void freehht_initDNS(void);

void freehht_handleRootPost();  //Post回调函数

void freehht_initWebServer(void);

void connectNewfreeHHT(void);

void setfreeHHT();

void DeletefreeHHT();

void test_freeHHT(String full_number, String default_passwd[], String hhtinterval);

String freeHHT_create_phNUM(const char* num);

void freeHHT_produce_phNUM(String phone_num_str);


#endif