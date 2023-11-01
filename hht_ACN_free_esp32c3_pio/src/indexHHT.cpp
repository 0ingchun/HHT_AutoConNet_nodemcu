#include <Arduino.h>

#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_efuse.h>

#include <DNSServer.h>
#include <WebServer.h>
#include <Preferences.h>
#include "HTTPClient.h"

#include "SetWifi.h"
#include "LedStatus.h"

#include "indexHHT.h"
#include "SetHHT.h"
#include "freeHHT.h"

#include "indexHHT_html.h"


const char* indexhht_AP_NAME = "OnO HHT _";//Web配网模式下的AP-hht名字
String Pref_indexHHT_Username, Pref_indexHHT_Password, Pref_indexHHT_Domain, Pref_indexHHT_Interval ,Pref_indexHHT_FollowerUrl;

//暂时存储hht账号密码

char sta_indexhht_followerUrl[64] = {0};
char sta_indexhht_domain[32] = {0};
char sta_indexhht_username[32] = {0};
char sta_indexhht_password[32] = {0};
char sta_indexhht_interval[16] = {0};

String indexhht_followerUrl = "http://10.10.16.12/api/portal/v1/login";
String indexhht_domain = "default";
String indexhht_username = "12345678900";
String indexhht_password = "123321";
String indexhht_interval = "6.00";


const byte indexhht_DNS_PORT = 53;//DNS端口号
IPAddress indexhht_apIP(192, 168, 4, 1);//esp32-AP-IP地址
DNSServer indexhht_dnsServer;//创建dnsServer实例

WebServer indexhht_server(80);//创建WebServer


unsigned long lastGetTime_SetindexHHT = 0;


bool setindexHHT_Flag = false;
bool login_indexHHT_Flag = false;

void indexhht_handleRoot() {//访问主页回调函数
  lastGetTime_SetindexHHT = millis();  // 更新最后连接时间
  LedStatus_Light(hht_led);
  Serial.print("lastGetTime_SetindexHHT: ");
  Serial.println(lastGetTime_SetindexHHT);

  indexhht_server.send(200, "text/html", indexhht_page_html);
}

void indexhht_initSoftAP(void) {//初始化AP模式
//把mac拼接到ap_name后
  uint8_t mac[6];
  WiFi.macAddress(mac);
  char macStr[18];
  sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.print("MAC Address: ");
  Serial.println(macStr);
  char wifiName[32];
  sprintf(wifiName, "%s%s", indexhht_AP_NAME, macStr);

    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(indexhht_apIP, indexhht_apIP, IPAddress(255, 255, 255, 0));
    Serial.println(WiFi.softAPIP());
    //  Serial.print("本地IP： ");
    //  Serial.println(WiFi.localIP());
    if(WiFi.softAP(wifiName)){
        Serial.println("ESP32 SoftAP for indexHHTset is right");
    }
}

void indexhht_initDNS(void){//初始化DNS服务器
    if(indexhht_dnsServer.start(indexhht_DNS_PORT, "*", indexhht_apIP)){//判断将所有地址映射到esp8266的ip上是否成功
        Serial.println("start dnsserver success.");
    }
    else Serial.println("start dnsserver failed.");
}

unsigned long lastPostTime_SetindexHHT = 0;

void indexhht_handleRootPost() {//Post回调函数
    Serial.println("void indexhht_handleRootPost()");
    lastPostTime_SetindexHHT = millis();
    LedStatus_Light(hht_led);
    Serial.println("lastPostTime_SetindexHHT: ");
    Serial.println(lastPostTime_SetindexHHT);

    Serial.println("indexhht_handleRootPost");


    indexhht_server.send(200, "text/html", indexhht_ok_page_html);//返回保存成功页面
    delay(3500);
    //连接wifi
    //connectNewWifi();


//   Serial.println("ESP32 restart...");
//   ESP.restart(); //重启ESP32
}

void goTo_setHHT() {
  // 在此处执行函数1的代码
  indexhht_server.stop();
  Serial.println("goTo_setHHT()");
  setHHT();
}

void goTo_freeHHT() {
  // 在此处执行函数2的代码
  indexhht_server.stop();
  Serial.println("goTo_freeHHT()");
  setfreeHHT();
}

void indexhht_initWebServer(void){//初始化WebServer
  //server.on("/",handleRoot);
  //上面那行必须以下面这种格式去写否则无法强制门户
    indexhht_server.on("/", HTTP_GET, indexhht_handleRoot);//设置主页回调函数
    indexhht_server.on("/", HTTP_POST, indexhht_handleRootPost);//设置Post请求回调函数
    indexhht_server.on("/goTo_setHHT", HTTP_GET, goTo_setHHT);
    indexhht_server.on("/goTo_freeHHT", HTTP_GET, goTo_freeHHT);
    indexhht_server.onNotFound(indexhht_handleRoot);//设置无法响应的http请求的回调函数
    indexhht_server.begin();//启动WebServer
    Serial.println("indexhht_WebServer started!");
}

unsigned long lastSetRunTime_SetindexHHT = 0;

void setindexHHT()
{
  Serial.println("void setindexHHT()");

  indexhht_initSoftAP();
  indexhht_initWebServer();
  indexhht_initDNS();
  Serial.println("into void setindexHHT()");

  while (setindexHHT_Flag == false)
  {
      indexhht_server.handleClient();
      indexhht_dnsServer.processNextRequest();

      //配置页面未超时自动重启，防止连接不上导致卡在配网页面
      lastSetRunTime_SetindexHHT = millis();
      Serial.print("lastConnectionTime_SetindexHHT-interval : ");
      Serial.println(lastSetRunTime_SetindexHHT - lastPostTime_SetindexHHT);
      LedStatus_Switch(hht_led);
      if(lastSetRunTime_SetindexHHT - lastPostTime_SetindexHHT > (1000*60)*3)  //1000ms = 1s 一万毫秒等于一秒
      {
        Serial.println("Start to Reboot.");
        ESP.restart();    //重启复位esp32
        Serial.println("RebootED !");
      }

      if (login_indexHHT_Flag == true)
      {
          indexhht_server.stop();
          setindexHHT_Flag = true;
      }
  }
}

