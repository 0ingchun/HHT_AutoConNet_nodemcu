#pragma once

#include <Arduino.h>

//配网页面代码 
String hht_page_html = R"(
<!DOCTYPE html>
<html lang='en'>
  <head>
    <meta charset='UTF-8'>
   
    <meta name="viewport" content="width=device-width,initial-scale=1.0,maximum-scale=1.0,minimum-scale=1.0,user-scalable=no">
    <title>配置HHT宽带🤡页面</title>
    <style type="text/css">
      * { margin: 0; padding: 0; }
       html { height: 100%; }
       h2 {text-align: center;color: #fff;line-height: 2.2;}
       body { height: 100%; background-color: #1F6F4A; 50% 50% no-repeat; background-size: cover;}
       .dowebok { position: absolute; left: 50%; top: 30%; width: 380px; height: 500px; margin: -200px 0 0 -200px; border: 3px solid #fff; border-radius: 10px; overflow: hidden;}
       
       .form-item { position: relative; width: 360px; margin: 0 auto; padding-bottom: 20px;}
       .form-item input { width: 288px; height: 48px; padding-left: 10px; border: 1px solid #fff; border-radius: 25px; font-size: 18px; color: #fff; background-color: transparent; outline: none;}
       
              /* 添加到你的现有CSS代码中 */
        .form-item select {
          width: 296px; /* 宽度 */
          height: 48px; /* 高度 */
          padding-left: 10px; /* 左内边距 */
          border: 1px solid #fff; /* 边框 */
          border-radius: 25px; /* 圆角 */
          font-size: 18px; /* 字体大小 */
          color: #fff; /* 字体颜色 */
          background-color: transparent; /* 背景颜色 */
          outline: none; /* 外轮廓 */
          -webkit-appearance: none; /* 移除默认的下拉箭头 */
          -moz-appearance: none;
          appearance: none;
        }

        /* 为下拉框添加一个背景箭头图片（可选） */
        .form-item select::-ms-expand {
          display: none; /* 针对IE/Edge的样式 */
        }

        .form-item select::after {
          content: "\25BC"; /* 添加一个下拉箭头 */
          position: absolute;
          top: 50%;
          right: 10px;
          pointer-events: none;
          color: #fff;
        }
		
				/* 基础样式 */
		.form-item input[type="range"] {
		  width: 288px; /* 宽度 */
		  height: 48px; /* 高度 */
		  -webkit-appearance: none; /* 移除默认外观 */
		  appearance: none;
		  background: transparent; /* 背景色 */
		  outline: none; /* 移除轮廓 */
		  margin: 0; /* 无外边距 */
		}

		/* 滑块（Thumb）样式 */
		.form-item input[type="range"]::-webkit-slider-thumb {
		  -webkit-appearance: none; /* 移除默认外观 */
		  appearance: none;
		  width: 24px; /* 宽度 */
		  height: 24px; /* 高度 */
		  background: #FFF; /* 背景色 */
		  border-radius: 50%; /* 圆形 */
		  cursor: pointer; /* 手型光标 */
		}

		.form-item input[type="range"]::-moz-range-thumb {
		  width: 24px; /* 宽度 */
		  height: 24px; /* 高度 */
		  background: #FFF; /* 背景色 */
		  border-radius: 50%; /* 圆形 */
		  cursor: pointer; /* 手型光标 */
		}

		/* 轨道（Track）样式 */
		.form-item input[type="range"]::-webkit-slider-runnable-track {
		  width: 100%; /* 宽度 */
		  height: 8px; /* 高度 */
		  background: #ddd; /* 背景色 */
		  border-radius: 4px; /* 圆角 */
		}

    #intervalDisplay {
      font-size: 18px;  /* 设置字体大小 */
      color: #fff;      /* 设置字体颜色为白色 */
      line-height: 1.4; /* 设置行高 */
    }


		.form-item input[type="range"]::-moz-range-track {
		  width: 100%; /* 宽度 */
		  height: 8px; /* 高度 */
		  background: #ddd; /* 背景色 */
		  border-radius: 4px; /* 圆角 */
		}

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
      <h2>>HHT宽带🤡参数配置<</h2>

            <br>
            <p><h3>🤡填写【宽带 账号/密码】:🤡</h3></p>
              <h5>
                用户名一般为 手机号
                <br>
                密码若未修改，则为 123321
                <br>
              </h5>
            </p>
            <br>

      <form style='text-align: center;padding-top: 20px' name='input' action='/' method='POST'>  
         <div class="form-item">
          <input id="username" type="text" name='username' autocomplete="off" placeholder="宽带🤡账号">
         </div>
         <div class="form-item">
          <input id="password" type="password" name='password' autocomplete="off" placeholder="宽带🤡密码">
         </div>

            <br>
            <p><h3>🌐选择【宽带运营商】:🌐</h3></p>
              <h5>
                一般为您办卡手机号的运营商
                <br>
              </h5>
            </p>
            <br>

          <div class="form-item">
            <select id="domain" name="domain">
              <option value="" disabled selected>点击选择🌐运营商,留空无法登录</option>
              <option value="cmcc">中国移动 (CMCC)</option>
              <option value="unicom">中国联通 (Unicom)</option>
              <option value="telecom">中国电信 (Telecom)</option>
              <option value="default">默认 (Default)</option>
            </select>
          </div>

            <p><h3>⏰拖动修改【自动重连时间】:⏰</h3></p>
              <h5>
                “自动重连时间”（单位：小时/hour），经过该时间后设备自动重新登录宽带
                <br>
                示例：每12小时自动重连宽带，请拖动圆点，修改为 12
              </h5>
            </p>
            <br>

      <script>
        document.addEventListener("DOMContentLoaded", function() {
        var slider = document.getElementById("interval");
        var output = document.getElementById("intervalDisplay");
        
        // 初始化显示
        output.innerHTML = slider.value;
        
        // 更新显示
        slider.oninput = function() {
          output.innerHTML = this.value;
        }
        });
      </script>
      
      <div class="form-item">
        <p><h3>⏰ <span id="intervalDisplay">12</span> 小时/HOUR</h3></p>
        <br>
        <input id="interval" type="range" name="interval" min="0.5" max="23.5" step="0.5">
      </div>

      <br>

         <div class="form-item">
           <div id="">
            <input id="send_button" type='submit' value='👻 保存并登录 👻'>
           </div>
        </div>
        <div class="form-item">
          <div class="user_text">

      <br>

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