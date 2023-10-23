#include <Arduino.h>

#include <esp_efuse.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <Preferences.h>

#include "LedStatus.h"

#include "SetWifi.h"

#include "SetWiFi_html.h"


const char* AP_NAME = "QwQ WiFi _";  //Web配网模式下的AP-wifi名字
String PrefSSID, PrefPassword, cityCode; 

//暂时存储wifi账号密码
char sta_ssid[32] = {0};
char sta_password[64] = {0};
char sta_citycode[32] = {0};


const byte DNS_PORT = 53;//DNS端口号
IPAddress apIP(192, 168, 4, 1);//esp32-AP-IP地址
DNSServer dnsServer;//创建dnsServer实例
WebServer server(80);//创建WebServer


unsigned long lastGetTime_SetWiFi = 0;

void handleRoot() {//访问主页回调函数
  lastGetTime_SetWiFi = millis();  // 更新最后连接时间
  LedStatus_Light(wifi_led);
  Serial.print("lastGetTime_SetWiFi: ");
  Serial.println(lastGetTime_SetWiFi);

  server.send(200, "text/html", wifi_page_html);
}

void initSoftAP(void){//初始化AP模式
//把mac拼接到ap_name后
  uint8_t mac[6];
  WiFi.macAddress(mac);
  char macStr[18];
  sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.print("MAC Address: ");
  Serial.println(macStr);
  char wifiName[32];
  sprintf(wifiName, "%s%s", AP_NAME, macStr);

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  Serial.println(WiFi.softAPIP());
//    Serial.print("本地IP： ");
//  Serial.println(WiFi.localIP());
  if(WiFi.softAP(wifiName)){
    Serial.println("ESP32 SoftAP for setWiFi is right");
  }
}

void initDNS(void){//初始化DNS服务器
  if(dnsServer.start(DNS_PORT, "*", apIP)){//判断将所有地址映射到esp8266的ip上是否成功
    Serial.println("start dnsserver success.");
  }
  else Serial.println("start dnsserver failed.");
}

unsigned long lastPostTime_SetWiFi = 0;

void handleRootPost() {//Post回调函数
  Serial.println("void handleRootPost()");
  lastPostTime_SetWiFi = millis();
  LedStatus_Light(wifi_led);
  Serial.println("lastPostTime_SetWiFi: ");
  Serial.println(lastPostTime_SetWiFi);

String wifiid="",wifipass="",cityid="";
  Serial.println("handleRootPost");
  if (server.hasArg("ssid")) {//判断是否有账号参数
    Serial.print("got ssid:");
    strcpy(sta_ssid, server.arg("ssid").c_str());//将账号参数拷贝到sta_ssid中
    Serial.println(sta_ssid);
  } else {//没有参数
    Serial.println("error, not found ssid");
    server.send(200, "text/html", "<meta charset='UTF-8'>提示：请输入WiFi名称");//返回错误页面
    return;
  }
  //密码与账号同理
  if (server.hasArg("password")) {
    Serial.print("got password:");
    strcpy(sta_password, server.arg("password").c_str());
    Serial.println(sta_password);
  } else {
    Serial.println("error, not found password");
    server.send(200, "text/html", "<meta charset='UTF-8'>提示：请输入WiFi密码");
    return;
  }


  // if (server.hasArg("citycode")) {
  //   Serial.print("got citycode:");
  //   strcpy(sta_citycode, server.arg("citycode").c_str());
  //   Serial.println(sta_citycode);
  // } else {
  //   Serial.println("error, not found citycode");
  //   server.send(200, "text/html", "<meta charset='UTF-8'>提示：请输入城市代码");
  //   return;
  // }
    Preferences prefs; 
  prefs.begin("wifi");
  wifiid=sta_ssid;wifipass=sta_password;cityid=sta_citycode;
  prefs.putString( "ssid" ,wifiid);
  prefs.putString( "password", wifipass);
  prefs.putString( "citycode", cityid);
  prefs.end();

  server.send(200, "text/html", wifi_ok_page_html);//返回保存成功页面
  delay(3500);
  //连接wifi
  //connectNewWifi();

  Serial.println("ESP32 restart...");
  ESP.restart(); //重启ESP32
}

