# Canon Developer

- ability to access camera remote control over bluetooth / WiFi
  - https://developers.canon-europe.com/

# Canon Camera Connect API

1. connect to camera network

2. call ccapi url to complete connection

http://192/168.1.2:8080/ccapi

3. when done disconnect on camera

4: When the client disconnects, it does NOT terminate the camera connection, MUST be done on camera

5: Station disconnected fires on the device as expected

# Root ccapi response

<pre>
{
  "ver100": [
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/deviceinformation",
      "get": true,
      "post": false,
      "put": false,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/functions/autopoweroff",
      "get": true,
      "post": false,
      "put": true,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/control/shutterbutton",
      "get": false,
      "post": true,
      "put": false,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/control/shutterbutton/manual",
      "get": false,
      "post": true,
      "put": false,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/control/moviemode",
      "get": true,
      "post": true,
      "put": false,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/control/recbutton",
      "get": false,
      "post": true,
      "put": false,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/control/af",
      "get": false,
      "post": true,
      "put": false,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings/shootingmodedial",
      "get": true,
      "post": false,
      "put": false,
      "delete": false
    },
  ]
}
</pre>

<pre>
{
  "ver100": [
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/deviceinformation",
      "get": true,
      "post": false,
      "put": false,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/devicestatus/storage",
      "get": true,
      "post": false,
      "put": false,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/devicestatus/battery",
      "get": true,
      "post": false,
      "put": false,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/devicestatus/lens",
      "get": true,
      "post": false,
      "put": false,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/devicestatus/temperature",
      "get": true,
      "post": false,
      "put": false,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/functions/registeredname/copyright",
      "get": true,
      "post": false,
      "put": true,
      "delete": true
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/functions/registeredname/author",
      "get": true,
      "post": false,
      "put": true,
      "delete": true
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/functions/registeredname/ownername",
      "get": true,
      "post": false,
      "put": true,
      "delete": true
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/functions/registeredname/nickname",
      "get": true,
      "post": false,
      "put": true,
      "delete": true
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/functions/datetime",
      "get": true,
      "post": false,
      "put": true,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/functions/cardformat",
      "get": false,
      "post": true,
      "put": false,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/functions/beep",
      "get": true,
      "post": false,
      "put": true,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/functions/displayoff",
      "get": true,
      "post": false,
      "put": true,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/functions/autopoweroff",
      "get": true,
      "post": false,
      "put": true,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/functions/wificonnection",
      "get": false,
      "post": true,
      "put": false,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/functions/wifisetting",
      "get": true,
      "post": false,
      "put": false,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/functions/wifisetting/set1",
      "get": true,
      "post": false,
      "put": true,
      "delete": true
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/functions/wifisetting/set2",
      "get": true,
      "post": false,
      "put": true,
      "delete": true
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/functions/wifisetting/set3",
      "get": true,
      "post": false,
      "put": true,
      "delete": true
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/contents",
      "get": true,
      "post": true,
      "put": true,
      "delete": true
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/control/shutterbutton",
      "get": false,
      "post": true,
      "put": false,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/control/shutterbutton/manual",
      "get": false,
      "post": true,
      "put": false,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/control/moviemode",
      "get": true,
      "post": true,
      "put": false,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/control/recbutton",
      "get": false,
      "post": true,
      "put": false,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/control/drivefocus",
      "get": false,
      "post": true,
      "put": false,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/control/af",
      "get": false,
      "post": true,
      "put": false,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings",
      "get": true,
      "post": false,
      "put": false,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings/shootingmodedial",
      "get": true,
      "post": false,
      "put": false,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings/av",
      "get": true,
      "post": false,
      "put": true,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings/tv",
      "get": true,
      "post": false,
      "put": true,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings/iso",
      "get": true,
      "post": false,
      "put": true,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings/exposure",
      "get": true,
      "post": false,
      "put": true,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings/wb",
      "get": true,
      "post": false,
      "put": true,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings/colortemperature",
      "get": true,
      "post": false,
      "put": true,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings/afoperation",
      "get": true,
      "post": false,
      "put": false,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings/afmethod",
      "get": true,
      "post": false,
      "put": true,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings/stillimagequality",
      "get": true,
      "post": false,
      "put": true,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings/stillimageaspectratio",
      "get": true,
      "post": false,
      "put": true,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings/flash",
      "get": true,
      "post": false,
      "put": true,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings/metering",
      "get": true,
      "post": false,
      "put": true,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings/drive",
      "get": true,
      "post": false,
      "put": true,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings/aeb",
      "get": true,
      "post": false,
      "put": true,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings/wbshift",
      "get": true,
      "post": false,
      "put": true,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings/wbbracket",
      "get": true,
      "post": false,
      "put": true,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings/colorspace",
      "get": true,
      "post": false,
      "put": true,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings/picturestyle",
      "get": true,
      "post": false,
      "put": true,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings/picturestyle/auto",
      "get": true,
      "post": false,
      "put": true,
      "delete": true
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings/picturestyle/standard",
      "get": true,
      "post": false,
      "put": true,
      "delete": true
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings/picturestyle/portrait",
      "get": true,
      "post": false,
      "put": true,
      "delete": true
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings/picturestyle/landscape",
      "get": true,
      "post": false,
      "put": true,
      "delete": true
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings/picturestyle/finedetail",
      "get": true,
      "post": false,
      "put": true,
      "delete": true
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings/picturestyle/neutral",
      "get": true,
      "post": false,
      "put": true,
      "delete": true
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings/picturestyle/faithful",
      "get": true,
      "post": false,
      "put": true,
      "delete": true
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings/picturestyle/monochrome",
      "get": true,
      "post": false,
      "put": true,
      "delete": true
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings/picturestyle/userdef1",
      "get": true,
      "post": false,
      "put": true,
      "delete": true
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings/picturestyle/userdef2",
      "get": true,
      "post": false,
      "put": true,
      "delete": true
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings/picturestyle/userdef3",
      "get": true,
      "post": false,
      "put": true,
      "delete": true
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings/picturestyle/userdef1/basepicturestyle",
      "get": true,
      "post": false,
      "put": true,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings/picturestyle/userdef2/basepicturestyle",
      "get": true,
      "post": false,
      "put": true,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings/picturestyle/userdef3/basepicturestyle",
      "get": true,
      "post": false,
      "put": true,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings/moviequality",
      "get": true,
      "post": false,
      "put": true,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings/soundrecording",
      "get": true,
      "post": false,
      "put": true,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings/soundrecording/level",
      "get": true,
      "post": false,
      "put": true,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings/soundrecording/windfilter",
      "get": true,
      "post": false,
      "put": true,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/settings/soundrecording/attenuator",
      "get": true,
      "post": false,
      "put": true,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/information/recordable",
      "get": true,
      "post": false,
      "put": false,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/liveview",
      "get": false,
      "post": true,
      "put": false,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/liveview/flip",
      "get": true,
      "post": false,
      "put": false,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/liveview/flipdetail",
      "get": true,
      "post": false,
      "put": false,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/liveview/scroll",
      "get": true,
      "post": false,
      "put": false,
      "delete": true
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/liveview/scrolldetail",
      "get": true,
      "post": false,
      "put": false,
      "delete": true
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/liveview/rtp",
      "get": true,
      "post": true,
      "put": false,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/liveview/rtpsessiondesc",
      "get": true,
      "post": false,
      "put": false,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/shooting/liveview/angleinformation",
      "get": false,
      "post": true,
      "put": false,
      "delete": false
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/event/polling",
      "get": true,
      "post": false,
      "put": false,
      "delete": true
    },
    {
      "url": "http://192.168.1.2:8080/ccapi/ver100/event/monitoring",
      "get": true,
      "post": false,
      "put": false,
      "delete": true
    }
  ],
  "ver110": [
    {
      "url": "http://192.168.1.2:8080/ccapi/ver110/shooting/settings/highframerate",
      "get": true,
      "post": false,
      "put": true,
      "delete": false
    }
  ]
}
</pre>
