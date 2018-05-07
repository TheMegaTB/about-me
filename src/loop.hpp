#ifndef LOOP_HPP
#define LOOP_HPP

#include <Arduino.h>
#include <ArduinoOTA.h>
#include "../lib/Colors/Colors.hpp"
#include "../lib/Colors/Range.hpp"
#include "../lib/Colors/Fade.hpp"
#include "strip.hpp"

// ---- Random junk that needs to go elsewhere

std::vector<RGBWColor*> sunKeyframes = {
    new RGBWColor(  0,   0,   0,   0),
    new RGBWColor(  0,   0, 255,   0),
    new RGBWColor(255,   0, 255,   0),
    new RGBWColor(255,   0,   0, 150),
    new RGBWColor(255, 170,   0, 150),
    new RGBWColor(255, 170, 255, 255),
    new RGBWColor(255, 255, 255, 255)
};

std::vector<RGBWColor*> ambientKeyframes = {
    new RGBWColor(  0,   0,   0,   0),
    new RGBWColor(  0,   0, 255,   0),
    new RGBWColor(  0,   0, 255,   0),
    new RGBWColor( 40,   0, 255,   0),
    new RGBWColor(170,  20, 255,  15),
    new RGBWColor(255,  40, 255,  80),
    new RGBWColor(255, 255, 255, 255)
};

int currentKeyframe = -1;
uint16_t keyframeDuration = 15000;
uint16_t currentKeyframePassedTime = keyframeDuration + 1;

// Sun properties
StartEndRange* sunRange = new StartEndRange(125, 175);

struct RangedFade {
    ColorFade fade;
    Range* range;
};

std::vector<RangedFade> fades;

// ---- Loop functions ----

void handleOTA() {
    ArduinoOTA.handle();
}

void handleAnimation() {
    // -- Sunrise simulation --
    // Do not continue the simulation when the last keyframe has been reached
    if (currentKeyframe < 6) {
        // Iterate through the keyframes
        currentKeyframePassedTime += TARGET_LOOP_MILLIS;
        if (currentKeyframePassedTime > keyframeDuration) {
            currentKeyframePassedTime = 0;
            currentKeyframe++;
            Serial.println(currentKeyframe);

            // Queue the fade to the next keyframe
            if (currentKeyframe < 6) {
                // Ambient
                fades.push_back({
                    ColorFade(ambientKeyframes[currentKeyframe], ambientKeyframes[currentKeyframe + 1], keyframeDuration),
                    new StartEndRange(0, STRIP_LED_COUNT)
                });

                // Sun
                fades.push_back({
                    ColorFade(sunKeyframes[currentKeyframe], sunKeyframes[currentKeyframe + 1], keyframeDuration),
                    sunRange
                });
            }
        }
    }
}

void handleFade() {
    if (fades.size() > 0) {
        auto it = fades.begin();
        while(it != fades.end()) {
            setColorInRange(it->fade.step(TARGET_LOOP_MILLIS), it->range, false);

            if (it->fade.completed) {
                Serial.println("Fade reset.");
                it->fade.reset();
                // Serial.println("Fade done.");
                // it = fades.erase(it);
            } else it++;
        }
        strip.Show();
    }
}


long loopStart = 0;
bool notified = false;
void handleLoopDuration() {
    long loopDuration = millis() - loopStart;
    if (loopDuration < TARGET_LOOP_MILLIS) {
        delay(TARGET_LOOP_MILLIS - loopDuration);
        if (notified) {
            digitalWrite(LED_BUILTIN, HIGH);
            notified = false;
        }
    } else {
        // Turn on the LED to notify!
        if (!notified) {
            digitalWrite(LED_BUILTIN, LOW);
            notified = true;
        }
        Serial.print("Can't keep up - ");
        Serial.print(loopDuration - TARGET_LOOP_MILLIS);
        Serial.println("ms behind!");
    }

    loopStart = millis();
}

#endif
