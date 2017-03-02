/**
 * Author: mosci
 * contact: icsom@gmx.de
 * last edited: 01.03.2017
 * License: Beer ;-)

 * Wireless RGB-LED
 * This sketch turns the ESP-01 (hooked up with one or more NeoPixel on GPIO02)
 * into a wireless LED controlled the Android & Iphone app 'remoteXY'
 * http://remotexy.com
 *
 * you can controll RGB RED, GREEN & BLUE via the APP-Slider
 * and Store the 'default-color' (which will lid by power-on) to EEPROM
 *
 * Configuring ...
 * ...is fairly easy, since I implenented WiFiManager:
 * If the ESP8266 can not connect to a Accesspoint, it will open a Accesspoint itself
 * with the SSID: 'RGB-LED' and the Password 'mosci12345'
 * connect your smartphone or tablet to this WLAN, and you will prompted a
 * configuration interface (if not automaticly, just try to connect you browser to any url you like ;-))
 * once a valid WLAN-Configuration is entered, this connection will be used after the next powercycle;
 *
 */

#include <EEPROM.h>
#include <Arduino.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiManager.h>

ESP8266WiFiMulti WiFiMulti;

IPAddress ip(192, 168, 1, 36);  //my predefined values - this can be overwritten through WiFiManager
IPAddress gw(192, 168, 1, 1);   //while WiFi-Configuration
IPAddress mask(255, 255, 255, 0);
char con_ssid[32]; //will hold the ssid of the wlan to which it is connectd
char con_psk[64];  // ^^ private shared key for the wlan (password)

#include <Adafruit_NeoPixel.h>
#define LEDS_PIN        2 //LED-Control-Pin
#define LEDS_NUMPIXELS  7 //number of LEDs
#define BLINKINTERVAL 200 //blink-interval in milliseconds
Adafruit_NeoPixel pixels(LEDS_NUMPIXELS, LEDS_PIN, NEO_GRB + NEO_KHZ800);
uint32_t currColor=pixels.Color(0,50,0,0); //current shown color if switch=1
uint32_t defaultColor; //startup-color & color if switch=0
uint32_t lastColor; //color-buffer
uint8_t red;  //red value of the app-led
uint8_t green; //green value of the app-led
uint8_t blue; //blue value of the app-led

/*
   -- rgb_led --

   This source code of graphical user interface
   has been generated automatically by RemoteXY editor.
   To compile this code using RemoteXY library 2.2.5 or later version
   download by link http://remotexy.com/en/library/
   To connect using RemoteXY mobile app by link http://remotexy.com/en/download/
     - for ANDROID 3.7.1 or later version;
     - for iOS 1.0.7 or later version;

   This source code is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
*/
//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// RemoteXY select connection mode and include library
#define REMOTEXY_MODE__ESP8266WIFI_LIB

// RemoteXY connection settings
#define REMOTEXY_WIFI_SSID ""
#define REMOTEXY_WIFI_PASSWORD ""
#define REMOTEXY_SERVER_PORT 6377
#include <RemoteXY.h>

// RemoteXY configurate
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =
  { 4,3,45,0,6,0,0,4,0,6
  ,4,8,56,1,4,0,19,4,8,56
  ,4,4,0,32,4,8,56,6,2,0
  ,62,4,22,11,2,79,78,0,79,70
  ,70,0,65,7,53,19,41,41,2 };

// this structure defines all the variables of your control interface
struct {

    // input variable
  int8_t red; // =0..100 slider position
  int8_t green; // =0..100 slider position
  int8_t blue; // =0..100 slider position
  uint8_t switch1; // =1 if switch ON and =0 if OFF

