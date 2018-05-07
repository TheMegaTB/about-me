#ifndef STRIP_HPP
#define STRIP_HPP

#include <Arduino.h>
#include <NeoPixelBus.h>
#include "../lib/Colors/Colors.hpp"
#include "../lib/Colors/Range.hpp"
#include "config.hpp"

NeoPixelBus<NeoGrbwFeature, Neo800KbpsMethod> strip(STRIP_LED_COUNT);

typedef uint16_t led_index;

void setColorInRange(Color* color, Range* range, bool show) {
    // TODO MARK Source of evil #2
    // RGBWColor* rgbwColor = color->toRGBW();
    // RgbwColor c = RgbwColor(rgbwColor->r, rgbwColor->g, rgbwColor->b, rgbwColor->w);
    RgbwColor c(0, 0, 0, 0);
    range->forEach([&](led_index i) {
        strip.SetPixelColor(i, c);
    });
    if (show) strip.Show();
}

void setColorInRange(Color* color, Range* range) {
    setColorInRange(color, range, true);
}

void setColor(Color* color, bool show) {
    auto fullStripRange = StartEndRange(0, STRIP_LED_COUNT);
    setColorInRange(color, &fullStripRange, show);
}

void setColor(Color* color) {
    setColor(color, true);
}

#endif
