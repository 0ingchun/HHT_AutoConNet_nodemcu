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

#include "SetHHT.h"

#include "SetHHT_html.h"


const char* hht_AP_NAME = "AwA HHT _";//Web配网模式下的AP-hht名字
String Pref_HHT_Username, Pref_HHT_Password, Pref_HHT_Domain, Pref_HHT_Interval ,Pref_HHT_FollowerUrl;

//暂时存储hht账号密码

char sta_hht_followerUrl[64] = {0};
char sta_hht_domain[32] = {0};
char sta_hht_username[32] = {0};
char sta_hht_password[32] = {0};
char sta_hht_interval[16] = {0};

String hht_followerUrl = "http://10.10.16.12/api/portal/v1/login";
String hht_domain = "default";
String hht_username = "12345678900";
String hht_password = "123321";
String hht_interval = "6.00";


String generate_url(void){
  Serial.println("void generate_url()");

  // randomSeed(millis());
  // char url_switch_flag = random(0, 1);
  String s_testUrl = "http://www.baidu.com";
      // if (url_switch_flag = 0)
      // {
      //   s_testUrl = "http://www.baidu.com";
      // }
      // else if (url_switch_flag = 1)
      // {
      //   s_testUrl = "http://www.bilibili.com";
      // }
      // else if (url_switch_flag = 2)
      // {
      //   s_testUrl = "http://www.bilibili.com";
      // }
  return s_testUrl;
}

const byte hht_DNS_PORT = 53;//DNS端口号
IPAddress hht_apIP(192, 168, 4, 1);//esp32-AP-IP地址
DNSServer hht_dnsServer;//创建dnsServer实例

WebServer hht_server(80);//创建WebServer


unsigned long lastGetTime_SetHHT = 0;

void hht_handleRoot() {//访问主页回调函数
  lastGetTime_SetHHT = millis();  // 更新最后连接时间
  LedStatus_Light(hht_led);
  Serial.print("lastGetTime_SetHHT: ");
  Serial.println(lastGetTime_SetHHT);

  hht_server.send(200, "text/html", hht_page_html);
}

void hht_initSoftAP(void) {//初始化AP模式
//把mac拼接到ap_name后
  uint8_t mac[6];
  WiFi.macAddress(mac);
  char macStr[18];
  sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.print("MAC Address: ");
  Serial.println(macStr);
  char wifiName[32];
  sprintf(wifiName, "%s%s", hht_AP_NAME, macStr);

    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(hht_apIP, hht_apIP, IPAddress(255, 255, 255, 0));
    Serial.println(WiFi.softAPIP());
    //  Serial.print("本地IP： ");
    //  Serial.println(WiFi.localIP());
    if(WiFi.softAP(wifiName)){
        Serial.println("ESP32 SoftAP for HHTset is right");
    }
}

void hht_initDNS(void){//初始化DNS服务器
    if(hht_dnsServer.start(hht_DNS_PORT, "*", hht_apIP)){//判断将所有地址映射到esp8266的ip上是否成功
        Serial.println("start dnsserver success.");
    }
    else Serial.println("start dnsserver failed.");
}

unsigned long lastPostTime_SetHHT = 0;

