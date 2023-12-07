/* WEMOS D1 MINI - RBG LED STRIP (4 PIN) HOMEKIT CONTROLLER
 *
 * Changes by Nino Cataffo https://github.com/TheDevNino 
 * Author of the initial Library: Mixiaoxiao (Wang Bin) https://github.com/Mixiaoxiao
 * Arduino-HomeKit-ESP8266 Library: https://github.com/Mixiaoxiao/Arduino-HomeKit-ESP8266
 *
 */

#include <Arduino.h>
#include <arduino_homekit_server.h>
#include "wifi_info.h"

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);

// WEMOS D1 MINI CONNECTIONS
#define REDPIN 15    //D8  GPIO15
#define GREENPIN 12  //D6  GPIO12
#define BLUEPIN 13   //D7  GPIO13

bool received_sat = false;
bool received_hue = false;

bool is_on = false;
float current_brightness =  100.0;
float current_sat = 0.0;
float current_hue = 0.0;
int rgb_colors[3];

void setup() {
	Serial.begin(115200);
	wifi_connect(); // in wifi_info.h

  // Define MOSFET RGB Connections
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);

  // All Channels LOW
  analogWrite(REDPIN, 0);
  analogWrite(GREENPIN, 0);
  analogWrite(BLUEPIN, 0);

	my_homekit_setup();
}

void loop() {
	my_homekit_loop();
	delay(10);
}

//==============================
// HomeKit setup and loop
//==============================

// access your HomeKit characteristics defined in my_accessory.c

extern "C" homekit_server_config_t accessory_config;
extern "C" homekit_characteristic_t cha_on;
extern "C" homekit_characteristic_t cha_bright;
extern "C" homekit_characteristic_t cha_sat;
extern "C" homekit_characteristic_t cha_hue;

static uint32_t next_heap_millis = 0;

void my_homekit_setup() {

  cha_on.setter = set_on;
  cha_bright.setter = set_bright;
  cha_sat.setter = set_sat;
  cha_hue.setter = set_hue;
  
	arduino_homekit_setup(&accessory_config);

}

void my_homekit_loop() {
	arduino_homekit_loop();
	const uint32_t t = millis();
	if (t > next_heap_millis) {
		// show heap info every 5 seconds
		next_heap_millis = t + 5 * 1000;
		LOG_D("Free heap: %d, HomeKit clients: %d",
				ESP.getFreeHeap(), arduino_homekit_connected_clients_count());
	}
}

void set_on(const homekit_value_t v) {
    bool on = v.bool_value;
    cha_on.value.bool_value = on; //sync the value

    if(on) {
        is_on = true;
        Serial.println("On");
    } else  {
        is_on = false;
        Serial.println("Off");
    }

    updateColor();
}

void set_hue(const homekit_value_t v) {
    Serial.println("set_hue");
    float hue = v.float_value;
    cha_hue.value.float_value = hue; //sync the value

    current_hue = hue;
    received_hue = true;
    
    updateColor();
}

void set_sat(const homekit_value_t v) {
    Serial.println("set_sat");
    float sat = v.float_value;
    cha_sat.value.float_value = sat; //sync the value

    current_sat = sat;
    received_sat = true;
    
    updateColor();

}

void set_bright(const homekit_value_t v) {
    Serial.println("set_bright");
    int bright = v.int_value;
    cha_bright.value.int_value = bright; //sync the value

    current_brightness = bright;

    updateColor();
}

void updateColor()
{
  if(is_on)
  {
      if(received_hue && received_sat)
      {
        HSV2RGB(current_hue, current_sat, current_brightness);
        received_hue = false;
        received_sat = false;
      }
      
      int b = map(current_brightness,0, 100,100, 1);
      Serial.println(b);
  
      analogWrite(REDPIN, rgb_colors[0]/b);
      analogWrite(GREENPIN, rgb_colors[1]/b);
      analogWrite(BLUEPIN, rgb_colors[2]/b);
    }
  else if(!is_on) //lamp - switch to off
  {
      Serial.println("is_on == false");
      analogWrite(REDPIN, 0);
      analogWrite(GREENPIN, 0);
      analogWrite(BLUEPIN, 0);
  }
}

void HSV2RGB(float h,float s,float v) {

  int i;
  float m, n, f;

  s/=100;
  v/=100;

  if(s==0){
    rgb_colors[0]=rgb_colors[1]=rgb_colors[2]=round(v*1023);
    return;
  }

  h/=60;
  i=floor(h);
  f=h-i;

  if(!(i&1)){
    f=1-f;
  }

  m=v*(1-s);
  n=v*(1-s*f);

  switch (i) {

    case 0: case 6:
      rgb_colors[0]=round(v*1023);
      rgb_colors[1]=round(n*1023);
      rgb_colors[2]=round(m*1023);
    break;

    case 1:
      rgb_colors[0]=round(n*1023);
      rgb_colors[1]=round(v*1023);
      rgb_colors[2]=round(m*1023);
    break;

    case 2:
      rgb_colors[0]=round(m*1023);
      rgb_colors[1]=round(v*1023);
      rgb_colors[2]=round(n*1023);
    break;

    case 3:
      rgb_colors[0]=round(m*1023);
      rgb_colors[1]=round(n*1023);
      rgb_colors[2]=round(v*1023);
    break;

    case 4:
      rgb_colors[0]=round(n*1023);
      rgb_colors[1]=round(m*1023);
      rgb_colors[2]=round(v*1023);
    break;

    case 5:
      rgb_colors[0]=round(v*1023);
      rgb_colors[1]=round(m*1023);
      rgb_colors[2]=round(n*1023);
    break;
  }
}
