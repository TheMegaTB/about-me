#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

#include "../lib/Colors/Colors.hpp"
#include "setup.hpp"
#include "strip.hpp"
#include "loop.hpp"

auto blueBlack = new HSIColor(240, 1, 0);
auto blue = new HSIColor(240, 1, 1);
auto fade = ColorFade(blueBlack, blue, 2000);
auto range = new StartEndRange(150, 200);

void setup() {
    setupSerial();
    setupWiFi();
    setupOTA();
    setupStrip();
    setupStatusLED();

    Serial.println("Setup done.");

    auto red = HSIColor(0, 1, 1);
    auto black = HSIColor(0, 1, 0);

    delay(500);
    setColor(&red);
    delay(1000);
    setColor(&black);
    delay(2000);

    fades.push_back({ fade, range });

    Serial.println("Beginning loop ...");
}

void loop() {
    handleOTA();
    // handleAnimation();
    handleFade();
    // handleLoopDuration();
}
