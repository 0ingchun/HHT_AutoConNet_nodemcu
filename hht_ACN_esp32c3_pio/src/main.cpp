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


void HHT_Connect()
{
  if (WiFi.status() == WL_CONNECTED) {

  //WiFiClient c;
	HTTPClient http;
	http.begin(followerUrl); //HTTP begin
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
          payload = "{\"domain\":\"" + hht_domain +"\",\"username\":\"" + hht_username +"\",\"password\":\"" + hht_password + "\"}";

      Serial.println(payload);

      int httpResponseCode = http.POST(payload);
      if (httpResponseCode == HTTP_CODE_OK) {
        String response = http.getString();
        if (response.indexOf("\"reply_code\": 0") != -1) {
          Serial.println("登录成功");
        }
        else {
          Serial.println("登录失败");
        }
      }
      else {
        Serial.println("HTTP request failed");
      }
		}
	}
	else
	{
		Serial.printf("HTTP Get Error: %s\n", http.errorToString(httpCode).c_str());
	}
    http.end();
  }
}

void Web_SetWifi_setup()
{
  // Serial.begin(115200);
  //delay(10);
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

void setup() {
  Serial.begin(115200);
  delay(10);
  Web_SetWifi_setup();
  //WiFi_Connect();
}

void loop() {
  while (1)
  {

    Web_SetWifi_loop();
    HHT_Connect();
    delay(5000); // seconds delay

    // payload = "domain=telecom&username=ffffff&password=ffffff";
    // Serial.println(payload);
    payload = "{\"domain\":\"" + hht_domain +"\",\"username\":\"" + hht_username +"\",\"password\":\"" + hht_password + "\"}";
    Serial.println(payload);

  }
}
