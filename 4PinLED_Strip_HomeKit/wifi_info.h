/*
 * wifi_info.h
 *
 *  Created on: 2020-05-15
 *      Author: Mixiaoxiao (Wang Bin)
 */

#ifndef WIFI_INFO_H_
#define WIFI_INFO_H_

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif
#include <WiFiManager.h>

void wifi_connect() {
    WiFiManager wifiManager;

    // Start WiFiManager (connect or create an AP and serve the config portal)
    if (!wifiManager.autoConnect("CustomAP")) {
        Serial.println("Failed to connect and hit timeout");
        delay(3000);
        // Reset and try again, or do whatever is required
        ESP.restart();
        delay(5000);
    }

    // If connected or AP mode, print the current IP
    Serial.println("Connected! IP address:");
    Serial.println(WiFi.localIP());
}

#endif /* WIFI_INFO_H_ */
