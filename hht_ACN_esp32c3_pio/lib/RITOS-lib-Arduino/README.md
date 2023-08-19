# RITOS / 互聯網時間操作系統
 for ESP8266 in Arduino

<https://github.com/0ingchun/RITOS-lib-Arduino>

Real Internet Time Operation System (RITOS). An RTOS library of the esp8266 core for Arduino IDE.

即時互聯網時間操作系統（RITOS）。 一個基於 Arduino IDE 的 Esp8266 處理器核的 RTOS 庫。

————————————————

**Development Environment / 開發環境**

Arduino IDE， PlatformIO

Adapt for ( esp8266, esp8266-12f, esp8266-12e, esp01, esp01s )

適用於 ( esp8266, esp8266-12f, esp8266-12e, esp01, esp01s )

————————————————

## **Start / 開始**

1. Installation library file / 安裝庫文件

Please download.zip and import

請下載.zip並導入

[Download / 下載 ](https://github.com/0ingchun/RITOS-lib-Arduino/archive/refs/heads/main.zip)RITOS-lib-Arduino.zip

2. Install the development board file / 安裝開發板文件

Add the source and download the esp8266 development board : 

新增該源并且下載esp8266開發板：

http://arduino.esp8266.com/stable/package_esp8266com_index.json

3. Try example / 嘗試示例

[RITOS-esp8266./examples/RITOSbasic/RITOSbasic.ino](https://github.com/0ingchun/RITOS-lib-Arduino/blob/main/examples/RITOSbasic/RITOSbasic.ino)

————————————————

**Attention / 注意**

- Use RITOS Note the Main Points:

1. Declare the Ritos class before creating the thread

The format is: Ritos class name;

2. Create a thread in the following format: class name task(function name);

3. The Delay() delay function cannot be used in the thread,
because RITOS and Delay() delay function call the same timer.
If you want to use the delay function in the thread,
please refer to Demo (RITOS-esp8266./examples/RITOSbasic/RITOSbasic.ino)


- 使用RITOS注意要點：

1. 建立線程前要先聲明Ritos的類

2. 格式為： Ritos 類名;

2.創建線程格式為：類名.task（函數名）;

3.在線程內無法使用Delay()延時函數，
因為RITOS與Delay()延時函數調用的是同一個定時器，
如果想要在線程內使用延時函數，
請參考Demo (RITOS-esp8266./examples/RITOSbasic/RITOSbasic.ino)

————————————————

**Thanks / 鳴謝**

Heartfelt thanks! To learn from you and salute.

*Original Author* : [SanUSB grupo](https://github.com/SanUSB-grupo) <<http://sanusb.org>>

[Original Project Link](https://github.com/SanUSB-grupo/RITOS)

————————————————

修改，打包，注釋等

Modify, Package, Comment, etc

- by *0. Chun*   <- a Runoob 一隻菜鳥

————————————————

[MIT License](https://github.com/0ingchun/RITOS-lib-Arduino/blob/main/LICENSE)