    // output variable
  uint8_t led_r; // =0..255 LED Red brightness
  uint8_t led_g; // =0..255 LED Green brightness
  uint8_t led_b; // =0..255 LED Blue brightness

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

//split the unit32_t rgb into unit8_t r, g or b
uint8_t splitColor ( uint32_t c, char value )
{
  switch ( value ) {
    case 'r': return (uint8_t)(c >> 16);
    case 'g': return (uint8_t)(c >>  8);
    case 'b': return (uint8_t)(c >>  0);
    default:  return 0;
  }
}

//blink black/currColor
void blinkPixel(int blinks) {
  currColor=pixels.getPixelColor(0);

  for (int i=0; i < blinks; i++) {
    changeColor(pixels.Color(0,0,0,0));
    pixels.show();
    delay(BLINKINTERVAL);
    changeColor(currColor);
    pixels.show();
    delay(BLINKINTERVAL);
  }
}
//change color and get app-values for the app-led
void changePixel(uint32_t pcolor) {
    if (RemoteXY.switch1==1) {
      lastColor=currColor;
      currColor=pixels.Color(RemoteXY.red, RemoteXY.green, RemoteXY.blue);
    }
    red   = splitColor(pcolor, 'r')*2.5;
    green = splitColor(pcolor, 'g')*2.5;
    blue  = splitColor(pcolor, 'b')*2.5;
    RemoteXY.led_r  = red;
    RemoteXY.led_g  = green;
    RemoteXY.led_b  = blue;
    changeColor(currColor);
    pixels.show();
  }

//iterate though all leds and set color
void changeColor(uint32_t newColor) {
    for (int i=0; i<LEDS_NUMPIXELS;i++) {
      pixels.setPixelColor(i, newColor);
    }
  }

void setup() {
  EEPROM.begin(512);
  pixels.begin(); // This initializes the NeoPixel library.
  changePixel(currColor); // red
  pixels.show();
  delay(BLINKINTERVAL);

  Serial.begin(115200);

  currColor=pixels.Color(0,0,0,20), //blue
  changePixel(currColor); // change pixel color.

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  //reset settings - for testing
  //wifiManager.resetSettings();
  currColor=pixels.Color(20,0,0); // Moderately dark red color.
  changePixel(currColor);
  wifiManager.setSTAStaticIPConfig(ip, gw, mask);
  if (!wifiManager.autoConnect("WiFi-RGB", "mosci12345")) {
    Serial.println("failed to connect, we should reset as see if it connects");
    delay(3000);
    ESP.reset();
    delay(5000);
  }

  //if you get here you have connected to the WiFi

  Serial.printf("connected to %s\n", con_ssid);
  Serial.println("local ip");
  Serial.println(WiFi.localIP());

  WiFi.SSID().toCharArray(con_ssid, 32);
  WiFi.psk().toCharArray(con_psk, 64);
  remotexy = new CRemoteXY (RemoteXY_CONF_PROGMEM, &RemoteXY, con_ssid , con_psk, REMOTEXY_SERVER_PORT);

  configTime(1 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("Setup Done.");

  EEPROM.get((512-32), defaultColor); //get stored default color from eeprom
  currColor=defaultColor;
  lastColor=defaultColor;
  changePixel(currColor); //show default color - we're going to the main loop now
}

void loop() {
  //"get app-values" (simplyfied)
  RemoteXY_Handler ();

  //app-switch is in position 'ON'
  //show color generated from the values of the app-slider
  if (RemoteXY.switch1 == 1) {
    changePixel(currColor);
  }
  //the switch has been turnd off & the last color was different from the defaultColor
  //we will store the new defaultColor
  else if ( (RemoteXY.switch1 == 0) && (defaultColor != lastColor) ) {
      EEPROM.put((512-32), lastColor);
      EEPROM.commit();
      delay(5);
      EEPROM.get((512-32), defaultColor);
      Serial.printf("-> store def: %d\n", lastColor);
      blinkPixel(2);
    }
  //show defaultColor
  else if (RemoteXY.switch1 == 0) {
      changePixel(defaultColor);
    }
  //take some rest
  delay(5);
}
