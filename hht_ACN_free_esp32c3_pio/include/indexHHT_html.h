#pragma once

#include <Arduino.h>

//选择功能页面
String indexhht_page_html = R"(
<!DOCTYPE html>
<html lang='en'>
  <head>
    <meta charset='UTF-8'>
    <meta name="viewport" content="width=device-width,initial-scale=1.0,maximum-scale=1.0,minimum-scale=1.0,user-scalable=no">
    <title>Button Execution Page</title>
    <style type="text/css">
      /* Your previous CSS here */
      * { margin: 0; padding: 0; }
      html { height: 100%; }
      body { height: 100%; background-color: #1F6F4A; }
      .dowebok { position: absolute; left: 50%; top: 30%; width: 380px; height: 500px; margin: -200px 0 0 -200px; border: 3px solid #fff; border-radius: 10px; overflow: hidden;}
      .form-item { position: relative; width: 360px; margin: 0 auto; padding-bottom: 20px;}
      .send_button { width: 360px; height: 50px; border: 0; border-radius: 25px; font-size: 18px; color: #1f6f4a; outline: none; cursor: pointer; background-color: #fff; }
      /* Add more if needed */
    </style>
  </head>
  <body>
    <div class="dowebok">
      <h2>请选择HHT功能</h2>
      <div class="form-item">
    <button style="width:100%;" onclick="window.location.href='/goTo_setHHT'">SetHHT</button>
      </div>
      <div class="form-item">
    <button style="width:100%;" onclick="window.location.href='/goTo_freeHHT'">FreeHHT</button>
      </div>
    </div>
    <script>
        // Your JavaScript here
        // 定义发送空POST请求的函数
        function sendEmptyPost() {
            fetch('/', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded',
                },
                body: 'setwifi_html'  // 空数据
            }).then(response => response.json())
            .then(data => console.log(data))
            .catch((error) => console.error('Error:', error));
        }

        // 每1秒（1000毫秒）执行一次sendEmptyPost函数
        setInterval(sendEmptyPost, 1000);
    </script>
  </body>
</html>
)";

String indexhht_ok_page_html = R"(
  
  <meta charset='UTF-8'>
  
  <h1>保存HHT成功，AutoConNetor重启中...</h1>

  <br>
  
  <h3>重置设备：先点击RST按钮，然后再快速按住FAC按钮不放，直到WiFi灯和HHT灯分别亮暗后放开，设备重置</h3>
)";