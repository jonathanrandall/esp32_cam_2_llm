
#ifndef CAM_IND_H
#define CAM_IND_H


//see https://github.com/limengdu/SeeedStudio-XIAO-ESP32S3-Sense-camera/tree/main/CameraWebServer_for_esp-arduino_3.0.x



static const char INDEX_HTML[] = R"rawliteral(
<html>
  <head>
    <title>ESP32-CAM Video Stream</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      body { font-family: Arial; text-align: center; margin:0px auto; padding-top: 30px;}
      table { margin-left: auto; margin-right: auto; }
      td { padding: 8 px; }
      .button {
        background-color: #2f4468;
        border: none;
        color: white;
        padding: 20px 40px;
        text-align: center;
        text-decoration: none;
        display: inline-block;
        font-size: 20px;
        margin: 6px 3px;
        cursor: pointer;
        -webkit-touch-callout: none;
        -webkit-user-select: none;
        -khtml-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;
        user-select: none;
        -webkit-tap-highlight-color: rgba(0,0,0,0);
      }
      img {  width: auto ;
        max-width: 100% ;
        height: auto ; 
      }
    </style>
  </head>
  <body>
    <h1>ESP32-CAM Video Stream</h1>
    <img src="" id="photo" >
    <div align="center">
      <button class="button" onclick="startStream('start');">START</button>
      <button class="button" onclick="startStream('stop');">STOP</button>
    </div>
   <script>
    function startStream(x) {
      if(x=='start'){
        document.getElementById("photo").src = window.location.href.slice(0, -1) + ":81/stream";
      }
      else if(x=='stop'){
        document.getElementById("photo").src = "";
      } 
    }
    window.onload = document.getElementById("photo").src = window.location.href.slice(0, -1) + ":81/stream";
  </script>
  </body>
</html>
)rawliteral";

#endif