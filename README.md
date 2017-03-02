# ESP8266-WiFi-RGB_LED
Control your NeoPixel-Rig via Smartphone / Tablet
![here should be a image](http://netz-weit.de/img_3072.jpg "ESP8266-01 with 7 RGB-LEDs hooked up")

7 LED 'Jewels' cost around 1-3$
ESP8266-01 are around 1-3$

Additional Libraries:
* [esp8266]: https://github.com/icsom/ESP8266-WiFi-RGB_LED
* [adafruit neopixel]: https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-installation
* [wifimanager]: https://github.com/tzapu/WiFiManager
* [remotexy]: http://remotexy.com/en/help/how-install-library/

Compile & upload this sketch via the arduino-ide to your ESP8266 and Control
the RGB-LEDs via your smartphone-app.

This sketch uses the functionality of the WiFiManager:
If the ESP8266 can not connect to a Wireless lan, which will be the case in most
of the first bootups, it opens a accesspoint with the
SSID 'WiFi-RGB' and the PSK 'mosci12345'.
You will connect your wlan-device to this accesspoint, and regardles which website
you gonna open in your browser, you will get delivered the WiFi-Configuratiom Panel.
Once, WiFi is configuered, the ESP8266 will close the accesspoint and connect to
your wlan.
![here should be a image](http://netz-weit.de/img_3088.png "WiFiManager")
![here should be a image](http://netz-weit.de/img_3090.png "WiFiManager")

Install the App from [RemoteXY]: http://remotexy.com/en/download/
(The 'free version' of the RemoteXY-App will be enough for this sketch)
Open the app, create a 'ethernet connection' to the IP-Address of the ESP8266
and enjoy changing the colors.
![here should be a image](http://netz-weit.de/img_3092.png "LED-Controls")
![here should be a image](http://netz-weit.de/img_3085.png "LED-Controls")
