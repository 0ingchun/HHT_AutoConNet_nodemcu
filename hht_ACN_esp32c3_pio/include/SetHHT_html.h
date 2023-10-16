#pragma once

#include <Arduino.h>

//配网页面代码 
String hht_page_html = R"(
<!DOCTYPE html>
<html lang='en'>
  <head>
    <meta charset='UTF-8'>
   
    <meta name="viewport" content="width=device-width,initial-scale=1.0,maximum-scale=1.0,minimum-scale=1.0,user-scalable=no">
    <title>配置HuiHuTong🤡页面</title>
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
       table, th, td {
        border: 1px solid white;
        }
       
    </style>
  </head>
  
  <body>
    <div class="dowebok">
      <h2>惠糊嗵🤡参数配置</h2>
      <form style='text-align: center;padding-top: 20px' name='input' action='/' method='POST'>  
         <div class="form-item">
          <input id="username" type="text" name='username' autocomplete="off" placeholder="惠糊嗵🤡用户名">
         </div>
         <div class="form-item">
          <input id="password" type="password" name='password' autocomplete="off" placeholder="惠糊嗵🤡密码">
         </div>

            <br>
            <p><h3>🤡填写【惠糊嗵 用户名/密码】：🤡</h3></p>
              <h5>
                用户名一般为 手机号
                <br>
                密码若为修改，则为 123321
                <br>
              </h5>
            </p>
            <br>

         <div class="form-item">
          <input id="domain" type="text" name='domain' autocomplete="off" placeholder="运营商代码📶,留空无法登录">
         </div>

            <p><h3>📶填写【运营商代码】：📶</h3></p>
              <h5>
                “运营商代码”由若干字母组成，请参考下表，填写错误则无法自动登录
                <br>
                <!-- cmcc（中國移動），unicom（中國聯通），telecom（中國電信），default（默認） -->
                <table>
                  <tr>
                    <th>代码</th>
                    <td>CMCC</td>
                    <td>Unicom</td>
                    <td>Telecom</td>
                    <td>Default</td>
                  </tr>
                  <tr>
                    <th>运营商</th>
                    <td>中国移动</td>
                    <td>中国联通</td>
                    <td>中国电信</td>
                    <td>默认</td>
                  </tr>
                </table>
                <br>
                运营商代码示例：若您是“中国移动”用户，请填写 cmcc
              </h5>
            </p>
            <br>

         <div class="form-item">
          <input id="interval" type="text" name='interval' autocomplete="off" placeholder="自动重连时间⏰/小时，🉑写12">
         </div>

            <p><h3>⏰填写【自动重连时间】：⏰</h3></p>
              <h5>
                “自动重连时间”为阿拉伯数字（单位：小时），经过该时间后设备自动重新登录HuiHuTong
                <br>
                小时制自动重连示例：每12小时自动重连HuiHuTong，请填写 12
              </h5>
            </p>
            <br>

         <div class="form-item">
           <div id="">
            <input id="send_button" type='submit' value='👻 保存并登录 👻'>
           </div>
        </div>
        <div class="form-item">
          <div class="user_text">

<!--
            <br>
            <p><h3>📶填写【运营商代码】：📶</h3></p>
              <h5>
                “运营商代码”由若干字母组成，请参考下表，填写错误则无法自动登录
                <br>
                cmcc（中國移動），unicom（中國聯通），telecom（中國電信），default（默認）
                <br>
                运营商代码示例：若您是“中国移动”用户，请填写 cmcc
              </h5>
            </p>
-->

<!-- 
            <br>
            <p><h3>⏰填写【自动重连时间】：⏰</h3></p>
              <h5>
                “自动重连时间”为阿拉伯数字（单位：小时），经过该时间后设备自动重新登录HuiHuTong
                <br>
                小时制自动重连示例：每12小时自动重连HuiHuTong，请填写 12
              </h5>
            </p>
-->

            <br>
            <p><h3>🍡 AutoConNetor for HHT 🍡</h3></p>
              <h5>
              HuiHuTong自动登录器 - Powered by 0.C.
              </h5>
            </p>

          </div>
         </div>
        
      </form> 
     </div>
         <script>
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