void hht_handleRootPost() {//Post回调函数
    Serial.println("void hht_handleRootPost()");
    lastPostTime_SetHHT = millis();
    LedStatus_Light(hht_led);
    Serial.println("lastPostTime_SetHHT: ");
    Serial.println(lastPostTime_SetHHT);

    String hhtuser="",hhtpass="",hhtdomain="",hhtfollowerUrl="",hhtinterval="";
    Serial.println("hht_handleRootPost");


    if (hht_server.hasArg("username")) {//判断是否有账号参数
      Serial.print("got username:");
      strcpy(sta_hht_username, hht_server.arg("username").c_str());//将账号参数拷贝到sta_ssid中
      Serial.println(sta_hht_username);
    } 
    else {//没有参数
      Serial.println("error, not found username");
      hht_server.send(200, "text/html", "<meta charset='UTF-8'>错误：请输入hht用户名");//返回错误页面
      return;
    }

    //密码与账号同理
    if (hht_server.hasArg("password")) {
      Serial.print("got password:");
      strcpy(sta_hht_password, hht_server.arg("password").c_str());
      Serial.println(sta_hht_password);
    } 
    else {
      Serial.println("error, not found password");
      hht_server.send(200, "text/html", "<meta charset='UTF-8'>错误：请输入hht密码");
      return;
    }


    if (hht_server.hasArg("domain")) {
      Serial.print("got domain:");
      strcpy(sta_hht_domain, hht_server.arg("domain").c_str());
      Serial.println(sta_hht_domain);
    }  
    else if (sta_hht_domain != "cmcc" && sta_hht_domain != "unicom" && sta_hht_domain != "telecom") {
      Serial.println("error, not found domain");
      hht_server.send(200, "text/html", "<meta charset='UTF-8'>错误：请参考页末提示输入运营商代码");
      return;
    } 
    else {
      Serial.println("error, not found domain");
      hht_server.send(200, "text/html", "<meta charset='UTF-8'>错误：运营商代码为空");
      return;
    }

    if (hht_server.hasArg("interval")) {
      Serial.print("got interval:");
      strcpy(sta_hht_interval, hht_server.arg("interval").c_str());
      Serial.println(sta_hht_interval);
    } 
    else if ( String(sta_hht_interval).toInt() >= 24) {
      Serial.println("error, interval >= 24 hours");
      hht_server.send(200, "text/html", "<meta charset='UTF-8'>错误：自动重连时间(hour)不得大于24");
      return;
    } 
    else {
      Serial.println("error, not found interval");
      hht_server.send(200, "text/html", "<meta charset='UTF-8'>错误：请输入自动重连时间");
      return;
    }


    Preferences prefs; 
    prefs.begin("hht");
    hhtuser = sta_hht_username; hhtpass = sta_hht_password; hhtdomain = sta_hht_domain; hhtinterval = sta_hht_interval;
    prefs.putString( "hht_username" ,hhtuser);
    prefs.putString( "hht_password", hhtpass);
    prefs.putString( "hht_domain", hhtdomain);
    prefs.putString( "hht_interval", hhtinterval);
    prefs.putString( "hht_followerUrl", hht_followerUrl);

    prefs.end();
    Serial.println("Get HHT_PREFerences Success!");

    hht_server.send(200, "text/html", hht_ok_page_html);//返回保存成功页面
    delay(3500);
    //连接wifi
    //connectNewWifi();

  Serial.println("ESP32 restart...");
  ESP.restart(); //重启ESP32
}

void hht_initWebServer(void){//初始化WebServer
  //server.on("/",handleRoot);
  //上面那行必须以下面这种格式去写否则无法强制门户
    hht_server.on("/", HTTP_GET, hht_handleRoot);//设置主页回调函数
    hht_server.onNotFound(hht_handleRoot);//设置无法响应的http请求的回调函数
    hht_server.on("/", HTTP_POST, hht_handleRootPost);//设置Post请求回调函数
    hht_server.begin();//启动WebServer
    Serial.println("hht_WebServer started!");
}

