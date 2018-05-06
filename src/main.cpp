#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

#include "../lib/Colors/Colors.hpp"
#include "setup.hpp"
#include "strip.hpp"
#include "loop.hpp"

void setup() {
    setupSerial();
    setupWiFi();
    setupOTA();
    setupStrip();
    setupStatusLED();

    Serial.println("Setup done.");

    // auto fade = ColorFade<HSIColor>(HSIColor(240, 1, 0), HSIColor(240, 1, 1), 2000);
    // auto range = StartEndRange(150, 200);
    // fades.push_back({ fade, range });

    setColor(HSIColor(0, 1, 1));
    delay(1000);
    setColor(HSIColor(0, 1, 0));
    delay(2000);

    Serial.println("Beginning loop ...");
}

void loop() {
    handleOTA();
    handleAnimation();
    handleFade();
    handleLoopDuration();
}
