#include "main.h"

/*
惠湖通 自动网关 脚本

更改 Payload 中的 domain username 和 password
domain 有 cmcc unicom telecom default 
（ cmcc（中國移動），unicom（中國聯通），telecom（中國電信），default（默認） ）

License:MIT
*/


void WiFi_Connect()
{
  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

  WiFi.setAutoReconnect(true); //设置当断开连接的时候自动重连
  //WiFi.persistent(true); //该方法设置将WiFi参数保存于Flash

  Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
}

//----------------------------------WIFI--------------------------------------//

void Web_SetWifi_setup()
{
  // Serial.begin(115200);
  //delay(10);
  pinMode(LED_BUILTIN, OUTPUT); //板载led灯作为指示
  pinMode(resetPin, INPUT_PULLUP);      //按键上拉输入模式(默认高电平输入,按下时下拉接到低电平)
  reset_detect();
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
    //Serial.println(PrefSSID + "1");
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

    if (i > 20)
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

void Web_SetWifi_loop()
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
}


//-----------------------------------HHT-------------------------------------//


void Web_SetHHT_setup()
{
  // Serial.begin(115200);
  //delay(10);
  pinMode(LED_BUILTIN, OUTPUT); //板载led灯作为指示
  pinMode(resetPin, INPUT_PULLUP);      //按键上拉输入模式(默认高电平输入,按下时下拉接到低电平)
  reset_detect();
  //首次使用自动进入配网模式,读取NVS存储空间内的ssid、password和citycode
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
  if (prefs.isKey("hht_followerurl"))
    Pref_HHT_FollowerUrl = prefs.getString("hht_followerurl");

    if (!(prefs.isKey("hht_followerurl"))){
      Pref_HHT_FollowerUrl = "http://10.10.16.12/api/portal/v1/login";}
  prefs.end();  //从nvs获取到wifi信息后，关闭Preferences

  if (Pref_HHT_Username == "nano")
  {
    Serial.println("Pref_HHT_Username = nano");
    setHHT();
  }
  /*
  else{

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
  Serial.println("wifi is reconnect for set hht");
  */


    Serial.println(Pref_HHT_Username.c_str());
    Serial.println(Pref_HHT_Password.c_str());
    Serial.println(Pref_HHT_Domain.c_str());
    Serial.println(Pref_HHT_FollowerUrl.c_str());
    Serial.println(Pref_HHT_Interval.c_str());

Serial.println("-------wdf?------");

    hht_username = Pref_HHT_Username.c_str();
    hht_password = Pref_HHT_Password.c_str();
    hht_domain = Pref_HHT_Domain.c_str();
    hht_followerUrl = Pref_HHT_FollowerUrl.c_str();
    hht_interval = Pref_HHT_Interval.c_str();

  byte j = 0;
  while (login_HHT_Flag == false)
  {   

      HHT_Connect(Pref_HHT_Username.c_str(), Pref_HHT_Password.c_str(), Pref_HHT_Domain.c_str(), Pref_HHT_FollowerUrl.c_str(), &login_HHT_Flag);

      Serial.println("login_HHT_Flag = " + String(login_HHT_Flag));

    j++;
    Serial.print("。");
    delay(500);

    if (j > 10)
    {
      Serial.println("HHT Login Failed! because Timeout 。");
      setHHT();
    }
     digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); //板载led灯闪烁
  }

  Serial.println("void Web_SetHHT_setup(): HHT Login Success!");

  Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
  Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
  Serial.println(WiFi.localIP());
  configTime(8 * 3600, 0, NTP1, NTP2, NTP3);
}

void Internal_HHT_Reconnect(String s_hht_interval)
{
  Serial.print("s_hht_interval to float = ");
  Serial.println(s_hht_interval.toFloat());
  Serial.print("Pref_HHT_Interval = ");
  Serial.println(Pref_HHT_Interval.c_str());

  float interval_to_ms = s_hht_interval.toFloat();
  interval_to_ms = interval_to_ms*60*60*1000;  //小时化为毫秒

  unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval_to_ms)
    {
      
      login_HHT_Flag == false;
      byte j = 0;
      while (login_HHT_Flag == false)
      {   

        HHT_Connect(Pref_HHT_Username.c_str(), Pref_HHT_Password.c_str(), Pref_HHT_Domain.c_str(), Pref_HHT_FollowerUrl.c_str(), &login_HHT_Flag);
      
        j++;
        Serial.print("!");
        delay(1000);

        // if (j > 10)
        // {
        //   Serial.println("HHT Login Failed! because Timeout 。");
        //   setHHT();
        // }
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); //板载led灯闪烁
      }

      struct tm timeInfo; //声明一个结构体

      if (!getLocalTime(&timeInfo))
      { //一定要加这个条件判断，否则内存溢出
        Serial.println("Failed to obtain time");
      }

      Serial.println("Interval Reconnect HHT Time:");
      Serial.println(&timeInfo, "%F %T %A");
      previousMillis = currentMillis;
    }

  // float i = s_hht_interval.toFloat();
  // if(i > 0)
  // {
  //  HHT_Connect(Pref_HHT_Username.c_str(), Pref_HHT_Password.c_str(), Pref_HHT_Domain.c_str(), Pref_HHT_FollowerUrl.c_str(), &login_HHT_Flag);
  // }
}
//-----------------------------------SYSTEM-------------------------------------//

void reset_detect()
{
  if(!digitalRead(resetPin)){
    delay(2000);
    if(!digitalRead(resetPin)){ //1Kde 下来电阻，10K的拉不动  
        Serial.println("\n按键已长按3秒,正在清空NVS保存的信息.");
        DeleteHHT();
        DeleteWiFi();    //删除保存的wifi信息 
        ESP.restart();    //重启复位esp32
        Serial.println("已重启设备.");
    }      
  }
}

//------------------------------------------------------------------------//

void setup() {

  Serial.begin(115200);
  delay(10);
  Web_SetWifi_setup();
  //WiFi_Connect();

  Web_SetHHT_setup();
}

void loop() {
  while (1)
  {
    reset_detect();
    
    Web_SetWifi_loop();
    // HHT_Connect();
    // delay(3000); // seconds delay

    // payload = "domain=telecom&username=ffffff&password=ffffff";
    // Serial.println(payload);
    
    //payload = "{\"domain\":\"" + hht_domain +"\",\"username\":\"" + hht_username +"\",\"password\":\"" + hht_password + "\"}";
    Serial.println(payload);

    //setHHT_new();

    //connectNewHHT();
    //Web_SetHHT_loop();
    // delay(3000); // seconds delay
    // Serial.println(hht_interval.toFloat());
    // delay(hht_interval.toFloat());
    Internal_HHT_Reconnect(Pref_HHT_Interval.c_str());
  }
}
