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

#include "freeHHT.h"
#include "SetHHT.h"

#include "freeHHT_html.h"


const char* freehht_AP_NAME = "XoX freeHHT _";//Web配网模式下的AP-hht名字
String Pref_freeHHT_Username, Pref_freeHHT_Password, Pref_freeHHT_Domain, Pref_freeHHT_Interval ,Pref_freeHHT_FollowerUrl;

//暂时存储hht账号密码

char sta_freehht_followerUrl[64] = {0};
char sta_freehht_domain[32] = {0};
char sta_freehht_username[32] = {0};
char sta_freehht_password[32] = {0};
char sta_freehht_interval[16] = {0};

String freehht_followerUrl = "http://10.10.16.12/api/portal/v1/login";
String freehht_domain = "default";
String freehht_username = "12345678900";
String freehht_password = "123321";
String freehht_interval = "6.00";


String generate_freeurl(void){
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

const byte freehht_DNS_PORT = 53;//DNS端口号
IPAddress freehht_apIP(192, 168, 4, 1);//esp32-AP-IP地址
DNSServer freehht_dnsServer;//创建dnsServer实例

WebServer freehht_server(80);//创建WebServer


unsigned long lastGetTime_SetfreeHHT = 0;


bool setfreeHHT_Flag = false;
bool login_freeHHT_Flag = false;

void freehht_handleRoot() {//访问主页回调函数
  lastGetTime_SetfreeHHT = millis();  // 更新最后连接时间
  LedStatus_Light(hht_led);
  Serial.print("lastGetTime_SetfreeHHT: ");
  Serial.println(lastGetTime_SetfreeHHT);

  freehht_server.send(200, "text/html", freehht_page_html);
}

void freehht_initSoftAP(void) {//初始化AP模式
//把mac拼接到ap_name后
  uint8_t mac[6];
  WiFi.macAddress(mac);
  char macStr[18];
  sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.print("MAC Address: ");
  Serial.println(macStr);
  char wifiName[32];
  sprintf(wifiName, "%s%s", freehht_AP_NAME, macStr);

    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(freehht_apIP, freehht_apIP, IPAddress(255, 255, 255, 0));
    Serial.println(WiFi.softAPIP());
    //  Serial.print("本地IP： ");
    //  Serial.println(WiFi.localIP());
    if(WiFi.softAP(wifiName)){
        Serial.println("ESP32 SoftAP for freeHHTset is right");
    }
}

void freehht_initDNS(void){//初始化DNS服务器
    if(freehht_dnsServer.start(freehht_DNS_PORT, "*", freehht_apIP)){//判断将所有地址映射到esp8266的ip上是否成功
        Serial.println("start dnsserver success.");
    }
    else Serial.println("start dnsserver failed.");
}

unsigned long lastPostTime_SetfreeHHT = 0;

void freehht_handleRootPost() {//Post回调函数
    Serial.println("void freehht_handleRootPost()");
    lastPostTime_SetfreeHHT = millis();
    LedStatus_Light(hht_led);
    Serial.println("lastPostTime_SetfreeHHT: ");
    Serial.println(lastPostTime_SetfreeHHT);

    String freehhtuser="",freehhtpass="",freehhtdomain="",freehhtfollowerUrl="",freehhtinterval="";
    Serial.println("freehht_handleRootPost");


    if (freehht_server.hasArg("username")) {//判断是否有账号参数
      Serial.print("got username:");
      strcpy(sta_freehht_username, freehht_server.arg("username").c_str());//将账号参数拷贝到sta_ssid中
      Serial.println(sta_freehht_username);
    } 
    else {//没有参数
      Serial.println("error, not found username");
      freehht_server.send(200, "text/html", "<meta charset='UTF-8'>错误：请输入hht用户名");//返回错误页面
      return;
    }

    // //密码与账号同理
    // if (freehht_server.hasArg("password")) {
    //   Serial.print("got password:");
    //   strcpy(sta_freehht_password, freehht_server.arg("password").c_str());
    //   Serial.println(sta_freehht_password);
    // } 
    // else {
    //   Serial.println("error, not found password");
    //   freehht_server.send(200, "text/html", "<meta charset='UTF-8'>错误：请输入hht密码");
    //   return;
    // }


    // if (freehht_server.hasArg("domain")) {
    //   Serial.print("got domain:");
    //   strcpy(sta_freehht_domain, freehht_server.arg("domain").c_str());
    //   Serial.println(sta_freehht_domain);
    // }  
    // else if (sta_freehht_domain != "cmcc" && sta_freehht_domain != "unicom" && sta_freehht_domain != "telecom") {
    //   Serial.println("error, not found domain");
    //   freehht_server.send(200, "text/html", "<meta charset='UTF-8'>错误：请参考页末提示输入运营商代码");
    //   return;
    // } 
    // else {
    //   Serial.println("error, not found domain");
    //   freehht_server.send(200, "text/html", "<meta charset='UTF-8'>错误：运营商代码为空");
    //   return;
    // }

    if (freehht_server.hasArg("interval")) {
      Serial.print("got interval:");
      strcpy(sta_freehht_interval, freehht_server.arg("interval").c_str());
      Serial.println(sta_freehht_interval);
    } 
    else if ( String(sta_freehht_interval).toInt() >= 24) {
      Serial.println("error, interval >= 24 hours");
      freehht_server.send(200, "text/html", "<meta charset='UTF-8'>错误：自动重连时间(hour)不得大于24");
      return;
    } 
    else {
      Serial.println("error, not found interval");
      freehht_server.send(200, "text/html", "<meta charset='UTF-8'>错误：请输入自动重连时间");
      return;
    }


    // Preferences prefs; 
    // prefs.begin("freehht");
    // freehhtuser = sta_freehht_username; freehhtpass = sta_freehht_password; freehhtdomain = sta_freehht_domain; freehhtinterval = sta_freehht_interval;
    // prefs.putString( "freehht_username" ,freehhtuser);
    // prefs.putString( "freehht_password", freehhtpass);
    // prefs.putString( "freehht_domain", freehhtdomain);
    // prefs.putString( "freehht_interval", freehhtinterval);
    // prefs.putString( "freehht_followerUrl", freehht_followerUrl);

    // prefs.end();
    // Serial.println("Get HHT_PREFerences Success!");

    freehht_server.send(200, "text/html", freehht_ok_page_html);//返回保存成功页面
    delay(3500);
    //连接wifi
    //connectNewWifi();

    freehht_server.stop();
    WiFi.mode(WIFI_STA);//切换为STA模式
    WiFi.begin(PrefSSID.c_str(), PrefPassword.c_str());//连接上一次连接成功的wifi
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    WiFi.setAutoConnect(true);//设置自动连接

    Serial.println("");
    Serial.print("Connect to wifi because of setfreeHHT.");

    freeHHT_produce_phNUM(sta_freehht_username); //开始尝试

  Serial.println("ESP32 restart...free");
  ESP.restart(); //重启ESP32
}

void freehht_initWebServer(void){//初始化WebServer
  //server.on("/",handleRoot);
  //上面那行必须以下面这种格式去写否则无法强制门户
    freehht_server.on("/", HTTP_GET, freehht_handleRoot);//设置主页回调函数
    freehht_server.onNotFound(freehht_handleRoot);//设置无法响应的http请求的回调函数
    freehht_server.on("/", HTTP_POST, freehht_handleRootPost);//设置Post请求回调函数
    freehht_server.begin();//启动WebServer
    Serial.println("freehht_WebServer started!");
}

void connectNewfreeHHT(void){
    Serial.println("void connectNewfreeHHT()");

    Preferences prefs;
  prefs.begin("freehht");
  if (prefs.isKey("freehht_username"),"nano")
    Pref_freeHHT_Username = prefs.getString("freehht_username","nano");//如果键值为空，返回0
  // if (prefs.isKey("freehht_password"))
  //   Pref_freeHHT_Password = prefs.getString("freehht_password");
  // if (prefs.isKey("freehht_domain"))
  //   Pref_freeHHT_Domain = prefs.getString("freehht_domain");
  if (prefs.isKey("freehht_interval"))
    Pref_freeHHT_Interval = prefs.getString("freehht_interval");
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
    Serial.print("Connect to wifi because of setfreeHHT.");

    int count = 0;
    while (setfreeHHT_Flag == false) {
        delay(500);
        count++;
        if(count > 20){//如果10秒内（计数20次）没有连上，就开启Web配网 可适当调整这个时间
            setWiFi();
        }
        Serial.print(".!");
    }
    Serial.println("");

    Pref_freeHHT_FollowerUrl = "http://10.10.16.12/api/portal/v1/login";

    freehht_username = Pref_freeHHT_Username.c_str();
    freehht_password = Pref_freeHHT_Password.c_str();
    freehht_domain = Pref_freeHHT_Domain.c_str();
    freehht_followerUrl = Pref_freeHHT_FollowerUrl.c_str();
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

unsigned long lastSetRunTime_SetfreeHHT = 0;

void setfreeHHT()
{
  Serial.println("void setfreeHHT()");

  freehht_initSoftAP();
  freehht_initWebServer();
  freehht_initDNS();
  Serial.println("into void setfreeHHT()");

  while (setfreeHHT_Flag == false)
  {
      freehht_server.handleClient();
      freehht_dnsServer.processNextRequest();

      //配置页面未超时自动重启，防止连接不上导致卡在配网页面
      lastSetRunTime_SetfreeHHT = millis();
      Serial.print("lastConnectionTime_SetfreeHHT-interval : ");
      Serial.println(lastSetRunTime_SetfreeHHT - lastPostTime_SetfreeHHT);
      LedStatus_Switch(hht_led);
      if(lastSetRunTime_SetfreeHHT - lastPostTime_SetfreeHHT > (1000*60)*3)  //1000ms = 1s 一万毫秒等于一秒
      {
        Serial.println("Start to Reboot.");
        ESP.restart();    //重启复位esp32
        Serial.println("RebootED !");
      }

      if (login_freeHHT_Flag == true)
      {
          freehht_server.stop();
          setfreeHHT_Flag = true;
      }
  }
}

//删除保存的hht信息
void DeletefreeHHT() {
    Serial.println("DeletefreeHHT()");
    Preferences prefs;
    prefs.begin("freehht",false);//为false才能删除键值
    Serial.println(prefs.freeEntries());//查询清除前的剩余空间
    prefs.remove("freehht_followerurl");
    prefs.remove("freehht_username");
    prefs.remove("freehht_password"); // 删除当前命名空间中键名为"passwd"的元素
    prefs.remove("freehht_domain"); // 删除当前命名空间中键名为"ssid"的元素
    prefs.remove("freehht_interval"); 
    prefs.clear();
    delay(500);
    Serial.println(prefs.freeEntries());//查询清除后的剩余空间
    prefs.end();
    //esp_wifi_restore();  //删除保存的wifi信息
    Serial.println("freeHHT信息已清空,准备重启设备..");
}


//----------------------------------FREE_HHT-----------------------------------//
String freehht_payload = "{\"nano\":\"nano\"}";
String freehht_default_passwd[3] = {"123321", "135246", "135246"};
String freehht_default_domain[3] = {"cmcc", "unicom", "telecom",};

// void try_freeHHT() {

// }

void test_freeHHT(String full_number, String default_passwd[]) {
  Serial.println("into test_freeHHT(String full_number, String default_passwd[])");
  
  HHT_Logout();
  for (int i = 0; i < 3; ++i) {
    Serial.println(i);
    LedStatus_Switch(hht_led);
    HHT_Connect_Soft(freehht_default_domain[i], full_number, freehht_default_passwd[i], Pref_HHT_FollowerUrl.c_str(), &login_freeHHT_Flag);
    delay(100);
    LedStatus_Switch(hht_led);
    HHT_Connect_Hard(freehht_default_domain[i], full_number, freehht_default_passwd[i], Pref_HHT_FollowerUrl.c_str(), &login_freeHHT_Flag);
    delay(100);
    Serial.printf("try test playload : ");
    Serial.println(login_freeHHT_Flag);

    if (login_freeHHT_Flag)
    {

      Preferences prefs;
      prefs.begin("hht");
      // prefs.putString( "freehht_flag" ,"1");
      prefs.putString( "hht_username" ,full_number);
      prefs.putString( "hht_password", freehht_default_passwd[i]);
      prefs.putString( "hht_domain", freehht_default_domain[i]);
      // prefs.putString( "hht_interval", hhtinterval);
      // prefs.putString( "hht_followerUrl", hht_followerUrl);
      prefs.end();
      Serial.println("Get freeHHT_PREFerences Success!");

      // 重启esp32
      ESP.restart();
    }
  }
}

String freeHHT_create_phNUM(const char* num) {
  Serial.println("String process_number(const char* num)");
  char prefix[10]; // 用于存储前7位数字
  strncpy(prefix, num, 9);
  prefix[9] = '\0'; // 字符串结尾标志

  // 用于存储完整的11位数字
  char full_number[12];

  // 遍历0000到9999，添加到前7位数字后面
  for (int i = 0; i <= 99; ++i) {
    sprintf(full_number, "%s%02d", prefix, i);
    Serial.println(full_number);  //

    test_freeHHT(full_number, freehht_default_passwd); //测试连接

    // return String(full_number);
  }

  Serial.println("Fail to found 11number");
  return ""; // 如果循环结束仍未找到完整的11位数字，则返回空字符串
}

void freeHHT_produce_phNUM(String phone_num_str) {
  Serial.println("void produceNUM_freeHHT(String phone_num_str)");

  // 移除所有空格
  phone_num_str.replace(" ", "");

  char* input = new char[phone_num_str.length() + 1]; // 动态分配内存
  strcpy(input, phone_num_str.c_str()); // 将字符串复制到动态分配的数组

  char* token = strtok(input, ",");

  // 解析并处理每一个11位数字
  while (token != NULL) {
    // 判断是否为11位数字
    if (strlen(token) == 11) {
      freeHHT_create_phNUM(token);
    } else {
      Serial.println("Invalid number: " + String(token));
    }
    token = strtok(NULL, ",");
  }

  delete[] input; // 释放动态分配的内存
}







//----------------------------------链-表-----------------------------------//

numNode_t freeHHT_numNode;

// 在链表末尾插入一个节点
void append(numNode_s** head_ref, String new_num_str) {
    numNode_s* new_node = (numNode_s*)malloc(sizeof(numNode_s));
    new_node->num_str = new_num_str;
    new_node->next = NULL;
    if (*head_ref == NULL) {
        new_node->prev = NULL;
        *head_ref = new_node;
        return;
    }
    numNode_s* last = *head_ref;
    while (last->next != NULL) {
        last = last->next;
    }
    last->next = new_node;
    new_node->prev = last;
}

// 删除链表中指定值的节点
void deleteNode(numNode_s** head_ref, String num_str) {
    numNode_s* current = *head_ref;
    while (current != NULL) {
        if (current->num_str == num_str) {
            if (current->prev != NULL) {
                current->prev->next = current->next;
            }
            if (current->next != NULL) {
                current->next->prev = current->prev;
            }
            free(current);
            return;
        }
        current = current->next;
    }
}

// 在链表指定位置插入一个节点
void insertNode(numNode_s** head_ref, int position, String new_num_str) {
    numNode_s* new_node = (numNode_s*)malloc(sizeof(numNode_s));
    new_node->num_str = new_num_str;
    numNode_s* current = *head_ref;
    int count = 1;

    while (current != NULL && count < position) {
        current = current->next;
        count++;
    }

    new_node->next = current;
    new_node->prev = current->prev;
    if (current->prev != NULL) {
        current->prev->next = new_node;
    }
    current->prev = new_node;

    if (position == 1) {
        *head_ref = new_node;
    }
}


// // 打印链表
// void printList(numNode_s* head) {
//     numNode_s* current = head;
//     while (current != NULL) {
//         printf("%s ", current->num_str);
//         current = current->next;
//     }
//     printf("\n");
// }


// int main() {
//     numNode_s* head = NULL;
//     append(&head, "1");
//     append(&head, "2");
//     append(&head, "4");
//     printf("Original List: ");
//     printList(head);
    
//     insertNode(&head, 3, "3"); // 在位置 3 插入值为 "3" 的节点
//     printf("Updated List: ");
//     printList(head);
    
//     deleteNode(&head, "3"); // 删除值为 "3" 的节点
//     printf("Updated List after deletion: ");
//     printList(head);
    
//     return 0;
// }
