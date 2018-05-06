#ifndef STRIP_HPP
#define STRIP_HPP

#include <Arduino.h>
#include <NeoPixelBus.h>
#include "../lib/Colors/Colors.hpp"
#include "../lib/Colors/Range.hpp"
#include "config.hpp"

NeoPixelBus<NeoGrbwFeature, Neo800KbpsMethod> strip(STRIP_LED_COUNT);

typedef uint16_t led_index;

template<typename C, typename R>
void setColorInRange(C color, R range, bool show) {
    RGBWColor rgbwColor = color.toRGBW();
    RgbwColor c = RgbwColor(rgbwColor.r, rgbwColor.g, rgbwColor.b, rgbwColor.w);
    range.forEach([&](led_index i) {
        strip.SetPixelColor(i, c);
    });
    if (show) strip.Show();
}

template<typename C, typename R>
void setColorInRange(C color, R range) {
    setColorInRange(color, range, true);
}

template<typename C>
void setColor(C color, bool show) {
    setColorInRange(color, StartEndRange(0, STRIP_LED_COUNT), show);
}

template<typename C>
void setColor(C color) {
    setColor(color, true);
}

#endif