void connectNewHHT(void){
    Serial.println("void connectNewHHT()");

    Preferences prefs;
  prefs.begin("hht");
  if (prefs.isKey("hht_username"),"nano")
    Pref_HHT_Username = prefs.getString("hht_username","nano");//如果键值为空，返回0
  if (prefs.isKey("hht_password"))
    Pref_HHT_Password = prefs.getString("hht_password");
  if (prefs.isKey("hht_domain"))
    Pref_HHT_Domain = prefs.getString("hht_domain");
  if (prefs.isKey("hht_interval"))
    Pref_HHT_Interval = prefs.getString("hht_interval");
  prefs.end();

// Preferences prefs;
//     prefs.begin("hht");

//     if(prefs.isKey("hht_usename"))
//     Pref_HHT_Username =  prefs.getString("hht_username");
//     Serial.println(Pref_HHT_Username);

//     if(prefs.isKey("hht_password"))
//     Pref_HHT_Password =  prefs.getString("hht_password");
//     Serial.println(Pref_HHT_Password);

//     if(prefs.isKey("hht_domain"))
//     Pref_HHT_Domain = prefs.getString("hht_domain");
//     Serial.println(Pref_HHT_Domain);

//     if(prefs.isKey("hht_interval"))
//     Pref_HHT_Interval = prefs.getString("hht_interval");
//     Serial.println(Pref_HHT_Interval);

//     if(prefs.isKey("hht_followerurl"))
//     Pref_HHT_FollowerUrl = prefs.getString("hht_followerurl");
//     Serial.println(Pref_HHT_FollowerUrl);

//     prefs.end();

    //  WiFi.mode(WIFI_STA);
    //  WiFi.begin(Pref_HHT_Username.c_str(), Pref_HHT_Password.c_str());

    WiFi.mode(WIFI_STA);//切换为STA模式
    WiFi.begin(PrefSSID.c_str(), PrefPassword.c_str());//连接上一次连接成功的wifi
    WiFi.setAutoConnect(true);//设置自动连接

    Serial.println("");
    Serial.print("Connect to wifi because of setHHT.");

    int count = 0;
    while (setHHT_Flag == false) {
        delay(500);
        count++;
        if(count > 20){//如果10秒内（计数20次）没有连上，就开启Web配网 可适当调整这个时间
            setWiFi();
        }
        Serial.print(".!");
    }
    Serial.println("");

    Pref_HHT_FollowerUrl = "http://10.10.16.12/api/portal/v1/login";

    hht_username = Pref_HHT_Username.c_str();
    hht_password = Pref_HHT_Password.c_str();
    hht_domain = Pref_HHT_Domain.c_str();
    hht_followerUrl = Pref_HHT_FollowerUrl.c_str();
    //HHT_Connect(Pref_HHT_Username.c_str(), Pref_HHT_Password.c_str(), Pref_HHT_Domain.c_str(), Pref_HHT_FollowerUrl.c_str());

    // if(WiFi.status() == WL_CONNECTED){
    //     Serial.println("HHT Connected!");
    //     Serial.print("IP address: ");
    //     Serial.println(WiFi.localIP());
    //     hht_server.stop();
    // }

    HHT_Connect_loop();
    // HHT_Connect(hht_domain, hht_username, hht_password, hht_followerUrl, &setHHT_Flag);

}

unsigned long lastSetRunTime_SetHHT = 0;

bool setHHT_Flag = false;
void setHHT()
{
  Serial.println("void setHHT()");

  hht_initSoftAP();
  hht_initWebServer();
  hht_initDNS();
  Serial.println("into void setHHT()");

  while (setHHT_Flag == false)
  {
      hht_server.handleClient();
      hht_dnsServer.processNextRequest();

      //配置页面未超时自动重启，防止连接不上导致卡在配网页面
      lastSetRunTime_SetHHT = millis();
      Serial.print("lastConnectionTime_SetHHT-interval : ");
      Serial.println(lastSetRunTime_SetHHT - lastPostTime_SetHHT);
      LedStatus_Switch(hht_led);
      if(lastSetRunTime_SetHHT - lastPostTime_SetHHT > (1000*60)*3)  //1000ms = 1s 一万毫秒等于一秒
      {
        Serial.println("Start to Reboot.");
        ESP.restart();    //重启复位esp32
        Serial.println("RebootED !");
      }

      if (login_HHT_Flag == true)
      {
          hht_server.stop();
          setHHT_Flag = true;
      }
  }
}

