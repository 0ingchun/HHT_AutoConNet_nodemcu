#include "SetHHT.h"

const char* hht_AP_NAME = "ESP32_HHT_AP";//Web配网模式下的AP-hht名字
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
String hht_interval = "5.00";


//配网页面代码 
String hht_page_html = R"(
<!DOCTYPE html>
<html lang='en'>
  <head>
    <meta charset='UTF-8'>
   
    <meta name="viewport" content="width=device-width,initial-scale=1.0,maximum-scale=1.0,minimum-scale=1.0,user-scalable=no">
    <title>ESP32配置HHT页面</title>
    <style type="text/css">
      * { margin: 0; padding: 0; }
       html { height: 100%; }
       h2 {text-align: center;color: #fff;line-height: 2.2;}
       body { height: 100%; background-color: #1F6F4A; 50% 50% no-repeat; background-size: cover;}
       .dowebok { position: absolute; left: 50%; top: 30%; width: 380px; height: 500px; margin: -200px 0 0 -200px; border: 3px solid #fff; border-radius: 10px; overflow: hidden;}
       
       .form-item { position: relative; width: 360px; margin: 0 auto; padding-bottom: 20px;}
       .form-item input { width: 288px; height: 48px; padding-left: 10px; border: 1px solid #fff; border-radius: 25px; font-size: 18px; color: #fff; background-color: transparent; outline: none;}
       .send_button { width: 360px; height: 50px; border: 0; border-radius: 25px; font-size: 18px; color: #1f6f4a; outline: none; cursor: pointer; background-color: #fff; }
       
       .tip { display: none; position: absolute; left: 20px; top: 52px; font-size: 14px; color: #f50; }
       .reg-bar { width: 360px; margin: 20px auto 0; font-size: 14px; overflow: hidden;}
       .reg-bar a { color: #fff; text-decoration: none; }
       .reg-bar a:hover { text-decoration: underline; }
       .reg-bar .reg { float: left; }
       .reg-bar .forget { float: right; }
       .dowebok ::-webkit-input-placeholder { font-size: 18px; line-height: 1.4; color: #fff;}
       .dowebok :-moz-placeholder { font-size: 18px; line-height: 1.4; color: #fff;}
       .dowebok ::-moz-placeholder { font-size: 18px; line-height: 1.4; color: #fff;}
       .dowebok :-ms-input-placeholder { font-size: 18px; line-height: 1.4; color: #fff;}
        
       @media screen and (max-width: 500px) {
       * { box-sizing: border-box; }
       .dowebok { position: static; width: auto; height: auto; margin: 0 30px; border: 0; border-radius: 0; }
       .logo { margin: 50px auto; }
       .form-item { width: auto; }
       .form-item input, .form-item button, .reg-bar { width: 100%; }
       }
       
    </style>
  </head>
  
  <body>
    <div class="dowebok">
      <h2>H H T 参 数 配 置</h2>
      <form style='text-align: center;padding-top: 20px' name='input' action='/' method='POST'>  
         <div class="form-item">
          <input id="username" type="text" name='username' autocomplete="off" placeholder="HHT名称">
         </div>
         <div class="form-item">
          <input id="password" type="password" name='password' autocomplete="off" placeholder="HHT密码">
         </div>
         <div class="form-item">
          <input id="domain" type="text" name='domain' autocomplete="off" placeholder="运营商代码,留空则默认或无法登录">
         </div>
         <div class="form-item">
          <input id="interval" type="text" name='interval' autocomplete="off" placeholder="自动重连时间(单位：小时)，建议填写12">
         </div>
         <div class="form-item">
           <div id="">
            <input id="send_button" type='submit' value='保存并登录'>
           </div>
        </div>
        <div class="form-item">
          <div class="user_text">
            <br>
            <p><h3>如何获取domain：</h3></p>
              <h5>
                运营商代码由若干字母组成，请参考下表，填写错误可能导致获取不到数据而无限重启
                <br>
                cmcc（中國移動），unicom（中國聯通），telecom（中國電信），default（默認）
                <br>
                运营商代码示例：若干您是移动用户，请填写 cmcc
                <br>
                小时制自动重连示例：每12小时自动重连HHT，请填写 12
              </h5>
            </p>
          </div>
         </div>
        
      </form> 
     </div>
  </body>
</html>
)";

const byte hht_DNS_PORT = 53;//DNS端口号
IPAddress hht_apIP(192, 168, 4, 1);//esp32-AP-IP地址
DNSServer hht_dnsServer;//创建dnsServer实例

WebServer hht_server(80);//创建WebServer

void hht_handleRoot() {//访问主页回调函数
  hht_server.send(200, "text/html", hht_page_html);
}

void hht_initSoftAP(void) {//初始化AP模式
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(hht_apIP, hht_apIP, IPAddress(255, 255, 255, 0));
    Serial.println(WiFi.softAPIP());
    //  Serial.print("本地IP： ");
    //  Serial.println(WiFi.localIP());
    if(WiFi.softAP(hht_AP_NAME)){
        Serial.println("ESP32 SoftAP for HHTset is right");
    }
}

void hht_initDNS(void){//初始化DNS服务器
    if(hht_dnsServer.start(hht_DNS_PORT, "*", hht_apIP)){//判断将所有地址映射到esp8266的ip上是否成功
        Serial.println("start dnsserver success.");
    }
    else Serial.println("start dnsserver failed.");
}

void hht_handleRootPost() {//Post回调函数
    String hhtuser="",hhtpass="",hhtdomain="",hhtfollowerUrl="",hhtinterval="";
    Serial.println("hht_handleRootPost");


    if (hht_server.hasArg("username")) {//判断是否有账号参数
    Serial.print("got username:");
    strcpy(sta_hht_username, hht_server.arg("username").c_str());//将账号参数拷贝到sta_ssid中
    Serial.println(sta_hht_username);
    } else {//没有参数
    Serial.println("error, not found username");
    hht_server.send(200, "text/html", "<meta charset='UTF-8'>提示：请输入hht用户名");//返回错误页面
    return;
    }

    //密码与账号同理
    if (hht_server.hasArg("password")) {
    Serial.print("got password:");
    strcpy(sta_hht_password, hht_server.arg("password").c_str());
    Serial.println(sta_hht_password);
    } else {
    Serial.println("error, not found password");
    hht_server.send(200, "text/html", "<meta charset='UTF-8'>提示：请输入hht密码");
    return;
    }


    if (hht_server.hasArg("domain")) {
    Serial.print("got domain:");
    strcpy(sta_hht_domain, hht_server.arg("domain").c_str());
    Serial.println(sta_hht_domain);
    } else {
    Serial.println("error, not found domain");
    hht_server.send(200, "text/html", "<meta charset='UTF-8'>提示：请输入运营商代码");
    return;
    }

    if (hht_server.hasArg("interval")) {
    Serial.print("got domain:");
    strcpy(sta_hht_interval, hht_server.arg("interval").c_str());
    Serial.println(sta_hht_interval);
    } else {
    Serial.println("error, not found interval");
    hht_server.send(200, "text/html", "<meta charset='UTF-8'>提示：请输入自动重连时间");
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

    hht_server.send(200, "text/html", "<meta charset='UTF-8'><h1>保存HHT成功，ESP32重启中...</h1>");//返回保存成功页面
    delay(2000);
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
    HHT_Connect(hht_domain, hht_username, hht_password, hht_followerUrl, &setHHT_Flag);

}


bool setHHT_Flag = false;
void setHHT()
{
    hht_initSoftAP();
    hht_initWebServer();
    hht_initDNS();
    while (setHHT_Flag == false)
    {
        hht_server.handleClient();
        hht_dnsServer.processNextRequest();
        if (login_HHT_Flag == true)
        {
            hht_server.stop();
            setHHT_Flag = true;
        }
    }
}

//删除保存的hht信息
void DeleteHHT() {
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
void HHT_Connect(String s_hht_domain, String s_hht_username, String s_hht_password, String s_hht_followerUrl, bool* p_login_HHT_Flag)
{
  if (WiFi.status() == WL_CONNECTED) {

    // WiFiClient c;
	HTTPClient http;
	http.begin(s_hht_followerUrl); //HTTP begin
  Serial.println("void HHT_Connect(): http try to connect: " + s_hht_followerUrl);
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
  if (WiFi.status() == WL_CONNECTED) {

    // WiFiClient c;
	HTTPClient http;
	http.begin(s_hht_followerUrl); //HTTP begin
  Serial.println("void HHT_Connect_Hard(): http try to connect: " + s_hht_followerUrl);
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

      String s_testUrl = "http://www.baidu.com";
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