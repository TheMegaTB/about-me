#include "Colors.hpp"

float getGammaCorrected(uint8_t value, double gammaValue) {
 return pow( (float) value / 255.0, gammaValue) * 255.0 + 0.5;
}

RGBWColor RGBWColor::getIntermediateColor(RGBWColor other, double mix) {
  uint16_t r = this->r * (1 - mix) + other.r * mix;
  uint16_t g = this->g * (1 - mix) + other.g * mix;
  uint16_t b = this->b * (1 - mix) + other.b * mix;
  uint16_t w = this->w * (1 - mix) + other.w * mix;

  return RGBWColor(r, g, b, w);
}

RGBWColor RGBWColor::getCorrected() {
  return RGBWColor(
    getGammaCorrected(this->r, gammaValues.r),
    getGammaCorrected(this->g, gammaValues.g),
    getGammaCorrected(this->b, gammaValues.b),
    getGammaCorrected(this->w, gammaValues.w)
  );
}
