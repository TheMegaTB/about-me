#ifndef COLORS_HPP
#define COLORS_HPP

#include <Arduino.h>

// TODO: Move elsewhere
const struct gammaValuesStruct {
  double r = 1;
  double g = 2;
  double b = 1.4;
  double w = 2.2;
} gammaValues;

float getGammaCorrected(uint8_t value, double gammaValue);
class RGBWColor;

template<typename C>
class Color {
  virtual RGBWColor toRGBW() = 0;
  virtual C getIntermediateColor(C other, double mix); // mix = 0 -> this || mix = 1 -> other
};

class RGBWColor: public Color<RGBWColor> {
public:
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t w;

  RGBWColor(uint8_t r, uint8_t g, uint8_t b, uint8_t w) : r(r), g(g), b(b), w(w) {};

  RGBWColor toRGBW() { return RGBWColor(this->r, this->g, this->b, this->w); }

  RGBWColor getIntermediateColor(RGBWColor other, double mix);

  RGBWColor getCorrected();
};

class RGBColor: public Color<RGBColor> {
public:
  uint8_t r;
  uint8_t g;
  uint8_t b;

  RGBColor(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {};

  RGBWColor toRGBW() {
    uint8_t minValue = min(this->r, min(this->g, this->b));
    return RGBWColor(
      this->r - minValue,
      this->g - minValue,
      this->b - minValue,
      minValue
    );
  }

  RGBColor getIntermediateColor(RGBColor other, double mix) {
    uint16_t r = this->r * (1 - mix) + other.r * mix;
    uint16_t g = this->g * (1 - mix) + other.g * mix;
    uint16_t b = this->b * (1 - mix) + other.b * mix;

    return RGBColor(r, g, b);
  }
};

class HSIColor: public Color<HSIColor> {
public:
  float h; // Hue
  float s; // Saturation
  float i; // Intensity

  HSIColor(float h, float s, float i)
    : h(fmod(h, 360)), // wrap around to 0-360 degrees
      s(s > 0 ? (s < 1 ? s : 1) : 0), // Limit s and i to interval [0, 1]
      i(i > 0 ? (i < 1 ? i : 1) : 0)
    {};

  RGBWColor toRGBW() {
    int r, g, b;
    float H = 3.14159 * this->h / 180.0; // Convert to radians.
    float S = this->s;
    float I = this->i;

    // Math! Thanks in part to Kyle Miller.
    if(H < 2.09439) {
      r = 255*I/3*(1+S*cos(H)/cos(1.047196667-H));
      g = 255*I/3*(1+S*(1-cos(H)/cos(1.047196667-H)));
      b = 255*I/3*(1-S);
    } else if(H < 4.188787) {
      H = H - 2.09439;
      g = 255*I/3*(1+S*cos(H)/cos(1.047196667-H));
      b = 255*I/3*(1+S*(1-cos(H)/cos(1.047196667-H)));
      r = 255*I/3*(1-S);
    } else {
      H = H - 4.188787;
      b = 255*I/3*(1+S*cos(H)/cos(1.047196667-H));
      r = 255*I/3*(1+S*(1-cos(H)/cos(1.047196667-H)));
      g = 255*I/3*(1-S);
    }

    return RGBWColor(
      getGammaCorrected(r, gammaValues.r),
      getGammaCorrected(g, gammaValues.g),
      getGammaCorrected(b, gammaValues.b),
      getGammaCorrected(255 * (1-S), gammaValues.w)
    );
  }

  HSIColor getIntermediateColor(HSIColor other, double mix) {
    float hueDistance = abs(other.h - this->h);
    if (this->h > other.h) {
      float newDistance = abs((other.h + 360) - this->h);
      if (newDistance < hueDistance) other.h += 360;
    } else {
      float newDistance = abs(other.h - (this->h + 360));
      if (newDistance < hueDistance) this->h += 360;
    }

    float h = this->h * (1 - mix) + other.h * mix;
    float s = this->s * (1 - mix) + other.s * mix;
    float i = this->i * (1 - mix) + other.i * mix;

    return HSIColor(h, s, i);
  }
};

#endif
