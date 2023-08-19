#include "main.h"

/*
惠湖通 自动网关 脚本

更改 Payload 中的 domain username 和 password
domain 有 cmcc unicom telecom default 
（ cmcc（中國移動），unicom（中國聯通），telecom（中國電信），default（默認） ）

License:MIT
*/


const char* wifi_ssid = "keke";
const char* wifi_password = "qp10al29zm38keke";

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


String followerUrl = "http://10.10.16.12/api/portal/v1/login";
String hht_domain = "default";
String hht_username = "12345678900";
String hht_password = "123321";

String payload;

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



void setup() {
  Serial.begin(115200);
  delay(10);
  WiFi_Connect();
}

void loop() {
  while (1)
  {
    HHT_Connect();
    delay(5000); // seconds delay

    // payload = "domain=telecom&username=ffffff&password=ffffff";
    // Serial.println(payload);
    payload = "{\"domain\":\"" + hht_domain +"\",\"username\":\"" + hht_username +"\",\"password\":\"" + hht_password + "\"}";
    Serial.println(payload);

  }
}
