#include <Arduino.h>
#include <WiFi.h>
//#include <WiFiUdp.h>
#include <HTTPClient.h>
#include <Preferences.h>
#include "SetWiFi.h" //Web配网
#include <esp_wifi.h>     //用于esp_wifi_restore() 删除保存的wifi信息

String PrefSSID, PrefPassword, cityCode;
//在VSCode Platform IO里面，板子led灯需要注释掉，不然报警告，在ArduinoIDE中需要启用这个宏定义，不然找不到该定义的错误信息产生。
#define LED_BUILTIN  (2)    //板载led灯

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
bool setWiFi_Flag = false;

String NowTime();
void setWiFi()
{
  initSoftAP();
  initWebServer();
  initDNS();
  while (setWiFi_Flag == false)
  {
    server.handleClient();
    dnsServer.processNextRequest();
    if (WiFi.status() == WL_CONNECTED)
    {
      server.stop();
      setWiFi_Flag = true;
    }
  }
}
//删除保存的wifi信息
void DeleteWiFi(){
  Preferences prefs;
    prefs.begin("wifi",false);//为false才能删除键值
    Serial.println(prefs.freeEntries());//查询清除前的剩余空间
    prefs.remove("ssid"); // 删除当前命名空间中键名为"ssid"的元素
    prefs.remove("password"); // 删除当前命名空间中键名为"ssid"的元素
    prefs.clear();
       delay(500);
       Serial.println(prefs.freeEntries());//查询清除后的剩余空间
       prefs.end();
       esp_wifi_restore();  //删除保存的wifi信息
       Serial.println("连接信息已清空,准备重启设备..");
  }
  
void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT); //板载led灯作为指示
  pinMode(resetPin, INPUT_PULLUP);      //按键上拉输入模式(默认高电平输入,按下时下拉接到低电平)
  //首次使用自动进入配网模式,读取NVS存储空间内的ssid、password和citycode
  Preferences prefs;
  prefs.begin("wifi");
  if (prefs.isKey("ssid"),"nano")
    PrefSSID = prefs.getString("ssid","nano");//如果键值为空，返回0
  
  if (prefs.isKey("password"))
    PrefPassword = prefs.getString("password");

  if (prefs.isKey("citycode"))
    cityCode = prefs.getString("citycode");

  prefs.end();//从nvs获取到wifi信息后，关闭Preferences

  if (PrefSSID == "nano")
  {
    setWiFi();
  }else{
    Serial.println(PrefSSID);
  Serial.println(PrefPassword);
    Serial.println(cityCode);
  WiFi.mode(WIFI_STA);//切换为STA模式，进行入网
  WiFi.begin(PrefSSID.c_str(), PrefPassword.c_str());
  Serial.println("正在连接" + PrefSSID + "...Connecting to WiFi...");
  Serial.println("-------------");
  }
  byte i = 0;
  while (WiFi.status() != WL_CONNECTED)
  {   
    i++;
    Serial.print('.');
    delay(500);

    if (i > 10)
    {
      setWiFi();
    }
     digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); //板载led灯闪烁
  }
  Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
  Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
  Serial.println(WiFi.localIP());
  configTime(8 * 3600, 0, NTP1, NTP2, NTP3);
}

void loop()
{

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    ledState = !ledState; //状态翻转
    digitalWrite(LED_BUILTIN, ledState);

    struct tm timeInfo; //声明一个结构体
    if (!getLocalTime(&timeInfo))
    { //一定要加这个条件判断，否则内存溢出
      Serial.println("Failed to obtain time");
    }
    Serial.println(&timeInfo, "%F %T %A");
    previousMillis = currentMillis;
  }
  if(!digitalRead(resetPin)){
        delay(3000);
        if(!digitalRead(resetPin)){ //1Kde 下来电阻，10K的拉不动  
           Serial.println("\n按键已长按3秒,正在清空NVS保存的信息.");  
           DeleteWiFi();    //删除保存的wifi信息 
           ESP.restart();    //重启复位esp32
           Serial.println("已重启设备.");
        }      
     }
}

