# ESP8266-WiFi-RGB_LED
Control your NeoPixel-Rig via Smartphone / Tablet
![here should be a image](http://netz-weit.de/img_3072.jpg "ESP8266-01 with 7 RGB-LEDs hooked up")
* [Fritzing Breadboard Picture](../fritzing_bb.png)

Components:
* 7 LED 'Jewels' cost around 1-3$
* ESP8266-01  around 1-3$
* 1000uF capacitor around 10 cent;

required Aduino-IDE Libraries:
* [esp8266](https://github.com/icsom/ESP8266-WiFi-RGB_LED)
* [adafruit neopixel](https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-installation)
* [wifimanager](https://github.com/tzapu/WiFiManager)
* [remotexy](http://remotexy.com/en/help/how-install-library/)

HowTo:
Compile & upload this sketch via the arduino-ide to your ESP8266 and Control
the RGB-LEDs via your Smartphone or Tablet.

Configuration:
You don't need to change anything in the sketch - there is only the Wireless lan,
which needs to be configured - this can be done while runtime.
This sketch uses the functionality of the WiFiManager:
If the ESP8266 can not connect to a Wireless lan, which will be the case in most
of the first bootups, it opens a accesspoint with the
SSID 'WiFi-RGB' and the PSK 'mosci12345' (this can be changed in the sketch).
You will connect your wlan-device to this accesspoint, and regardles which website
you gonna open in your browser, you will get pointed to the WiFi-Configuratiom Panel.
Once, WiFi is configuered, the ESP8266 will close the accesspoint and connect to
your wlan - and will do so after every powercycle.
* [WiFiManager Image 1](http://netz-weit.de/img_3088.png)
* [WiFiManager Image 2](http://netz-weit.de/img_3090.png)

Install the App from [RemoteXY](http://remotexy.com/en/download/)
(The 'free version' of the RemoteXY-App will be enough for this gimmick)
Open the app, create a 'ethernet connection' to the IP-Address of the ESP8266
and enjoy changing the colors.
* [RemoteXY Image 1](http://netz-weit.de/img_3092.png)
* [RemoteXY Image 2](http://netz-weit.de/img_3085.png)