void initWebServer(void){//初始化WebServer
  //server.on("/",handleRoot);
  //上面那行必须以下面这种格式去写否则无法强制门户
  server.on("/", HTTP_GET, handleRoot);//设置主页回调函数
  server.onNotFound(handleRoot);//设置无法响应的http请求的回调函数
  server.on("/", HTTP_POST, handleRootPost);//设置Post请求回调函数
  server.begin();//启动WebServer
  Serial.println("WebServer started!");
}


void connectNewWifi(void){
Preferences prefs; 
  prefs.begin("wifi");
    if(prefs.isKey("ssid")) 
      PrefSSID =  prefs.getString("ssid");
      Serial.println(PrefSSID);
    if(prefs.isKey("password")) 
      PrefPassword =  prefs.getString("password");
      Serial.println(PrefPassword);
    if(prefs.isKey("citycode")) 
      cityCode = prefs.getString("citycode");
      Serial.println(cityCode);
  prefs.end();

  WiFi.mode(WIFI_STA);//切换为STA模式
  WiFi.begin(PrefSSID.c_str(), PrefPassword.c_str());

  WiFi.setAutoConnect(true);//设置自动连接
  //  WiFi.begin(PrefSSID.c_str(), PrefPassword.c_str());//连接上一次连接成功的wifi
  Serial.println("");
  Serial.print("Connect to wifi");
  int count = 0;
   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    count++;
    if(count > 20){//如果10秒内（计数20次）没有连上，就开启Web配网 可适当调整这个时间
      // initSoftAP();
      // initWebServer();
      // initDNS();
      // break;//跳出 防止无限初始化
      setWiFi();
    }
    Serial.print(".");
  }
  Serial.println("");
  if(WiFi.status() == WL_CONNECTED){
    Serial.println("WIFI Connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    server.stop();
  }
}

unsigned long lastSetRunTime_SetWiFi = 0;

bool setWiFi_Flag = false;
void setWiFi()
{
  initSoftAP();
  // Serial.println(PrefSSID+3);
  initWebServer();
  // Serial.println(PrefSSID+4);
  initDNS();
  // Serial.println(PrefSSID+5);
  Serial.println("into void setWiFi()");
  lastGetTime_SetWiFi = millis();

  while (setWiFi_Flag == false)
  {
    server.handleClient();
    // Serial.println(PrefSSID+6);
    dnsServer.processNextRequest();
    // Serial.println(PrefSSID+7);

    //配置页面未超时自动重启，防止连接不上导致卡在配网页面
    lastSetRunTime_SetWiFi = millis();
    Serial.print("lastConnectionTime_SetWiFi-interval : ");
    Serial.println(lastSetRunTime_SetWiFi - lastPostTime_SetWiFi);
    LedStatus_Switch(wifi_led);
    if(lastSetRunTime_SetWiFi - lastPostTime_SetWiFi > (1000*60)*3)  //1000ms = 1s 一万毫秒等于一秒
    {
      Serial.println("Start to Reboot.");
      ESP.restart();    //重启复位esp32
      Serial.println("RebootED !");
    }

    // while(1)
    // {
    //   Serial.println("wuhuwuhu------");
    // }

    if (WiFi.status() == WL_CONNECTED)
    {
      // Serial.println(PrefSSID+8);
      server.stop();
      // Serial.println(PrefSSID+9);
      setWiFi_Flag = true;
    }
  }
  // Serial.println(PrefSSID+10);
}
//删除保存的wifi信息
void DeleteWiFi(){
  Preferences prefs;
    prefs.begin("wifi",false);//为false才能删除键值
    Serial.println(prefs.freeEntries());//查询清除前的剩余空间
    prefs.remove("ssid"); // 删除当前命名空间中键名为"ssid"的元素
    prefs.remove("password"); // 删除当前命名空间中键名为"ssid"的元素
    prefs.remove("citycode"); /////////////////////////////////////////////////////
    prefs.clear();
  delay(500);
  Serial.println(prefs.freeEntries());//查询清除后的剩余空间
  prefs.end();
  esp_wifi_restore();  //删除保存的wifi信息
  Serial.println("连接信息已清空，准备重启设备..");
}