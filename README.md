# HomeKit-LED-Controller-ESP8266
You can control a 4 PIN LED Strip just by using MOSFETs and a ESP8266 with this Code

Details...

This Arduino sketch is designed to run on a Wemos D1 Mini board and control an RGB LED strip with four pins using HomeKit. It allows you to control the color, brightness, and power state of the RGB LED strip using HomeKit accessories on iOS devices. Below is a brief description of the key components and functions in the code:

### Dependencies:
The code includes the necessary libraries, including Arduino.h and arduino_homekit_server.h.
It relies on the "Arduino-HomeKit-ESP8266" library by Mixiaoxiao for HomeKit functionality.
### Pin Definitions:
The RGB LED strip is connected to the Wemos D1 Mini using pins D8 (GPIO15) for the red channel, D6 (GPIO12) for the green channel, and D7 (GPIO13) for the blue channel.
### WiFi Connection:
Wi-Fi connection is established using the wifi_connect function defined in an external header file (wifi_info.h).
Write your **SSID** and **Password** into the header file, or include an AP Capture.
```
const char *ssid = "YOUR-SSID";
const char *password = "YOUR-PWD";
```

![LED-RGB-Strip-Controller_bb](https://github.com/TheDevNino/HomeKit-LED-Controller-ESP8266/assets/44969214/a1029a23-b1d5-4ddf-b17e-5fd42e08c151)

[Image source](https://www.ghost7.com/build-a-homekit-enabled-wifi-led-strip/)
