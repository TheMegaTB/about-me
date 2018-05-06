#ifndef SETUP_HPP
#define SETUP_HPP

#include <ArduinoOTA.h>
#include <NeoPixelBus.h>

#include "config.hpp"
#include "strip.hpp"

void setupOTA() {
    Serial.println("Setting up OTA ...");

    ArduinoOTA.setHostname(WLAN_HOSTNAME);
    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
            type = "sketch";
        else // U_SPIFFS
            type = "filesystem";

        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        Serial.println("Start updating " + type);
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\n", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

    ArduinoOTA.begin();
}

void setupWiFi() {
    Serial.println("Connecting WiFi ...");

    WiFi.hostname(WLAN_HOSTNAME);
    WiFi.mode(WIFI_STA);
    WiFi.begin(WLAN_SSID, WLAN_PASSWD);
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("Connection Failed! Rebooting...");
        delay(WLAN_RETRY_INTERVAL);
        ESP.restart();
    }
}

void setupSerial() {
    Serial.begin(SERIAL_BAUD);

    Serial.println();
    Serial.println("LED Strip initializing...");
    Serial.flush();
}

void setupStatusLED() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
}

void setupStrip() {
    Serial.println("Setting up LEDs ...");
    strip.Begin();
    strip.Show();
}

#endif