//删除保存的hht信息
void DeleteHHT() {
    Serial.println("DeleteHHT()");
    Preferences prefs;
    prefs.begin("hht",false);//为false才能删除键值
    Serial.println(prefs.freeEntries());//查询清除前的剩余空间
    prefs.remove("hht_followerurl");
    prefs.remove("hht_username");
    prefs.remove("hht_password"); // 删除当前命名空间中键名为"passwd"的元素
    prefs.remove("hht_domain"); // 删除当前命名空间中键名为"ssid"的元素
    prefs.remove("hht_interval");
    prefs.clear();
    delay(500);
    Serial.println(prefs.freeEntries());//查询清除后的剩余空间
    prefs.end();
    //esp_wifi_restore();  //删除保存的wifi信息
    Serial.println("HHT信息已清空,准备重启设备..");
}


//----------------------------------LOGIN_HHT-----------------------------------//


String payload = "{\"nano\":\"nano\"}";
bool login_HHT_Flag = false;
void HHT_Connect_Soft(String s_hht_domain, String s_hht_username, String s_hht_password, String s_hht_followerUrl, bool* p_login_HHT_Flag)
{
  Serial.println("void HHT_Connect_Soft()");
  if (WiFi.status() == WL_CONNECTED) {

    // WiFiClient c;
	HTTPClient http;
	http.begin(s_hht_followerUrl); //HTTP begin
  Serial.println("http try to connect: " + s_hht_followerUrl);
  // http.begin("http://10.10.16.12/api/portal/v1/login");
  //http.begin("http://www.bing.com");

	int httpCode = http.GET();

	if (httpCode > 0)
	{
		// httpCode will be negative on error
		Serial.printf("HTTP Get Code: %d\r\n", httpCode);

		if (httpCode == HTTP_CODE_OK) // 收到正确的内容
		{
      http.addHeader("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
      http.addHeader("X-Requested-With", "XMLHttpRequest");

      // String payload = "domain=telecom&username=ffffff&password=ffffff";
      // String payload = "{\"domain\":\"telecom\",\"username\":\"ffffff\",\"password\":\"ffffff\"}";
      //payload = "{\"domain\":\"telecom\",\"username\":\"ffffff\",\"password\":\"ffffff\"}";
      payload = "{\"domain\":\"" + s_hht_domain +"\",\"username\":\"" + s_hht_username +"\",\"password\":\"" + s_hht_password + "\"}";

      Serial.println("payload is OK to send:" + payload);

        // delay(50);

      int httpResponseCode = http.POST(payload);
      if (httpResponseCode == HTTP_CODE_OK) {
        String response = http.getString();

        // int count = 0;
        // while ( ! (response.indexOf("\"reply_code\": 0") != -1 ) )
        // {
        //   delay(500);
        //   count++;
        //   if(count > 20){//如果10秒内（计数20次）没有连上，就开启Web配网 可适当调整这个时间
        //     //setHHT_new();
        //   }
        //   Serial.print(".");
        // }
        
        if (response.indexOf("\"reply_code\": 0") != -1) {
          Serial.println("登录成功");
          *p_login_HHT_Flag = true;
        }
        else {
          Serial.println("登录失败");
          *p_login_HHT_Flag = false;
        }
      }
      else {
        Serial.println("HTTP request failed");
        *p_login_HHT_Flag = false;
      }
		}
	}
	else
	{
		Serial.printf("HTTP Get Error: %s\n", http.errorToString(httpCode).c_str());
    Serial.println("");
    *p_login_HHT_Flag = false;
	}
    http.end();
  }
}

void HHT_Connect_Hard(String s_hht_domain, String s_hht_username, String s_hht_password, String s_hht_followerUrl, bool* p_login_HHT_Flag)
{
  Serial.println("void HHT_Connect_Hard()");

  if (WiFi.status() == WL_CONNECTED) {

    // WiFiClient c;
	HTTPClient http;
	http.begin(s_hht_followerUrl); //HTTP begin
  Serial.println("http try to connect: " + s_hht_followerUrl);
  // http.begin("http://10.10.16.12/api/portal/v1/login");
  //http.begin("http://www.bing.com");

	int httpCode = http.GET();

	if (httpCode > 0)
	{
		// httpCode will be negative on error
		Serial.printf("HTTP Get Code: %d\r\n", httpCode);

    Serial.printf("HHT_Connect_Hard: Hardly to Connect!!!");

		// if (httpCode == HTTP_CODE_OK) // 收到正确的内容
		// {
      http.addHeader("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
      http.addHeader("X-Requested-With", "XMLHttpRequest");

      // String payload = "domain=telecom&username=ffffff&password=ffffff";
      // String payload = "{\"domain\":\"telecom\",\"username\":\"ffffff\",\"password\":\"ffffff\"}";
      //payload = "{\"domain\":\"telecom\",\"username\":\"ffffff\",\"password\":\"ffffff\"}";
      payload = "{\"domain\":\"" + s_hht_domain +"\",\"username\":\"" + s_hht_username +"\",\"password\":\"" + s_hht_password + "\"}";

      Serial.println("payload is OK to send:" + payload);

        // delay(50);

      int httpResponseCode = http.POST(payload);

      http.end();
      ///////////Connect end

      String s_testUrl = generate_url();

      http.begin(s_testUrl); //HTTP begin
      Serial.println("void HHT_Connect_Hard(): http try to connect: " + s_testUrl);
      httpResponseCode  = http.GET();
      Serial.printf("HTTP Get Code: %d\r\n", httpResponseCode);

      if (httpResponseCode == HTTP_CODE_OK) {
        String response = http.getString();

        // int count = 0;
        // while ( ! (response.indexOf("\"reply_code\": 0") != -1 ) )
        // {
        //   delay(500);
        //   count++;
        //   if(count > 20){//如果10秒内（计数20次）没有连上，就开启Web配网 可适当调整这个时间
        //     //setHHT_new();
        //   }
        //   Serial.print(".");
        // }
        
        // if (response.indexOf("\"reply_code\": 0") != -1) {
        //   Serial.println("登录成功");
        //   *p_login_HHT_Flag = true;
        // }
        // else {
        //   Serial.println("登录失败");
        //   *p_login_HHT_Flag = false;
        // }

        *p_login_HHT_Flag = true;
      }
      else {
        Serial.println("HTTP request failed");
        *p_login_HHT_Flag = false;
      }
		// }
	}
	else
	{
		Serial.printf("HTTP Get Error: %s\n", http.errorToString(httpCode).c_str());
    Serial.println("");
    *p_login_HHT_Flag = false;
	}
    http.end();
  }
}

void HHT_Connect_ping(bool* p_login_HHT_Flag)
{
  Serial.println("void HHT_Connect_ping()");

  // Serial.println("wdfwdfwdf----------------------+++++++++");
  // Serial.println(WiFi.status());

  if (WiFi.status() == WL_CONNECTED) {

    // WiFiClient c;

    // Serial.println("^^^^^^^^^^^^^^^^^^^^^^^+++++++++here!!!!!!!!!!");
    // Serial.println(*p_login_HHT_Flag);

    // byte k = 0;
    // while(*p_login_HHT_Flag == false)
    // {

      HTTPClient http;
      String s_testUrl = generate_url();
      // String s_testUrl = "http://www.baidu.com";
      http.begin(s_testUrl); //HTTP begin

      Serial.println("http try to connect GET : " + s_testUrl);
      int httpResponseCode = http.GET();
      Serial.printf("HTTP Get Code: %d\r\n", httpResponseCode);

      // Serial.println("@@@@@@@@@@@whynorun###############################!!!!!!!!!!");

      if (httpResponseCode == HTTP_CODE_OK) {
      String response = http.getString();
        *p_login_HHT_Flag = true;
        // break;
      }
      else {
        Serial.println("HTTP request failed");
        *p_login_HHT_Flag = false;
      }

      http.end();

    //   k++;
    //   Serial.print("*");

    //   if (*p_login_HHT_Flag == true || k > 3)
    //   {
    //     break;
    //   }

    //   delay(250);
    // }

    // Serial.println(k);

    Serial.println("login_HHT_Flag = " + String(login_HHT_Flag));
  }
}

void HHT_Connect_Both()
{
  Serial.println("void HHT_Connect_Both()");
  LedStatus_Switch(hht_led);
  HHT_Connect_Soft(Pref_HHT_Domain.c_str(), Pref_HHT_Username.c_str(), Pref_HHT_Password.c_str(), Pref_HHT_FollowerUrl.c_str(), &login_HHT_Flag);
  delay(100);
  LedStatus_Switch(hht_led);
  HHT_Connect_Hard(Pref_HHT_Domain.c_str(), Pref_HHT_Username.c_str(), Pref_HHT_Password.c_str(), Pref_HHT_FollowerUrl.c_str(), &login_HHT_Flag);
  delay(100);
}

void HHT_Connect_loop(){
  Serial.println("void HHT_Connect_loop()");

  byte j = 0;

  Serial.println("login_HHT_Flag = " + String(login_HHT_Flag));
  while (login_HHT_Flag == false)
  {   

    // HHT_Connect(Pref_HHT_Domain.c_str(), Pref_HHT_Username.c_str(), Pref_HHT_Password.c_str(), Pref_HHT_FollowerUrl.c_str(), &login_HHT_Flag);
    // delay(500);
    // HHT_Connect_Hard(Pref_HHT_Domain.c_str(), Pref_HHT_Username.c_str(), Pref_HHT_Password.c_str(), Pref_HHT_FollowerUrl.c_str(), &login_HHT_Flag);
    HHT_Connect_Both();
    LedStatus_Switch(hht_led);
    Serial.println("login_HHT_Flag = " + String(login_HHT_Flag));

    Serial.print("!");
    j++;
    Serial.print("loop Time : ");
    Serial.println(j);

    delay(50);
    LedStatus_Switch(hht_led);

    if (j > 5)
    {
      Serial.println("HHT Login Failed! because Timeout 。");
      setHHT();
    }
    
  }
}

void HHT_Logout() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    const char* server = "10.10.16.12";
    // const int port = 80;

    String url = "http://" + String(server) + "/api/portal/v1/logout";
    http.begin(url);

    http.addHeader("Accept", "application/json, text/javascript, */*; q=0.01");
    http.addHeader("Accept-Language", "zh-CN,zh;q=0.9");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
    http.addHeader("Connection", "keep-alive");
    http.addHeader("Origin", "http://" + String(server));
    http.addHeader("Referer", "http://" + String(server) + "/portal/");
    http.addHeader("User-Agent", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/115.0.0.0 Safari/537.36");
    http.addHeader("X-Requested-With", "XMLHttpRequest");

    String payload = "{\"domain\":\"default\"}";

    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Response received: " + response);
    } else {
      Serial.println("Error on HTTP request");
    }

    http.end();
  }
}

// bool setHHT_Flag_new = false;
// void setHHT_new()
// {
//     hht_initSoftAP();
//     hht_initWebServer();
//     hht_initDNS();
//     while (setHHT_Flag_new == false)
//     {
//         hht_server.handleClient();
//         hht_dnsServer.processNextRequest();

//         connectNewHHT();
//               Serial.println("setHHT_Flag_new = " + setHHT_Flag_new);

//         while (setHHT_Flag_new == true)
//         {
//             Serial.println("hht connect ok");
//             hht_server.stop();
//             // setHHT_Flag_new = true;
//         }
//         // else setHHT_Flag_new = false;
//     }
// }